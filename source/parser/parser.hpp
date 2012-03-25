#ifndef PARSER_H
#define PARSER_H

//custom incudes
#include "../tokenizer/tokenizer.h"
#include "../registration/registration.h"

//std includes
#include <string>
#include <deque>

//Qt includes
#include <QString>

struct Variable
{
    std::string name;
    std::string type;
};
/** Contains various possiblities to make custom types in Java.
  */
enum Structures{strStructure, strClass, strInterface};

namespace Java{
/** Visibility levels in Java
  * public: freely accessable
  * protected: freely accessable by derived classes and package classes
  * default: freely accessable by package classes
  * private: not accessable
  */
enum VisibilityLevels{vsPublic, vsProtected, vsDefault, vsPrivate};

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

/** Contains values used to represent the function of a identifier in a class. Members in classes are eihter functions, variables or constants.
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
    vector<Token> initializationValue;
};

/// @todo not very nice, clean up that mess!
template<int> class Parser;

/// a helper function, that requires a deque called tokens and a Token called current to be declared in the same scope. It takes the first element out of the deque and stores it in the variable current, then removing that element from the deque.
#define next_token() current = tokens.front(); tokens.pop_front()
template<>
class Parser<strClass>
{
private:
    Java::VisibilityLevels visibilityLevel;
    std::string baseClass;
    std::string className;
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
    void parse()
    {
        Token current;
        next_token();
        // check for visiblility level
        if(Java::isVisibilityLevel(current.token))
        {
            visibilityLevel = Java::toVisibilityLevel(current.token);
            // just to ensure integrity, ensure that the next token is "class"
            next_token();
            /// @todo better error handling: continue looking for a token that is "class"
            if(current.token != "class")
                throw "Cannot find beginning of class declaration";
        }
        else if(current.token != "class")
            throw "Cannot find beginning of class declaration";
        // expecting a class name, i.e a identifier
        next_token();
        if(current.kind == tkIdentifier)
            className = current.token;
        else
            throw "Error parsing class: Cannot retrieve class name.";
        next_token();
        if(current.token == "extends")
        {
            next_token();
            if(current.kind == tkIdentifier)
                baseClass = current.token;
            else
                throw "Error parsing class: no base class specified";
        }
        next_token();
        if(current.token != "{")
            throw "expected '{' after class name";
        // Now, the 'header' is completly processed and members and variables follow
        // the abort condition is an emergency: it prevents the function from hanging up if the class definiton is corrupt. For well-formed class defintions, the while loop will end at if(token == "}") break;
        while(!tokens.empty())
        {
            Member member;
            // is our member a constant?
            bool constant = false;
            // Syntax of every member declaration:
            // function: [visibility] type name(){[...]}
            // variable: [visibility] [final] type name;
            // constant: equal to variable, but with const keyword
            // So, we look for known types until we encounter a not yet known identifier. The not yet known identifier ergo is the member's name. We can easily distinguish variable and function declaration through the brackets '()'. Syntactic elements (consider <> in list<..>) are ignored.
            /// @todo check for correct syntax (just more integrity)
            // Extract all keywords (= visibility level and "final" keyword)
            next_token();
            if(token == "}")
                break;
            while(current.kind == tkKeyword)
            {
                next_token();
                if(Java::isVisibilityLevel(current.token))
                    Member.visibilityLevel = Java::toVisibilityLevel(current.token);
                else if(current.token == "final")
                    constant = true;
            }

            // Extract all tokens that determine the member's type. (look 10 lines up for further explanation)
            while(isKnownType(current) || current.kind != tkIdentifier)
            {
                member.type += current.token;
                next_token();
            }
            // an identifier that is not a type has been found => it is name of our member!
            memberName = current.token;
            // now, we need to determine, what kind of member we've found. There are ALWAYS round brackets after the function's name in a function declaration.
            next_token();
            // function declaration
            if(current.token == "(")
            {
                // leading bracket "{"
                member.initializationValue.push_back(current);
                while(current != "}")
                {
                    // function parameters and function body
                    member.initializationValue.push_back(current);
                    next_token();
                }
                // closing bracket "}"
                member.initializationValue.push_back(current);
                functions.push_back(member);
            }
            // variable
            else if(current.token == "=")
            {
                while(current.token != ";")
                    member.initializationValue.push_back(current);
                if(constant)
                    constans.push_back(member);
                else
                    variables.push_back(member);
            }

        }
    }

    void toCPPSourceCode();
    void toCPPHeader():
};

#endif // PARSER_H
