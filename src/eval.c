#include <stdio.h>
#include <stdlib.h>

#include "eval.h"
#include "variables.h"
#include "datatypes.h"
#include "parser.h"

enum EvalStatus {
    EStatus_OK,
    EStatus_Done,
    EStatus_Error,
};

void eval(struct Section* section){
    printf("# Evaluating...\n");
    struct Section* prev_section = section;
    int status = EStatus_OK;
    while(status == EStatus_OK){
        if (section != NULL){
            switch (section->datatype){
                case TYPE_VAR:
                case TYPE_INT:
                    break;
                case TYPE_MINUS:
                case TYPE_PLUS:
                case TYPE_DIVISION:
                case TYPE_MULTIPLICATION:
                case TYPE_MODULO:
                    if (section->prev == NULL){
                        printf("%s has nothing prior to it to calculate\n", section->string);
                        status = EStatus_Error;
                    }
                    else if (section->next == NULL){
                        printf("%s has nothing after it to calculate\n", section->string);
                        status = EStatus_Error;
                    }
                    else {
                        if (section->prev->datatype != TYPE_INT && section->prev->datatype != TYPE_VAR){
                            printf("%s cannot calculate %s of the type %s\n", section->string, section->prev->string, DataTypeNames[section->prev->datatype]);
                            status = EStatus_Error;
                        }

                        else if (section->next->datatype != TYPE_INT && section->next->datatype != TYPE_VAR){
                            printf("%s cannot calculate %s of the type %s\n", section->string, section->next->string, DataTypeNames[section->next->datatype]);
                            status = EStatus_Error;
                        }
                        else {
                            int prevVal, nextVal;
                            int val = 0;

                            struct Variable* varptr;
                            switch (section->prev->datatype){
                                case TYPE_INT:
                                    prevVal = atoi(section->prev->string);
                                    break;
                                case TYPE_VAR:
                                    varptr = get_variable(section->prev->string);
                                    if (varptr != NULL){
                                        if (varptr->datatype == TYPE_INT){
                                            prevVal = *(int*)varptr->value;
                                        }
                                        else {
                                            printf("Variable %s isn't defined\n", varptr->name);
                                            return;
                                        }
                                    }
                                    break;
                                default:
                                    printf("This really shouldn't be happening. Arithmetic expression type check didn't catch incorrect type\n");
                                    return;
                                    break;
                            }
                            switch (section->next->datatype){
                                case TYPE_INT:
                                    nextVal = atoi(section->next->string);
                                    break;
                                case TYPE_VAR:
                                    varptr = get_variable(section->next->string);
                                    if (varptr != NULL){
                                        if (varptr->datatype == TYPE_INT){
                                            nextVal = *(int*)varptr->value;
                                        }
                                        else {
                                            printf("Variable %s isn't defined\n", varptr->name);
                                            return;
                                        }
                                    }
                                    break;
                                default:
                                    printf("This really shouldn't be happening. Arithmetic expression type check didn't catch incorrect type\n");
                                    return;
                                    break;
                            }

                            switch (section->datatype){
                                case TYPE_PLUS:
                                    val = prevVal + nextVal;
                                    break;
                                case TYPE_MINUS:
                                    val = prevVal - nextVal;
                                    break;
                                case TYPE_MULTIPLICATION:
                                    val = prevVal * nextVal;
                                    break;
                                case TYPE_DIVISION:
                                    if (nextVal == 0){
                                        printf("Division by zero\n");
                                        return;
                                    }
                                    val = prevVal / nextVal;
                                    break;
                                case TYPE_MODULO:
                                    val = prevVal % nextVal;
                                    break;
                                default:
                                    printf("Something went incredibly wrong with the evaluation of arithmetic\n");
                                    status = EStatus_Error;
                                    return;
                                    break;
                            }
                            printf("%s%s%s=%d\n",
                                section->prev->string,              // val
                                DataTypeNames[section->datatype],   // operand
                                section->next->string,              // val
                                val);                               // result

                            const int MAXLEN = 20;
                            char valstr[MAXLEN];
                            snprintf(valstr, MAXLEN, "%d", val);
                            struct Section* newsec = create_section(TYPE_INT, valstr);

                            if (section->next->next){
                                section->next->next->prev = newsec;
                                newsec->next = section->next->next;
                            }
                            if (section->prev->prev){
                                section->prev->prev->next = newsec;
                                newsec->prev = section->prev->prev;
                            }

                            free_section(section->next);
                            free_section(section->prev);
                            free_section(section);

                            section = newsec;
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
                        else if (section->next->datatype != TYPE_INT &&
                                 section->next->datatype != TYPE_STR &&
                                 section->next->datatype != TYPE_VAR){
                            printf("%s is of type %s and cannot be assigned to %s of type %s\n",
                                    section->prev->string, DataTypeNames[section->prev->datatype],
                                    section->next->string, DataTypeNames[section->next->datatype]);
                            status = EStatus_Error;
                        }
                        else {
                            printf("Assigning %s to %s\n",
                                section->next->string, section->prev->string);
                            if (section->next->datatype == TYPE_INT)
                                create_variable_int(section->prev->string, atoi(section->next->string));
                            else if (section->next->datatype == TYPE_STR){
                                create_variable_str(section->prev->string, section->next->string);
                            }
                            else if (section->next->datatype == TYPE_VAR){
                                printf("Var to var assignment is not yet implemented\n");
                                status = EStatus_Error;
                            }
                            struct Variable* var = get_variable(section->prev->string);
                            if (var != NULL){
                                if (var->datatype == TYPE_INT){
                                    printf("Variable %s has value %d\n",
                                        var->name, *(int*)var->value);
                                }
                                if (var->datatype == TYPE_STR){
                                    printf("Variable %s has value \"%s\"\n",
                                        var->name, (const char*)var->value);
                                }
                            }
                            else {
                                printf("This shouldn't be happening. Variable %s was created, but couldn't be accessed\n",
                                section->prev->string);
                            }
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
            prev_section = section;
            section = section->prev;
        }
        else {
            status = EStatus_Done;
        }
    }
    struct Section* temp_section;
    section = prev_section;
    while (section != NULL){
        temp_section = section;
        section = section->next;
        free_section(temp_section);
    }
}
