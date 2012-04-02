#ifndef GLOBALS_H
#define GLOBALS_H

/**
  * @file Header for declarations that almost every file needs and conflicting declarations.
  */

/// @internal moved here from parser/parser.h, so that registration/registration.h doesn't need to include parser.h anymore.
struct Variable
{
    /// Name of the variable
    std::string name;
    /// Useful when dealing with more complex types (like arrays of arrays or such), that can not be expressed by type.
    std::string typeName;
    /// Type of the variable
    Types type;
};

/** Contains various kinds of Java-types.
  * @internal moved here from parser/parser.h, so that registration/registration.h doesn't need to include parser.h anymore.
  */
enum Types{
    /// Classes
    typClass,
    /// Interfaces
    typInterface,
    /// Primitive types, like int, float...
    typPrimitive,
    /// Arrays
    typArray
};
#endif // GLOBALS_H
