#include "../include/mySimpleComputer.h"
#include "sc_memory.h"
#include "stdio.h"

extern int memory[MEMORY_SIZE];

int sc_memoryLoad(char* filename)
{
    if (filename == NULL) {
        return -1;
    }

    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return -1;
    }

    int temp_memory[MEMORY_SIZE];
    size_t data = fread(temp_memory, sizeof(int), MEMORY_SIZE, file);
    fclose(file);

    if (data != MEMORY_SIZE) {
        return -1;
    }

    for (int i = 0; i < MEMORY_SIZE; ++i) {
        memory[i] = temp_memory[i];
    }

    sc_cacheInit();

    return 0;
}