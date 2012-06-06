#ifndef PARSER_H
#define PARSER_H

//custom incudes
#include "../tokenizer/tokenizer.h"
#include "../registration/registration.h"
#include "../globals.h"

//std includes
#include <string>
#include <deque>
#include <vector>

//Qt includes
#include <QString>

namespace Java{
/** Visibility levels in Java. Members are ALWAYS accessable inside their class.
  */
enum VisibilityLevels{
    /// freely accessable
    vsPublic,
    /// freely accessable by derived classes and package classes
    vsProtected,
    /// freely accessable by package classes
    vsDefault,
    /// not accessable
    vsPrivate};

VisibilityLevels toVisibilityLevel(std::string lvl)
{
    if(lvl == "public")
        return Java::vsPublic;
    else if(lvl == "protected")
        return Java::vsProtected;
    else if(lvl == "default")
        return Java::vsDefault;
    else if(lvl == "private")
        return Java::vsPrivate;
    else
        throw "Error in toJavaVisibilityLevel(): '" + lvl + "' is not a visibility-keyword!" ;
}

bool isVisibilityLevel(std::string lvl)
{
    try{
        toVisibilityLevel(lvl);
        return true;
    }
    catch(...)
    {
        return false;
    }
}

}

/** Contains values used to represent the function of a identifier in a class. Members in classes are either functions, variables or constants.
  */
enum MemberKind{mkFunction, mkVariable, mkConstant};

struct Member
{
    /// @see MemberKind
    MemberKind kind;
    /// type of the member (like int, string etc.)
    std::string type;
    /// every member in a class has a name
    std::string memberName;
    /// every member in a class has a visibility level. @see Java::visibilityLevel
    Java::VisibilityLevels visibilityLevel;
    /// optional. Care when interpreting this value! If type is mkFunction, initializationValue contains the function body + parameters, like this: ([parameters]){function body}. Else, it will contain an initial value for a constant/variable.
    std::vector<Token> initializationValue;
};

/// a helper function that requires a deque called tokens and a Token called current to be declared in the same scope. It takes the first element out of the deque and stores it in the variable current, aftewards removing the taken element from the deque. Do not forget to put a semicolon after the macro! @internal The macro was designed to not require a semicolon at the end, but the indent of QtCreator did not accept that...
#define next_token() current = tokens.front(); tokens.pop_front()
class ClassParser
{
private:
    Java::VisibilityLevels visibilityLevel;
    std::string baseClass;
    std::string name;
    std::deque<Token> & tokens;
    std::vector<Member> variables;
    std::vector<Member> functions;
public:
    /// using a deque for the pop_front()-method and faster insertation/deletion at front and back
    Parser(std::deque<Token> & Tokens);
    /**
      * Parses a java class. Syntax of a java class:
      * [visiblity level] class className [extends className] {
      * [visibility level] type variableName1;
      * [visibility level] type variableName2 [ = initializationValue ];
      * ...
      * [visibility level] type variableNameN;
      * [visibility level] type functionName1() {[definition];}
      * [visibility level] type functionName2() {[definition];}
      * ...
      * [visibility level] type functionNameN() {[definition];}
      */
    void parse();
    std::string toCPPSourceCode();
    std::string toCPPHeader();
};

#endif // PARSER_H
