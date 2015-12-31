#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "datatypes.h"
#include "variables.h"

#define TABLESIZE 100

/*
    Local definitions
*/

// Node stuff
struct Node {
    struct Variable* variable;
    struct Node* next;
};
static struct Node* table[TABLESIZE];
static struct Node* get_node(const char* name);
static void delete_node(struct Node* node);
// Hash
static int generate_hash(const char* string);

/*
    Global declarations
*/

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

struct Variable* create_variable_int(const char* name, int value){
    struct Variable* var = create_variable_empty(name, TYPE_INT);
    var->value = malloc(sizeof(int));
    memcpy(var->value, &value, sizeof(int));
    return var;
}

struct Variable* create_variable_str(const char* name, const char* value){
    struct Variable* var = create_variable_empty(name, TYPE_STR);
    var->value = malloc((strlen(value)+1)*sizeof(char));
    strcpy(var->value, value);
    return var;
}

struct Variable* create_variable_func(const char* name, const char* value){
    struct Variable* var = create_variable_empty(name, TYPE_FUNC);
    var->value = malloc((strlen(value)+1)*sizeof(char));
    strcpy(var->value, value);
    return var;
}

bool add_variable(struct Variable* var){
    bool success = false;

    struct Node* node;

    node = get_node(var->name);
    if (node != NULL){
        if (node->variable->datatype == var->datatype){
            free(node->variable);
            node->variable = var;
            success = true;
        }
        else {
            printf("Variable %s of type %s cannot be assigned to value of type %s\n",
                var->name, DataTypeNames[node->variable->datatype], DataTypeNames[var->datatype]);
            free(var);
            success = false;
        }
    }
    else {
        int hash = generate_hash(var->name);
        node = malloc(sizeof(struct Node));
        node->variable = var;

        if (table[hash] != NULL)
            node->next = table[hash];
        else
            node->next = NULL;

        table[hash] = node;
        success = true;
    }
    return success;
}

struct Variable* get_variable(const char* name){
    struct Variable* var = NULL;
    struct Node* node = get_node(name);
    if (node != NULL)
        var = node->variable;
    return var;
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

/*
    Local declarations
*/

// Hashing
static int generate_hash(const char* string){
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

/*
    Local node definitions
*/

static struct Node* get_node(const char* name){
    int hash = generate_hash(name);
    struct Node* node = table[hash];

    bool hit = false;
    while (node != NULL && hit == false){
        if (strcmp(name, node->variable->name) == 0)
            hit = true;
        else
            node = node->next;
    }
    return node;
}

static void delete_node(struct Node* node){
    free(node->variable->name);
    free(node->variable->value);
    free(node->variable);
    free(node);
}
