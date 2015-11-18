
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>

#include "datatypes.h"

struct Command {
    char* string;
    struct Command* next;
};

struct Command* first_command = NULL;
struct Command* last_command = NULL;

void add_command(char* string){
    struct Command* command = malloc(sizeof(struct Command));
    command->string = malloc(strlen(string)*sizeof(char));
    strcpy(command->string, string);
    if (first_command != NULL)
        last_command->next = command;
    else
        first_command = command;
    last_command = command;
}

void getline(){
    char line[100];

    printf("> ");
    fgets(line, 100, stdin);

    int len = strlen(line);
    if (len >= 100)
        printf("Line is too long\n");
    if (len > 0)
        add_command(line);
}

struct Section {
    int datatype;
    char* string;
    struct Section* prev;
    struct Section* next;
};

void* create_section(const int datatype, const char* string){
    struct Section* section = malloc(sizeof(struct Section));
    section->datatype = datatype;

    int len = strlen(string);
    section->string = malloc(len*sizeof(char));
    strcpy(section->string, string);

    section->prev = NULL;
    section->next = NULL;
    return section;
}

void* parser(){
    printf("# Parsing...\n");

    struct Command* command = last_command;
    char* string = command->string;
    int len = strlen(string);

    struct Section* section = NULL;
    struct Section* last_section = NULL;

    int datatype = TYPE_NONE;
    int datatype_starti = 0;
    int datatype_endi = 0;
    int prev_datatype;
    for (int i=0; i<=len; i++){
        // A-Z
        if (string[i] >= 'A' && string[i] <= 'Z'){
            datatype = TYPE_VAR;
        }
        // a-z
        else if (string[i] >= 'a' && string[i] <= 'z'){
            datatype = TYPE_VAR;
        }
        // 0-1
        else if (string[i] >= '0' && string[i] <= '9'){
            if (prev_datatype != TYPE_VAR)
                datatype = TYPE_INT;
        }
        else {
            switch (string[i]) {
                case ' ': // blank
                case '\0': // end of string
                case '\n':
                    datatype = TYPE_NONE;
                    break;
                case '=': // Assign
                    datatype = TYPE_ASSIGNMENT;
                    if (prev_datatype == datatype){
                        datatype = TYPE_COMPARISON;
                        prev_datatype = TYPE_COMPARISON;
                    }
                    break;
                case '+': // +
                    datatype = TYPE_PLUS;
                    break;
                case '-': // -
                    datatype = TYPE_MINUS;
                    break;
                case '/': // /
                    datatype = TYPE_DIVISION;
                    break;
                case '%': // %
                    datatype = TYPE_MODULO;
                    break;
                default:
                    printf("Unknown token %c\n", string[i]);
                    break;
            }
        }
        if (datatype != prev_datatype){
            datatype_endi = i-1;
            if (datatype_endi >= 0){
                if (prev_datatype != TYPE_NONE){
                    printf("%s ", DataTypeNames[prev_datatype]);

                    int len = datatype_endi-datatype_starti+2;
                    char* datastring = malloc(len*sizeof(char));
                    strncpy(datastring, string+datatype_starti, len-1);
                    datastring[len-1] = '\0';

                    printf("%s", datastring);
                    printf("(%d,%d,%d), ", datatype_starti, datatype_endi, len-1);

                    section = (struct Section*) create_section(prev_datatype, datastring);
                    if (last_section == NULL)
                        last_section = section;
                    else {
                        last_section->next = section;
                        section->prev = last_section;
                        last_section = section;
                    }
                    printf("\n");
                }
            }
            datatype_starti = i;
        }
        prev_datatype = datatype;
    }
    return last_section;
}

enum EvalStatus {
    EStatus_OK,
    EStatus_Done,
    EStatus_Error,
};

void eval(struct Command* command, struct Section* section){
    printf("# Evaluating...\n");
    int status = EStatus_OK;
    while(status == EStatus_OK){
        if (section == NULL){
            status = EStatus_Done;
        }
        else {
            switch (section->datatype){
                case TYPE_VAR:
                case TYPE_INT:
                    break;
                case TYPE_MINUS:
                case TYPE_PLUS:
                    if (section->prev == NULL){
                        printf("%s has nothing prior to it to calculate\n", section->string);
                        status = EStatus_Error;
                    }
                    else if (section->next == NULL){
                        printf("%s has nothing after it to calculate\n", section->string);
                        status = EStatus_Error;
                    }
                    else {
                        if (section->prev->datatype != TYPE_INT){
                            printf("%s cannot calculate %s of the type %s\n", section->string, section->prev->string, DataTypeNames[section->prev->datatype]);
                            status = EStatus_Error;
                        }

                        else if (section->next->datatype != TYPE_INT){
                            printf("%s cannot calculate %s of the type %s\n", section->string, section->next->string, DataTypeNames[section->next->datatype]);
                            status = EStatus_Error;
                        }
                        else {
                            int prevVal = atoi(section->prev->string);
                            int nextVal = atoi(section->next->string);
                            int val=0;
                            if (section->datatype == TYPE_PLUS)
                                val = prevVal + nextVal;
                            else if (section->datatype == TYPE_MINUS)
                                val = prevVal - nextVal;
                            else if (section->datatype == TYPE_DIVISION)
                                val = prevVal / nextVal;
                            else if (section->datatype == TYPE_MODULO)
                                val = prevVal % nextVal;
                            else { // This isn't supposed to happen
                                printf("Something went incredibly wrong with the evaluation of arithmetic\n");
                                status = EStatus_Error;
                            }
                            printf("%s%s%s=%d\n", section->prev->string, DataTypeNames[section->datatype],section->next->string, val);
                            //create_section()
                        }
                    }
                    break;
                case TYPE_ASSIGNMENT:
                    if (section->prev == NULL){
                        printf("There is nothing prior to %s to assign a value\n", section->string);
                        status = EStatus_Error;
                    }
                    else if (section->next == NULL){
                        printf("There is nothing after %s to use for the assignment\n", section->string);
                        status = EStatus_Error;
                    }
                    else {
                        if (section->prev->datatype != TYPE_VAR){
                            printf("%s is not a variable and can therefore not be assigned\n", section->string);
                            status = EStatus_Error;
                        }
                        else if (section->next->datatype != TYPE_INT ||
                                 section->next->datatype != TYPE_VAR){
                            printf("%s is of type %s and cannot be assigned to %s of type %s\n",
                                    section->prev->string, DataTypeNames[section->prev->datatype],
                                    section->next->string, DataTypeNames[section->next->datatype]);
                            status = EStatus_Error;
                        }
                        else {
                            printf("Assigning %s to %s\n",
                                section->next->string, section->prev->string);
                        }
                    }
                    break;
                case TYPE_COMPARISON:
                    if (section->prev == NULL){
                        printf("%s has nothing prior to it to compare with\n", section->string);
                        status = EStatus_Error;
                    }
                    else if (section->next == NULL){
                        printf("%s has nothing after it to calculate with\n", section->string);
                        status = EStatus_Error;
                    }
                    else {
                        if (section->prev->datatype != section->next->datatype){
                            printf("Cannot compare %s and %s since they are of different types\n",
                                    section->prev->string, section->next->string);
                            status = EStatus_Error;
                        }
                        else {
                            if (section->prev->datatype == TYPE_INT){
                                int prevVal = atoi(section->prev->string);
                                int nextVal = atoi(section->next->string);
                                printf("%d\n", prevVal==nextVal);
                            }
                            else {
                                printf("%s and %s is of type %s, which doesn't have a implementation for comparison\n",
                                section->prev->string, section->next->string, DataTypeNames[section->prev->datatype]);
                                status = EStatus_Error;
                            }
                        }
                    }
                    break;
            }
            section = section->prev;
        }
    }
    printf("\n");
}

void cleanup();

int main(){
    signal(SIGINT, cleanup);

    struct Section* section;

    while (true){
        getline();
        section = parser();
        eval(last_command, section);
    }
    return 0;
}

void cleanup(){
    struct Command* command = first_command;
    struct Command* old_command;
    printf("\nOutput:\n");
    while ( command != NULL ){
        printf("%s", command->string);
        free(command->string);
        old_command = command;
        command = command->next;
        free(old_command);
    }
    exit(0);
}
