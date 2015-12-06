
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "datatypes.h"
#include "input.h"
#include "parser.h"
#include "eval.h"

struct Section* create_section(const int datatype, const char* string){
    struct Section* section = malloc(sizeof(struct Section));
    if (section == NULL)
        goto SECTION_MEMERROR;

    section->datatype = datatype;

    int len = strlen(string);
    section->string = NULL;
    section->string = malloc((len+1)*sizeof(char));
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
        if (section->string != NULL)
            free(section->string);
        free(section);
    }
}

struct Section* parser(char* string){
    printf("# Parsing...\n");

    int len = strlen(string);

    struct Section* section = NULL;
    struct Section* last_section = NULL;

    int datatype = TYPE_NONE;
    int datatype_starti = 0;
    int datatype_endi = 0;
    int prev_datatype = TYPE_NONE;
    for (int i=0; i<=len; i++){
        datatype_endi = i-1;
        // Types with escape chars
        if (datatype == TYPE_STR){
            while ( string[i] != '"' && i < len)
                i++;
            if (string[i] != '"'){
                printf("No end \" for string! Exiting\n");
                exit(-1);
            }
            datatype = TYPE_NONE;
            datatype_starti++;
            datatype_endi = i-1;
        }
        else if (datatype == TYPE_FUNC){
            datatype = TYPE_NONE;
            while ( string[i] != ')' && i < len)
                i++;
            if (string[i] != ')'){
                printf("No end ) for function! Exiting\n");
                exit(-1);
            }
            datatype_endi = i;
        }
        else if (datatype == TYPE_PREPROCESS){
            datatype = TYPE_NONE;
            while ( string[i] != ')' && i < len)
                i++;
            if (string[i] != ')'){
                printf("No end ) for preprocessor! Exiting\n");
                exit(-1);
            }
            datatype_endi = i;
        }
        // Alphanumerics
        // A-Z
        else if (string[i] >= 'A' && string[i] <= 'Z'){
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
        // Symbols
        else {
            switch (string[i]) {
                case ' ': // blank
                case '\0': // end of string
                case '\n': // newline
                    datatype = TYPE_NONE;
                    break;
                case '"':
                    // Start of string
                    if (datatype != TYPE_STR)
                        datatype = TYPE_STR;
                    // End of string
                    else
                        datatype = TYPE_NONE;
                    break;
                case '(':
                    // Function start
                    if (prev_datatype == TYPE_VAR){
                        prev_datatype = TYPE_FUNC;
                        datatype = TYPE_FUNC;
                    }
                    // Preprocess start
                    else {
                        prev_datatype = TYPE_PREPROCESS;
                        datatype = TYPE_PREPROCESS;
                    }
                    break;
                case ')': // Function/Preprocess end
                    datatype = TYPE_NONE;
                    break;
                case ':':
                    datatype = TYPE_SUBPROCESS;
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
                case '*':
                    datatype = TYPE_MULTIPLICATION;
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
            if (datatype_endi >= 0){
                if (prev_datatype != TYPE_NONE){
                    printf("%s ", DataTypeNames[prev_datatype]);

                    int len = datatype_endi-datatype_starti+2;
                    char* datastring = malloc(len*sizeof(char));
                    strncpy(datastring, string+datatype_starti, len-1);
                    datastring[len-1] = '\0';

                    printf("%s", datastring);
                    printf("(%d,%d,%d), ", datatype_starti, datatype_endi, len-1);

                    // Subprocess
                    if (prev_datatype == TYPE_SUBPROCESS){
                        printf("Subprocess is not properly implelemted yet\n");
                        char* substring;
                        char* line = input_getline();
                        if (line == NULL){
                            printf("I'm noping out of here, bye!\n");
                        }
                        else {
                            // Count spaces
                            int space_count = 0;
                            for (int i=0; line[i] == ' '; i++)
                                space_count++;
                            printf("Spacing: %d\n", space_count);
                            if (space_count < 1){
                                printf("No spacing after subprocess, exiting\n");
                                exit(-1);
                            }
                            // Move line to substring and remove spacing
                            int newlen = strlen(line)-space_count+1;
                            substring = malloc(newlen);
                            strncpy(substring, line+space_count, newlen);
                            free(line);

                            int spacing = 0;
                            do {
                                line = input_getline();
                                // Count spaces
                                spacing = 0;
                                for (int i=0; line[i] == ' ' && line[i] != '\0'; i++)
                                    spacing++;
                                if (spacing == space_count){
                                    // Append line to substring
                                    int oldlen = strlen(substring);
                                    char* old_substring = substring;
                                    line += spacing;
                                    int newlen = strlen(line);
                                    int totlen = oldlen + newlen + 2;
                                    substring = malloc(totlen);
                                    strncpy(substring, old_substring, oldlen);
                                    substring[oldlen-1] = '\n';
                                    strncpy(substring+oldlen, line, newlen);
                                    substring[totlen-1] = '\0';

                                    printf("%s\n", substring);

                                    line -= spacing;
                                    free(line);
                                    free(old_substring);
                                }
                                else if (strlen(line) > 1){
                                    printf("Invalid spacing, exiting\n");
                                    exit(-1);
                                }
                            } while (spacing == space_count);
                            section = create_section(prev_datatype, datastring);
                            section_add_substring(section, substring);
                        }
                    }
                    // Preprocess
                    if (prev_datatype == TYPE_PREPROCESS){
                        datastring++;
                        if (i< len-3)
                            datastring[len-3] = '\0';
                        printf("%s\n", datastring);
                        section = parser(datastring);
                        section = eval(section);
                        if (section == NULL){
                            printf("Something with the parsing of a preprocess went wrong %s\n", datastring);
                            exit(-1);
                        }
                        datastring--; // Needed for free(datastring) to not screw up
                        printf("# Back to parsing\n");
                    }
                    // Default
                    else {
                        section = create_section(prev_datatype, datastring);
                    }

                    // Add parsed section and prepare for next
                    if (last_section == NULL)
                        last_section = section;
                    else {
                        last_section->next = section;
                        section->prev = last_section;
                        last_section = section;
                    }
                    printf("\n");
                    free(datastring);
                }
            }
            datatype_starti = i;
        }
        prev_datatype = datatype;
    }
    return last_section;
}
