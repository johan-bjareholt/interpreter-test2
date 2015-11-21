
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

#include "input.h"
#include "datatypes.h"
#include "parser.h"
#include "eval.h"

void cleanup();

int main(){
    signal(SIGINT, cleanup);

    char buf[100];
    char* bufptr = buf;
    struct Section* section;

    while (true){
        getline(buf);
        section = parser(bufptr);
        eval(section);
    }
    return 0;
}

void cleanup(){
    exit(0);
}
