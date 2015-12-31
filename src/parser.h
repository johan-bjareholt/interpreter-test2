#ifndef PARSER_H
#define PARSER_H

#include "datatypes.h"
#include "section.h"

struct Section* parser(char* string);

char* get_subprocess_section();

#endif
