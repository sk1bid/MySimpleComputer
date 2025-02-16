#include "sc_memory.h"
#include "sc_reg.h"

extern int instructionCounter;

int sc_icounterSet(int value)
{
    if (value < 0 || value >= MEMORY_SIZE) {
        return -1;
    }

    instructionCounter = value;
    return 0;
}