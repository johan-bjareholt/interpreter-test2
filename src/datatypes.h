#ifndef DATATYPES_H
#define DATATYPES_H

enum DataTypes {
    TYPE_NONE,
    TYPE_VAR,
    TYPE_FUNC,
    TYPE_INT,
    TYPE_STR,
    TYPE_PLUS,
    TYPE_MINUS,
    TYPE_MULTIPLICATION,
    TYPE_DIVISION,
    TYPE_MODULO,
    TYPE_ASSIGNMENT,
    TYPE_COMPARISON,
    
    TYPE_PREPROCESS,
    TYPE_SUBPROCESS,
};

extern char* DataTypeNames[];

#endif
