#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast_builder.h"

void build_ast(struct Token* token, int type_end){
    while (token != NULL){
        // Prepare for next token
        switch (token->category){
            case TC_SYMBOL:
                break;
            case TC_ARITHMETIC:
                {
                    if (token->prev == NULL ||
                        !( token->prev->type == TT_INT || token->prev->type == TT_FLOAT)
                    ){
                        printf("ERROR: Operand must have a value or variable prior to it\n");
                        exit(-1);
                    }
                    else if (token->next == NULL ||
                        !( token->next->type == TT_INT || token->next->type == TT_FLOAT)
                    ){
                        printf("ERROR: Operand must have a value or variable following it\n");
                        exit(-1);
                    }
                    else {
                        struct Token* op1 = token->prev;
                        struct Token* op2 = token->next;

                        // Replace prev and next
                        if (token->prev->prev != NULL)
                            token->prev = token->prev->prev;
                        if (token->next->next != NULL)
                            token->next = token->next->next;

                        // operand 1
                        op1->prev = NULL;
                        op1->next = op2;

                        // operand 2
                        op2->prev = op1;
                        op2->next = NULL;

                        // Add to subtoken
                        token->subtoken_fist = op1;
                        printf("%s will operate on %s and %s\n", token->content, op1->content, op2->content);
                    }
                }
                break;
            default:
                break;
        }
        token = token->prev;
    }
}
