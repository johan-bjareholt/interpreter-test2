#ifndef SECTION_H
#define SECTION_H

#include <stdbool.h>

#include "parser.h"

struct Section {
    int datatype;
    char* string;
    char* substring;
    struct Section* prev;
    struct Section* next;
};

struct Section* create_section(const int datatype, const char* string);
void free_section(struct Section* section);
bool section_add_substring(struct Section* section, const char* substring);

#endif
