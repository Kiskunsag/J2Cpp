#ifndef PARSER_H
#define PARSER_H

#include <string>

// tokens do NOT give information related to the type!
// IMPORTANT! tkConstant applies to values that do not have a corresponding identifier. Constants like    "public static final int MAX_UNITS = 25;" are considered identifiers.
enum tokenKind{tkIdentifier, tkOperator, tkConstant};

bool parsefile(std::string fileName, bool recursive = false);

struct Token{
    std::string token;
    tokenKind kind;
};

Token make_token(std::string token, tokenKind kind)
{
    Token res;
    res.token = token;
    res.kind = kind;
    return res;
}

#endif // PARSER_H
