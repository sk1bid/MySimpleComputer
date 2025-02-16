#include "sc_reg.h"
#include "stdio.h"
extern int instructionCounter;

int sc_icounterGet(int *value){
    if (value == NULL){
        return -1;
    }

    *value = instructionCounter;
    return 0;
}