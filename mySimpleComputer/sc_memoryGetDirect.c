#include "../include/mySimpleComputer.h"
#include "sc_memory.h"
#include <stdio.h>

int sc_memoryGetDirect(int address, int* value)
{
    if (address < 0 || address >= MEMORY_SIZE) {
        return -1;
    }

    if (value == NULL) {
        return -1;
    }

    *value = memory[address];

    return 0;
}