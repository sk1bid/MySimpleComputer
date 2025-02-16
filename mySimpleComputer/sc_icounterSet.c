#include "sc_reg.h"
#include "sc_memory.h"

extern int instructionCounter;

int sc_icounterSet(int value){
    if (value < 0 || value >= MEMORY_SIZE){
        return -1;
    }

    instructionCounter = value;
    return 0;
}