
#include <stdlib.h>

#include "token.h"

struct Token* create_token(){
    struct Token* token = malloc(sizeof(struct Token));
    return token;
}

const char* TokenTypeNames[] = {
    "TT_SYMBOL",
    "TT_INT",
    "TT_FLOAT",
    "TT_STRING",
    "TT_ARITHMETIC",
    "TT_SCOPE",
    "TT_SEPARATOR",
    "TT_SPACING",
    "TT_TERMINATOR",
};

void tat_reset(struct TokenAllowedTable* tat){
    tat->symbol = true;
    tat->integer = true;
    tat->floatt = true;
    tat->string = true;
    tat->arithmetic = true;
    tat->scope = true;
    tat->separator = true;
    tat->spacing = true;
    tat->terminator = true;
}

int tat_test(struct TokenAllowedTable* tat){
    int true_c = 0;
    int last_true = -1;
    if (tat->symbol){
        true_c++;
        last_true = TT_SYMBOL;
    }
    if (tat->integer){
        true_c++;
        last_true = TT_INT;
    }
    if (tat->floatt){
        true_c++;
        last_true = TT_FLOAT;
    }
    if (tat->string){
        true_c++;
        last_true = TT_STRING;
    }
    if (tat->arithmetic){
        true_c++;
        last_true = TT_ARITHMETIC;
    }
    if (tat->scope){
        true_c++;
        last_true = TT_SCOPE;
    }
    if (tat->separator){
        true_c++;
        last_true = TT_SEPARATOR;
    }
    if (tat->spacing){
        true_c++;
        last_true = TT_SPACING;
    }
    if (tat->terminator){
        true_c++;
        last_true = TT_TERMINATOR;
    }
    if (true_c > 1)
        return -1;
    else if (true_c < 1)
        return -2;
    else
        return last_true;
}
