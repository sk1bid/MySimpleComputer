#include "sc_reg.h"

extern int accumulator;

int sc_accumulatorSet(int value){
    if (value > 16383 || value < -16384){
        return -1; // недопустимое значение
    }

    accumulator = value;
    return 0;
}