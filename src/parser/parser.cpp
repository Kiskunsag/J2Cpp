
std::string ClassParser::toCPPHeader() {
        // header
        // class foo [: bar] {
        string result = "class " + name;
        if(baseClass != "")
            result += " : " + baseClass;
        result += "{\r\n";
        string constants;

        for(Java::VisibilityLevels lvl = Java::vsPublic; i <= Java::vsPrivate; i++)
        {
            switch(lvl)
            {
            case Java::vsPublic :    result += "public:\\r\\n";
            case Java::vsProtected : result += "protected:\\r\\n";
            /// @todo: other case that private for vsDefault. Anonymous namespace?
            case Java::vsDefault:
            case Java::vsPrivate : result += "private:\\r\\n";
            }

        // members
        for(std::vector<Member>::iterator i = variables.begin(); i != variables.end(); i++)
        {
            // Only if visible in level lvl and a variable - constants will be processed in an own namespace which has the same as the class itself.
            if(lvl == i->visibilityLevel)
            {
                if(i->kind == mkVariable)
                {
                    // type name [= initialization];
                    result += i->type + " " + i->name;
                    if(i->initializationValue != "")
                      result += " = " + i->initializationValue;
                    result += ";\\r\\n";
                }
                else if (i->kind == i->visibilityLevel == mkConstant)
                {
                    if(i->initializationValue == "")
                        throw "error: constant '" + i->name + "' in class '" +  name + "' has no initialization value.";
                    constants += "static const" +  i->type + " " + i->name + " = " + i->initializationValue;
                    constants += ";\\r\\n";
                }
            }
        }
        for(std::vector<Member>::iterator i = functions.begin(); i != functions.end(); i++)
        {
            if(lvl == i ->visibilityLevel)
            {
                // type name ([arg])
                result += i->type + " " + i->name;
                // iterator charwise until the first closing bracket is found. After the closing bracket, the implementation follows, but we ignore that as we are just writing the header
                /// @TODO: Java is call by value, but objects are passed as references: Changing the passed object will affect the original object, however setting the passed object to null will not affect the original object. In C++, this is only possible with pointers! Java's primitives are plain call by value, without something outrageous.
                for(std::vector<Token>::iterator j = i->initializationValue.begin(); j != i->initializationValue.end() && j->token != ')'; j++) result += *j;
                result += ");";
            }
        }
        }

        // every constant is placed in a namespace that is called like the currently parsed class.
        if (constants != "")
        {
            result += "\\r\\n namespace " + name + "{\\r\\n";
            result += constants;
            result += "};\\r\\n";
        }

        result += "};";
        return result;
    }

std::string ClassParser::toCPPSourceCode()
    {
        for(std::vector<Member>::iterator i = functions.begin(); i != functions.end(); i++)
        {

        }
    }

void ClassParser::parse()
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
            name = current.token;
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
