#include "../include/mySimpleComputer.h"
#include "sc_memory.h"
#include "stdio.h"

extern int memory[MEMORY_SIZE];

int sc_memorySave(char* filename)
{
    if (filename == NULL) {
        return -1;
    }

    // Сбрасываем кэш перед сохранением
    sc_cacheFlush();

    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        return -1;
    }

    size_t data = fwrite(memory, sizeof(int), MEMORY_SIZE, file);
    fclose(file);

    if (data != MEMORY_SIZE) {
        return -1;
    }
    return 0;
}