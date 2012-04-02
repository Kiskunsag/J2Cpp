#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>

// tokens do NOT give information related to the type!
// IMPORTANT! tkConstant applies to values that do not have a corresponding identifier. Constants like    "public static final int MAX_UNITS = 25;" are considered identifiers.
/** Every token represents a string that fits one of these catgories:
  * tkIdentifier: Any java identifier (variable names, constants' names, class names etc.)
  * tkOperator: Any java operator: unary, binary or ternary
  * tkKeyword: Any java keyword.
  * tkOther: Following names are classified as tkOther, because they do not fit any category above: { } ; < >
  */
enum tokenKind{tkIdentifier, tkOperator, tkConstant, tkKeyword, tkOther, tkUnknown};

/** Parses a file and returns the file in tokenized form.
  * @throw string
  */
void parsefile(std::string fileName, bool recursive = false);

struct Token{
    std::string token;
    tokenKind kind;
};

/// @todo merge files into one file 'java.txt'
/// @todo put constants into file 'settings.txt' / 'settings.ini' / 'settings.xml'
static const char keywordFilePath[] = "res/KEYWORDS.TXT";
static const char literalsFilePath[] = "res/LITERALS.TXT";

/** Constructs a token.
  * @group helper-function
  */
Token make_token(std::string token, tokenKind kind)
{
    Token res;
    res.token = token;
    res.kind = kind;
    return res;
}

#endif // TOKENIZER_H
