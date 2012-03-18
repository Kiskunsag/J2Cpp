#include "parser.h"

//Qt includes
#include <QFile>
#include <QHash>
#include <QTextStream>
#include <QRegExp>
#include <QStringList>

//std includes
#include <string>
#include <vector>

using namespace std;


// idea: every "{" opens a new block which can be used to divide the variables from each other. Consequently, every "}" closes such a line. Internally, we will represent staring and ending blocks with their "name" (i.e. function name, class name, struct name or <anon> for anonymous namespacess) and divide them with "::". So, if a nested class "nested" embedded in a class "upper" contains a variable int i, the type of i can be looked up by searching "upper::nested::i", which will return a enum-value that determines the type. The replacement-parser doesn't care for exact type evidence, it only tries to make the syntax fit.

/**
  @todo verify that after the translation, no identifer equals a reserved keyword. That can easily be done as soon as the source code is entirely tokenized.
  */



bool parsefile(std::string fileName, bool recursive)
{

    // <[namespaces::]variable, type>
    QHash<string, string> variables;
    vector<Token> tokens;

    QFile file(QString::fromStdString(fileName));
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString line = in.readLine();
    string block("");
    while(!line.isNull()){

        // Now, our task is to tokenize the line
        // a few Java Code. Our tasks appear as comment-lines:

        /*
// This example is from the book _Java in a Nutshell_ by David Flanagan.
// Written by David Flanagan.  Copyright (c) 1996 O'Reilly & Associates.
// You may study, use, modify, and distribute this example for any purpose.
// This example is provided WITHOUT WARRANTY either expressed or implied.

// flat syntax replacement: #include instead of import. Warn about applet and awt are not available for C++.
import java.applet.*;
import java.awt.*;

// register cllass Scribble, mark it as public, extends Applet. No tokes are created, but block is set to Scribble.
public class Scribble extends Applet {
    // register various variables.
    private int last_x = 0;
    private int last_y = 0;
    private Color current_color = Color.black;
    private Button clear_button;
    private Choice color_choices;

    // register function Scribble()::init(), returns void. No tokens are created, but block is set to Scribble:init.
    public void init() {
        // Set the background color
        // <id this><access><func::begin setBackground><id Color><access><id white><func::end setBackground><end>
        // After tokenizing, types are thrown in:
        // <Scribble* this><access><func::begin setBackground><enum Color><access><int white><func::end setBackground><end>
        // Now, every token is translated into valid C++.
        // <Scribble* this> this
        // <access> -> (as this is a pointer)
        // <func::begin setBackground> setBackground(
        // <enum Color> Color
        // <access> :: (as Color is an enum)
        // <int white> white
        // <func::end setBackground> )
        // <end> ; (as statement is over)
        // the resulting C++-statement is: this->setBackground(Color::white);
        this.setBackground(Color.white);

        // Create a button and add it to the applet.
        // Also, set the button's colors
        clear_button = new Button("Clear");
        clear_button.setForeground(Color.black);
        clear_button.setBackground(Color.lightGray);
        this.add(clear_button);

        // Create a menu of colors and add it to the applet.
        // Also set the menus's colors and add a label.
        color_choices = new Choice();
        color_choices.addItem("black");
        color_choices.addItem("red");
        color_choices.addItem("yellow");
        color_choices.addItem("green");
        color_choices.setForeground(Color.black);
        color_choices.setBackground(Color.lightGray);
        this.add(new Label("Color: "));
        this.add(color_choices);
    }

    // Called when the user clicks the mouse to start a scribble
    public boolean mouseDown(Event e, int x, int y)
    {
        last_x = x; last_y = y;
        return true;
    }

    // Called when the user scribbles with the mouse button down
    public boolean mouseDrag(Event e, int x, int y)
    {
        Graphics g = this.getGraphics();
        g.setColor(current_color);
        g.drawLine(last_x, last_y, x, y);
        last_x = x;
        last_y = y;
        return true;
    }

    // Called when the user clicks the button or chooses a color
    public boolean action(Event event, Object arg) {
        // If the Clear button was clicked on, handle it.
        if (event.target == clear_button) {
            Graphics g = this.getGraphics();
            Rectangle r = this.bounds();
            g.setColor(this.getBackground());
            g.fillRect(r.x, r.y, r.width, r.height);
            return true;
        }
        // Otherwise if a color was chosen, handle that
        else if (event.target == color_choices) {
            String colorname = (String) arg;
            if (arg.equals("black")) current_color = Color.black;
            else if (arg.equals("red")) current_color = Color.red;
            else if (arg.equals("yellow")) current_color = Color.yellow;
            else if (arg.equals("green")) current_color = Color.green;
            return true;
        }
        // Otherwise, let the superclass handle it.
        else return super.action(event, arg);
    }

}

          */ // taken from: http://oreilly.com/catalog/javanut/examples/section1/Scribble.java
        // In programming languages, delimiters are used to seperate tokens from each other. We will determine tokens like-wise and split up the expression. We need to take special care of binary operators like comparision operators.
        //line.split(".,;()")
        // Regex is: !=|==|>=|<=|\+\+|--|&&|\|\||\?\:|instanceof|<<|>>|>>>|\*|[=.,;()+-/%!><~&^\|]
        /// @todo: More syntax-checks (i.e. ||| must NOT recognized as || and |).
        // The character class is at the end, because == needs a higher precedence than =. Consider as well != against ! and =. Seperators are all operators and whitespaces
        QRegExp tokenSeperator("^[\\t]|!=|==|>=|<=|\\+\\+|--|&&|\\|\\||\\?\\:|instanceof|<<|>>|>>>|\\*|[=.,;()+-/%!><~&^\\|]");
        QStringList tokenStrings(line.split(tokenSeperator, QString::SkipEmptyParts));
        QString token;
        string tokenStdString = token.toStdString();

        /// @todo SINN VON SINGLECHARACTER UND DOUBLECHARACTER-AUFTEILUNG? GGF. TRENNZEICHEN VERWENDEN!

        string singleCharacterOperators = "=.,;()+-*/%!><~&^|";
        string doubleCharacterOperators = "!===>=<=++--&&||?:<<>>";

        // Check the current token:
        foreach(token, tokenStrings)
        {
            bool found = false;
            // Is the token an operator?
            switch(token.length())
            {
            // if the token is an operator, it must be one of the following: = . , ; ( ) + - / % ! > < ~ & ^ |
            case 1:
            {                
                // No regex here (performance issues)
                for(string::iterator i = singleCharacterOperators.begin(); i != singleCharacterOperators.end(); i++)
                    // Assuming that tokenStdString has length 1, both left and right hands of the expression are chars
                    if (*i == *tokenStdString.c_str())
                    {
                        found = true;
                        break;
                    }
            }
            case 2:
                // No regex here (performance issues)
                for(int i = 0; i < doubleCharacterOperators.size() -1; i+=2)
                    if (tokenStdString == doubleCharacterOperators.substr(i, 2))
                    {
                        found = true;
                        break;
                    }
             case 3:
                if(tokenStdString == ">>>")
                {
                    found = true;
                    break;
                }
             case 9:
                if(tokenStdString == "instanceof")
                {
                    found = true;
                    break;
                }
            }
            if(found)
            {
                tokens.push_back(make_token(token.toStdString(), tkOperator));
                continue;
            }

            // Is the token an identifier? If so, it may only contain alphanumeric chars and must not start with a number. Additionally, the identifier is registered, if it was not declared before.
            QRegExp alphanumeric("\\w");
            // ASCII 48-57 is 0-9, identifiers do not start with numbers
            if(token[0] < 48 && token[0] > 57 )
            {
                found = alphanumeric.exactMatch(token);
            }
            if(found)
            {
                tokens.push_back(make_token(token.toStdString(), tkIdentifier));
                continue;
            }
            // Check whether the token is a constant. If so, it must be a string (syntax: "\w+"), an integer (syntax: [+]|[-]12345), a float point value (syntax: 12345\.[12345{d|f}]), a boolean (syntax: true|false), a char (syntax: '\w')

            QRegExp str(" \"\\w+ \" ");
            found = str.exactMatch(token);
            if(found)
            {
                tokens.push_back(make_token(token.toStdString(), tkOperator));
                continue;
            }
            QRegExp integer("[+]|[-]12345");
            found = integer.exactMatch(token);
            if(found)
            {
                tokens.push_back(make_token(token.toStdString(), tkOperator));
                continue;
            }
            QRegExp point("\\d+\\.\\d+(?:d|f)");
            found = point.exactMatch(token);
            if(found)
            {
                tokens.push_back(make_token(token.toStdString(), tkOperator));
                continue;
            }
            QRegExp chr("'\\w'");
            found = chr.exactMatch(token);
            if(found)
            {
                tokens.push_back(make_token(token.toStdString(), tkOperator));
                continue;
            }

            /// @todo the token is a keyword

            // The token is obviously wrong. Comment it out with /* */ and add a /// @todo: error line or such.

        }

        // Now as the tokens are found, we need to find out their kind, i.e variable, class name, function, operator etc. The token's function can be found in the enum tokenKind
        // declaration (+initialization)
        // Syntax : type foo[;] | [ = bar]              for bar = primitive
        //          type foo[;] | [ = new bar([arg])]   for bar = class
        // initialization
        // Syntax : foo = bar;                          for bar = primitive
        //          foo = new bar([arg]);               for bar = class
        // assignment
        // Syntax : foo = bar;                          for bar = class
        // plain function calls, i.e no variable interaction at all. plain function calls and initializations are not easy to distinguish. Consider the following:

        // Foo(Bar); // Construction
        // Foo(Bar); // Function-call

        // There is no syntactic difference. The only way to distinguish, is to look out for a variable declaration. If there is a Foo Bar; line somewhere before the call, it must be a construction, else a function-call. That's why it's important to save variables and their type, in our case this is done by the HashMap "variables".

        line = in.readLine();
    }
}
