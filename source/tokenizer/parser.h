#ifndef PARSER_H
#define PARSER_H

/**
 * @file The idea behind the parser is, that it can give source-evidence for each file. So, if the parser that does the real translation afterwards, finds following Java-statements:

// bar.j
class Test {
    int i;
}

static class Main
    Test t;
    static void Main(){
        t = new Test();
        t.i = 17;
}

Now, it must look up the type of the variable t in the class Main in the file bar.j . That kind of data-organization must be represented in a dictionary, holding the file-name + path as a key and a type-dictionary at the other.

For that duty every variable, function and class are store in the corresponding vector holding t
 */

bool parsefile(std::string fileName, bool recursive = false);

#endif // PARSER_H
