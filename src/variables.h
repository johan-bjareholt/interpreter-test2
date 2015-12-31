#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdbool.h>

struct Variable {
    char* name;
    int datatype;
    void* value;
};

struct Variable* create_variable_int(const char* name, int value);
struct Variable* create_variable_str(const char* name, const char* value);
struct Variable* create_variable_func(const char* name, const char* value);

bool add_variable(struct Variable* var);
struct Node* get_node(const char* name);
struct Variable* get_variable(const char* name);
void purge_all_variables();

#endif
