#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast_builder.h"

void build_ast(struct Token* token, int type_end){
    while (token != NULL){
        // Prepare for next token
        token = token->prev;
    }
}
