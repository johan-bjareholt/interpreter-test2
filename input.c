#include <stdio.h>
#include <string.h>

#include "input.h"

void getline(char* line){
    int len;

    getlinestart:

    printf("> ");
    fgets(line, 100, stdin);

    len = strlen(line);
    if (len == 0)
        goto getlinestart;
    if (len >= 100){
        printf("Line is too long\n");
        goto getlinestart;
    }
}
