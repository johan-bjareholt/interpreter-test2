
#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>

struct Token {
    // Origin data
    char* srcname;
    int linenr;
    int posnr;
    // Metadata
    int category;
    int type;
    // Data
    char* content;
    struct Token* subtoken_fist;
    // Prev/Next
    struct Token* prev;
    struct Token* next;
};

struct Token* create_token(int tokencategory, char* content);

enum TokenCategories {
    TC_SYMBOL,
    TC_INT,
    TC_FLOAT,
    TC_STRING,
    TC_ARITHMETIC,
    TC_SCOPE,
    TC_SEPARATOR,
    TC_SPACING,
    TC_TERMINATOR,
};

extern const char* TokenCategoryNames[];

enum TokenTypes {
    // Object
    TT_OBJECT,
    // Function
    TT_FUNCTION,
    // Raw types
    TT_INT,
    TT_FLOAT,
    // Arithmetic
    TT_PLUS,
    TT_MINUS,
    TT_MULTIPLICATION,
    TT_DIVISION,
    TT_MODULO,
    TT_EQUALS,
    TT_INCREASE,
    TT_DECREASE,
    TT_MULTIPLY,
    TT_POWER,
    // Scopes
    TT_BRACES_OPEN,
    TT_BRACES_CLOSE,
    TT_BRACKETS_OPEN,
    TT_BRACKETS_CLOSE,
    TT_PARANTHESIS_OPEN,
    TT_PARANTHESIS_CLOSE,
};

extern const char* TokenTypeNames[];

struct TokenAllowedTable {
    bool symbol;
    bool integer;
    bool floatt;
    bool string;
    bool arithmetic;
    bool scope;
    bool separator;
    bool spacing;
    bool terminator;
};

void tat_reset(struct TokenAllowedTable* tat);

int tat_test(struct TokenAllowedTable* tat);

#endif
