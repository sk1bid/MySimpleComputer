#include "sc_reg.h"

extern int accumulator;

int sc_accumulatorInit(void)
{
    accumulator = 0;
    return 0;
}