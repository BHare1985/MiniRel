#ifndef DATATYPES_H
#define DATATYPES_H

// Data types that minirel understands
enum Datatype { INTEGER = 0, DOUBLE = 1, STRING = 2 };

// Operations that are supported in predicates in minirel
enum Operator { LT, LTE, EQ, GTE, GT, NE, NOTSET };

#endif
