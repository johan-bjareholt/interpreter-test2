#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "datatypes.h"
#include "variables.h"

#define TABLESIZE 100

struct Node {
    struct Variable* variable;
    struct Node* next;
};

struct Node* table[TABLESIZE];

struct Variable* create_variable_empty(const char* name, int datatype){
    struct Variable* var = malloc(sizeof(struct Variable));

    int len = strlen(name);
    var->name = malloc((len+1)*sizeof(char));
    strcpy(var->name, name);

    var->datatype = datatype;

    var->value = NULL;

    add_variable(var);

    return var;
}

struct Variable* create_variable_int(const char* name, int datatype, int value){
    struct Variable* var = create_variable_empty(name, datatype);
    var->value = malloc(sizeof(int));
    memcpy(var->value, &value, sizeof(int));
    return var;
}

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

void delete_node(struct Node* node){
    free(node->variable->name);
    free(node->variable->value);
    free(node->variable);
    free(node);
}

void purge_all_variables(){
    for (int i=0; i<TABLESIZE; i++){
        struct Node* node_to_delete;
        while (table[i] != NULL){
            node_to_delete = table[i];
            table[i] = table[i]->next;
            delete_node(node_to_delete);
        }

    }
}
