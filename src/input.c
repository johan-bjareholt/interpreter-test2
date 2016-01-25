#define _GNU_SOURCE // Adds getline
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "input.h"

bool interactive;
FILE* fp;
static char* sourcename = NULL;
static int linenr = -1;

static char* current_line = NULL;

static void input_set_sourcename(const char* newname){
    if (sourcename != NULL)
        free(sourcename);
    sourcename = malloc(sizeof(newname));
    strcpy(sourcename, newname);
}

void input_set_interactive(){
    interactive = true;
    input_set_sourcename("i");
    fp = stdin;
}

bool input_set_file(const char* filename){
    interactive = false;
    printf("Opening file %s\n", filename);
    input_set_sourcename(filename);
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
    if (current_line == NULL){
        size_t len = 0;
        int read;

        if (interactive == true)
            printf("> ");

        read = getline(&current_line, &len, fp);
        linenr++;

        if (read == 0)
            return input_getline();
        else if (read == -1)
            return NULL;
        else {
            //printf("Retrieved line of length %d :\n", read);
            //printf("%s", current_line);
        }
    }
    return current_line;
}

void input_consume_line(){
    if (current_line != NULL){
        free(current_line);
        current_line = NULL;
    }
}

int input_line_nr(){
    return linenr;
}

const char* input_get_name(){
    if (sourcename == NULL)
        input_set_sourcename("Unknown");
    return sourcename;
}
