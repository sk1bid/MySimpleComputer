#include "sc_reg.h"
#include "stdio.h"

extern int accumulator;

int sc_accumulatorGet(int *value){
    if (value == NULL){
        return -1;
    }

    *value = accumulator;
    return 0;
}