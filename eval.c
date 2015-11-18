#include <stdio.h>
#include <stdlib.h>

#include "eval.h"
#include "datatypes.h"
#include "parser.h"

enum EvalStatus {
    EStatus_OK,
    EStatus_Done,
    EStatus_Error,
};

void eval(struct Section* section){
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
}
