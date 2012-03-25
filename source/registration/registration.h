#ifndef REGISTRATION_H
#define REGISTRATION_H

#include "../tokenizer/tokenizer.h"

/** Checks whether the string in token represents a type.
  */
bool isKnownType(Token token);

/** Registers a string, so that the converter will treat it as type in the future. Primitives and classes are considered types.
  * @see isKnownType
  * @return true, if type could be registered. false, if type has already been registered.
  */
bool registerType(Token token);
#endif // REGISTRATION_H
