
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "datatypes.h"
#include "input.h"
#include "parser.h"
#include "eval.h"

char* getmoreinput(char* oldline){
    char* line = input_getline();
    if (strlen(line) < 2){
        return getmoreinput(oldline);
    }
    else {
        char* newline = malloc(strlen(oldline)+strlen(line)-1);
        strcpy(newline, oldline);
        strcpy(newline+strlen(oldline)-1, line);
        return newline;
    }
}

char* insertline(char* oldline, char* insertline, int pos){
    char* newline = malloc(strlen(oldline)+strlen(insertline)+1);
    strncpy(newline, oldline, pos);
    strcpy(newline+pos, insertline);
    strcpy(newline+strlen(newline)-1, oldline+pos);
    return newline;
}

struct Section* parser(char* string){
    printf("# Parsing...\n");

    int len = strlen(string);
    char* newstring = malloc(len*sizeof(char)+1);
    strcpy(newstring, string);
    string = newstring;

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
        else if (datatype == TYPE_FUNCCALL){
            datatype = TYPE_NONE;
            FUNC_LOOP_START:
            while ( string[i] != ')' && i < len)
                i++;
            if (string[i] != ')'){
                printf("Need more\n");
                char* oldstring = string;
                string = getmoreinput(string);
                free(oldstring);
                len = strlen(string);
                i--;
                printf("%i,%s\n", len, string);
                goto FUNC_LOOP_START;
            }
            datatype_endi = i;
            i--;
        }
        else if (datatype == TYPE_PREPROCESS){
            datatype = TYPE_NONE;
            PREPROCESS_LOOP_START:
            while ( string[i] != ')' && i < len)
                i++;
            if (string[i] != ')'){
                printf("Need more\n");
                string = getmoreinput(string);
                len = strlen(string);
                i--;
                printf("%i,%s\n", len, string);
                goto PREPROCESS_LOOP_START;
            }
            datatype_endi = i;
            i--;
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
                        prev_datatype = TYPE_FUNCCALL;
                        datatype = TYPE_FUNCCALL;
                    }
                    // Preprocess start
                    else {
                        prev_datatype = TYPE_PREPROCESS;
                        datatype = TYPE_PREPROCESS;
                    }
                    break;
                case ')': // Function/Preprocess end
                    if (last_section->datatype == TYPE_FUNCCALL){
                        if (string[i+1] == ':'){
                            i++;
                            // Subprocess/Functiondef
                            printf("Preparing funcdef\n");
                            datatype = TYPE_NONE;
                            last_section->datatype = TYPE_FUNCDEF;
                            char* substring = get_subprocess_section();
                            printf("%s\n", substring);
                            prev_datatype = TYPE_FUNCCONTENT;
                            prev_datatype = TYPE_NONE;
                            section = create_section(TYPE_FUNCCONTENT, substring);
                            last_section->next = section;
                            section->prev = last_section;
                            last_section = section;
                        }
                        else {

                        }
                    }
                    else {
                        datatype = TYPE_NONE;
                    }
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
                    // Preprocess
                    if (prev_datatype == TYPE_PREPROCESS){
                        datastring++;
                        if (i < len-3)
                            datastring[len-3] = '\0';
                        printf("%s\n", datastring);
                        section = parser(datastring);
                        section = eval(section);
                        if (section == NULL){
                            printf("Something with the parsing of a preprocess went wrong %s\n", datastring);
                            exit(-1);
                        }
                        datastring--; // Needed for free(datastring) to not screw up
                        //datastring = NULL;
                        printf("# Back to parsing\n");
                    }
                    // Default
                    else {
                        section = create_section(prev_datatype, datastring);
                    }
                    free(datastring);

                    // Add parsed section and prepare for next
                    if (last_section == NULL)
                        last_section = section;
                    else {
                        last_section->next = section;
                        section->prev = last_section;
                        last_section = section;
                    }
                }
            }
            datatype_starti = i;
        }
        prev_datatype = datatype;
    }
    free(string);
    return last_section;
}


char* get_subprocess_section(){
    char* line = input_getline();
    if (line == NULL){
        printf("I'm noping out of here, bye!\n");
        exit(-1);
    }

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
    char* substring = malloc(newlen);
    strncpy(substring, line+space_count, newlen);
    free(line);

    int spacing = 0;
    do {
        line = input_getline();
        // Count spaces
        spacing = 0;
        if (line == NULL){
            printf("End of input\n");
        }
        else {
            for (int i=0; strlen(line) > i && line[i] == ' ' && line[i] != '\0'; i++)
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

                puts(substring);

                line -= spacing;
                free(old_substring);
            }
            else if (strlen(line) > 1){
                printf("Invalid spacing, exiting\n");
                exit(-1);
            }
        }
        free(line);
    } while (spacing == space_count);
    return substring;
}
