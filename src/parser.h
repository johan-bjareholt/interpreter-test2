#ifndef PARSER_H
#define PARSER_H

#include "datatypes.h"

struct Section {
    int datatype;
    char* string;
    char* substring;
    struct Section* prev;
    struct Section* next;
};

struct Section* create_section(const int datatype, const char* string);
void free_section(struct Section* section);

struct Section* parser(char* string);

#endif
