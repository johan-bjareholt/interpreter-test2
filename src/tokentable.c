#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "tokentable.h"

int hashgen(const char* symbolname);

struct Token* get_symbol_from_table(struct TokenTable* tokentable, const char* symbolname){
    struct TokenEntry* result = NULL;
    int hash = hashgen(symbolname);
    struct TokenEntry* pos = tokentable->table[hash];
    do {
        if (strcmp(symbolname, pos->name) == 0)
            result = pos;
        else
            pos = pos->next;
    } while (result != pos || pos == NULL);

    if (result == NULL){
        printf("Symbol %s is not defined\n", symbolname);
    }

    return result->token;
}

struct TokenEntry* create_tokenentry(const char* name, struct Token* token){
    // Malloc
    struct TokenEntry* tokenentry = malloc(sizeof(struct TokenEntry));
    // Name
    tokenentry->name = malloc(strlen(name));
    strcpy(tokenentry->name, name);
    // Value
    tokenentry->token = token;
    // Next
    tokenentry->next = NULL;

    return tokenentry;
}

struct Token* add_token_to_table(const char* name, struct Token* token, struct TokenTable* tokentable){
    // Create token
    struct TokenEntry* tokenentry = create_tokenentry(name, token);
    // Insert token
    int hash = hashgen(name);
    tokenentry->next = tokentable->table[hash];
    tokentable->table[hash] = tokenentry;

    return tokenentry->token;
}

int hashgen(const char* symbolname){
    int sum = 0;
    for (int i=0; (i<5 && symbolname[i] != '\0'); i++)
        sum += symbolname[i];

    return sum % TABLESIZE;
}
