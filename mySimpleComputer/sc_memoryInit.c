#include "../include/mySimpleComputer.h"
#include "sc_memory.h"

extern int memory[MEMORY_SIZE];

int sc_memoryInit(void)
{
    for (int i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = 0;
    }
    sc_cacheInit();
    return 0;
}