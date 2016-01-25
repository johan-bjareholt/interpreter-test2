
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>

#include "input.h"
#include "lexer.h"

void handleInterrupt();
void cleanup();

int parse_switches(const int argc, const char* argv[]);

/*
- input
- lexer
    - non-linear, keep track on all sets
- parser
    - Builds the AST
- halfway interpreter
- interpreter/compiler
*/

int main(const int argc, const char* argv[]){
    signal(SIGINT, handleInterrupt);

    if (parse_switches(argc, argv) < 0)
        return -1;

    // Interpret
    char* line;

    bool done = false;
    while (done == false)
    {
        line = input_getline();
        if (line != NULL){
            printf("%s", line);
            lex(line);
            input_consume_line();
        }
        else {
            done = true;
        }
    }

    cleanup();

    return EXIT_SUCCESS;
}

int parse_switches(const int argc, const char* argv[]){
    const char* filename = NULL;

    // Parse switches
    for (int argi=1; argi<argc; argi++){
        const char* arg = argv[argi];
        if (arg[0] == '-'){
            if (arg[1] == '-'){
                if (strcmp(arg+2, "in") == 0){
                    goto SWITCH_FILEIN;
                }
                else {
                    goto SWITCH_INVALID;
                }
            }
            else {
                switch (arg[1]){
                    case 'i':
                        SWITCH_FILEIN:
                        if (argi+1 >= argc){
                            printf("-i/--in switch needs an argument\n");
                            return -1;
                        }
                        argi++;
                        filename = argv[argi];
                        break;
                    default:
                        goto SWITCH_INVALID;
                }
            }
        }
        else {
            SWITCH_INVALID:
            printf("Invalid switch %s\n", arg);
            exit(-1);
        }
    }

    // Setup input
    if (filename == NULL)
        input_set_interactive();
    else {
        if (input_set_file(filename) == false)
            return -1;
    }

    return 0;
}

void handleInterrupt(){
    cleanup();
    exit(EXIT_SUCCESS);
}

void cleanup(){
    input_close();
}
