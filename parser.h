#ifndef PARSER_H
#define PARSER_H

#include "datatypes.h"

struct Section {
    int datatype;
    char* string;
    struct Section* prev;
    struct Section* next;
};

void* create_section(const int datatype, const char* string);

void* parser(char* string);

#endif
