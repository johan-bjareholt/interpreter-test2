
#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>

struct Token {
    // Origin data
    char* srcname;
    int linenr;
    int posnr;
    // Metadata
    int tokencategory;
    int tokentype;
    // Data
    char* content;
    // Prev/Next
    struct Token* prev;
    struct Token* next;
};

struct Token* create_token();

enum TokenTypes {
    TT_SYMBOL,
    TT_INT,
    TT_FLOAT,
    TT_STRING,
    TT_ARITHMETIC,
    TT_SCOPE,
    TT_SEPARATOR,
    TT_SPACING,
    TT_TERMINATOR,
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
