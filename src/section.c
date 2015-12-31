#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "section.h"

struct Section* create_section(const int datatype, const char* string){
    struct Section* section = malloc(sizeof(struct Section));
    if (section == NULL)
        goto SECTION_MEMERROR;

    section->datatype = datatype;

    int len = strlen(string);
    section->string = NULL;
    section->string = malloc((len+1)*sizeof(char));
    section->substring = NULL;
    if (section->string == NULL)
        goto SECTION_MEMERROR;
    strcpy(section->string, string);

    section->prev = NULL;
    section->next = NULL;
    return section;

    SECTION_MEMERROR:
    printf("ERROR: Couldn't allocate memory for a new section\n");
    exit(-1);
}

bool section_add_substring(struct Section* section, const char* substring){
    if (section == NULL || substring == NULL)
        return false;

    section->substring = malloc(strlen(substring));
    strcpy(section->substring, substring);

    printf("Added a substring to a section\n");

    return true;
}

void free_section(struct Section* section){
    if (section != NULL){
        if (section->string != NULL){
            free(section->string);
            free(section->substring);
        }
        free(section);
    }
}
