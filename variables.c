#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "datatypes.h"

#define TABLESIZE 100

struct Variable {
    char* name;
    int datatype;
    void* value;
};

struct Node {
    struct Variable* variable;
    struct Node* next;
};

struct Node* table[TABLESIZE];

int generate_hash(const char* string){
    // Count name len
    int len = strlen(string);
    if (len>4)
        len = 4;
    // Generate hash
    int hash = 0;
    for (int ci=0; ci<len; ci++)
        hash += string[ci];
    hash %= TABLESIZE;
    // Return
    return hash;
}

void add_variable(struct Variable* var){
    int hash = generate_hash(var->name);

    struct Node* node = malloc(sizeof(struct Node));
    node->variable = var;

    // ToDo
    // Check if variable with the same name already exists

    if (table[hash] != NULL)
        node->next = table[hash];
    else
        node->next = NULL;

    table[hash] = node;
}

struct Variable* get_variable(const char* name){
    int hash = generate_hash(name);
    struct Node* node = table[hash];

    bool hit = false;
    while (node != NULL && hit == false){
        if (strcmp(name, node->variable->name) == 0)
            hit = true;
        else
            node = node->next;
    }

    struct Variable* result = NULL;
    if (hit == true)
        result = node->variable;
    return result;
}
