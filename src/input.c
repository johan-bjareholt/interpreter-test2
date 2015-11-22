#define _GNU_SOURCE // Adds getline
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "input.h"

bool interactive;
FILE* fp;

void input_set_interactive(){
    interactive = true;
    fp = stdin;
}

bool input_set_file(const char* filename){
    interactive = false;
    printf("Opening file %s\n", filename);
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Couldn't open file %s\n", filename);
        return false;
    }
    return true;
}

void input_close(){
    if (interactive == false)
        fclose(fp);
}

char* input_getline(){
    char* line = NULL;
    size_t len = 0;
    int read;

    if (interactive == true)
        printf("> ");

    read = getline(&line, &len, fp);

    if (read == 0)
        return input_getline();
    else if (read == -1)
        return NULL;
    else {
        printf("Retrieved line of length %d :\n", read);
        printf("%s", line);
    }
    return line;
}
