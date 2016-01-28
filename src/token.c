
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "token.h"

struct Token* create_token(int tokencategory, char* tokenstr){
    struct Token* token = malloc(sizeof(struct Token));
    token->category = tokencategory;
    token->content = tokenstr;
    switch (tokencategory){
        case TC_SYMBOL:
            goto TOKENCATEGORY_NOT_IMPLEMENTED;
            break;
        case TC_INT:
            token->type = TT_INT;
            break;
        case TC_FLOAT:
            token->type = TT_FLOAT;
            break;
        case TC_STRING:
            goto TOKENCATEGORY_NOT_IMPLEMENTED;
            break;
        case TC_ARITHMETIC:
            {
                int len = strlen(token->content);
                if (len == 1){
                    switch (token->content[0]){
                        case '+': token->type = TT_PLUS; break;
                        case '-': token->type = TT_MINUS; break;
                        case '*': token->type = TT_MULTIPLICATION; break;
                        case '/': token->type = TT_DIVISION; break;
                        case '%': token->type = TT_MODULO; break;
                        default:
                            goto TC_ARITHMETIC_NOT_IMPLEMENTED_ERROR;
                            break;
                    }
                }
                else {
                    TC_ARITHMETIC_NOT_IMPLEMENTED_ERROR:
                    printf("Operator %s is not implemented\n", token->content);
                    exit(-1);
                }
            }
            break;
        case TC_SCOPE:
            goto TOKENCATEGORY_NOT_IMPLEMENTED;
            break;
        case TC_SEPARATOR:
            goto TOKENCATEGORY_NOT_IMPLEMENTED;
            break;
        case TC_TERMINATOR:
            goto TOKENCATEGORY_NOT_IMPLEMENTED;
            break;
        default:
            printf("Fatal error: Unknown/Invalid tokencategory\n");
            exit(-1);
            TOKENCATEGORY_NOT_IMPLEMENTED:
            printf("Fatal error: Tokencategory not yet implemented\n");
            exit(-1);

    }
    printf("%s\n", TokenTypeNames[token->type]);
    return token;
}

const char* TokenCategoryNames[] = {
    "TC_SYMBOL",
    "TC_INT",
    "TC_FLOAT",
    "TC_STRING",
    "TC_ARITHMETIC",
    "TC_SCOPE",
    "TC_SEPARATOR",
    "TC_SPACING",
    "TC_TERMINATOR",
};

const char* TokenTypeNames[] = {
    // Object
    "TT_OBJECT",
    // Function
    "TT_FUNCTION",
    // Raw types
    "TT_INT",
    "TT_FLOAT",
    // Arithmetic
    "TT_PLUS",
    "TT_MINUS",
    "TT_MULTIPLICATION",
    "TT_DIVISION",
    "TT_MODULO",
    "TT_EQUALS",
    "TT_INCREASE",
    "TT_DECREASE",
    "TT_MULTIPLY",
    "TT_POWER",
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
        last_true = TC_SYMBOL;
    }
    if (tat->integer){
        true_c++;
        last_true = TC_INT;
    }
    if (tat->floatt){
        true_c++;
        last_true = TC_FLOAT;
    }
    if (tat->string){
        true_c++;
        last_true = TC_STRING;
    }
    if (tat->arithmetic){
        true_c++;
        last_true = TC_ARITHMETIC;
    }
    if (tat->scope){
        true_c++;
        last_true = TC_SCOPE;
    }
    if (tat->separator){
        true_c++;
        last_true = TC_SEPARATOR;
    }
    if (tat->spacing){
        true_c++;
        last_true = TC_SPACING;
    }
    if (tat->terminator){
        true_c++;
        last_true = TC_TERMINATOR;
    }
    if (true_c > 1)
        return -1;
    else if (true_c < 1)
        return -2;
    else
        return last_true;
}
