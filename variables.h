#ifndef VARIABLES_H
#define VARIABLES_H

struct Variable {
    char* name;
    int datatype;
    void* value;
};

struct Variable* create_variable_int(const char* name, int datatype, int value);

void add_variable(struct Variable* var);
struct Variable* get_variable(const char* name);

#endif
