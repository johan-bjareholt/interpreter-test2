
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "input.h"
#include "token.h"


bool isSymbol(int pos, char chr);

bool isInteger(int pos, char chr);

bool isFloat(int pos, char chr);

bool isString(int pos, char chr);

bool isArithmetic(int pos, char chr);

bool isScope(int pos, char chr);

bool isSeparator(int pos, char chr);

bool isSpacing(int pos, char chr);

bool isTerminator(int pos, char chr);

struct Token* lex(char* line){

    int len = strlen(line);

    struct Token* last_token = NULL;

    struct TokenAllowedTable tat;
    tat_reset(&tat);
    int tat_last = -1;

    int tokenstart = 0;
    for (int i=0; i<len; i++){
        //printf("%i: %c\n", i, line[i]);
        int pos = i-tokenstart;
        if (tat.symbol && !isSymbol(pos, line[i]))
            tat.symbol = false;
        if (tat.integer && !isInteger(pos, line[i]))
            tat.integer = false;
        tat.floatt = isFloat(pos, line[i]);
        if (tat.string && !isString(pos, line[i]))
            tat.string = false;
        if (tat.arithmetic && !isArithmetic(pos, line[i]))
            tat.arithmetic = false;
        if (tat.scope && !isScope(pos, line[i]))
            tat.scope = false;
        if (tat.separator && !isSeparator(pos, line[i]))
            tat.separator = false;
        if (tat.spacing && !isSpacing(pos, line[i]))
            tat.spacing = false;
        if (tat.terminator && !isTerminator(pos, line[i]))
            tat.terminator = false;

        int result = tat_test(&tat);
        if (result < 0){
            int type = tat_last;
            if (type == -2){
                printf("Critical error: No candidates for %i: %s\n", pos, line+tokenstart);
            }
            else if (type == -1){
                printf("Error: Multiple candidates for %i: %s\n", pos, line+tokenstart);
                printf("This is typically because of a invalid syntax or invalid char\n");
                i++;
                exit(-1);
            }
            // Simply skip spacing
            else if (type == TC_SPACING){}
            else {
                char* tokenstr = malloc(pos);
                strncpy(tokenstr, line+tokenstart, pos);
                // Debug text
                printf("+%i:%s\n", pos, tokenstr);
                printf("%s\n", TokenCategoryNames[type]);
                // Create token
                struct Token* token = create_token(type, tokenstr);
                token->srcname = NULL;
                token->linenr = input_line_nr();
                token->posnr = tokenstart;

                if (last_token == NULL){
                    last_token = token;
                }
                else {
                    last_token->next = token;
                    token->prev = last_token;
                    last_token = token;
                }
            }
            tokenstart = i;
            tat_reset(&tat);
            tat_last = -1;
            i--;
        }
        else {
            tat_last = result;
        }
    }

    return last_token;
}


bool isSymbol(int pos, char chr){
    if (pos == 0 && (chr >= '0' && chr <= '9'))
        return false;
    if ((chr >= 'a' && chr <= 'z') ||
        (chr >= 'A' && chr <= 'Z') ||
        (chr >= '0' && chr <= '9'))
        return true;
    else
        return false;
}

bool isSpacing(int pos, char chr){
    if (chr == ' ')
        return true;
    else
        return false;
}

bool isInteger(int pos, char chr){
    if (chr >= '0' && chr <= '9')
        return true;
    else
        return false;
}

static int valid_chars = -1;
static bool point = false;
bool isFloat(int pos, char chr){
    if (valid_chars > pos || pos == 0){
        valid_chars = -1;
        point = false;
    }
    if (chr >= '0' && chr <= '9')
        valid_chars++;
    else if (chr == '.'){
        if (point)
            valid_chars = -1;
        else {
            point = true;
            valid_chars++;
        }
    }
    else
        valid_chars = 0;

    //printf("%i %i %i \n", pos, valid_chars, point );
    if (valid_chars == pos && point == true)
        return true;
    else
        return false;
}

static int str_last_end_pos = -2;
bool isString(int pos, char chr){
    if (str_last_end_pos == pos-1)
        return false;
    else
        str_last_end_pos = -2;

    if (pos == 0){
        if (chr == '"')
            return true;
        else
            return false;
    }
    else {
        if (chr == '"'){
            str_last_end_pos = pos;
            return true;
        }
        else
            return true;
    }
}

bool isArithmetic(int pos, char chr){
    if (pos == 0){
        switch(chr){
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
                return true;
            default:
                return false;
        }
    }
    else if (pos == 1){
        switch(chr){
            case '=':
            case '+':
            case '-':
            case '*':
                return true;
            default:
                return false;
        }
    }
    return false;
}

bool isScope(int pos, char chr){
    // Implement this properly
    if (pos == 0 && (
            chr == '{' || chr == '}' ||
            chr == '(' || chr == ')' ||
            chr == '[' || chr == ']'
        ))
        return true;
    else
        return false;
}

bool isSeparator(int pos, char chr){
    if (pos == 0){
        switch (chr){
            case ',':
                return true;
            default:
                return false;
        }
    }
    else
        return false;
}

bool isTerminator(int pos, char chr){
    switch (chr){
        case '\n':
        case ';':
            return true;
        default:
            return false;
    }
}
