#include "sc_memory.h"

static int memory[MEMORY_SIZE];

/*
int sc_memoryInit (void) – инициализирует оперативную
память Simple Computer, задавая всем её ячейкам нулевые значения
*/

int sc_memoryInit(void){
    for (int i = 0; i < MEMORY_SIZE; i++){
        memory[i] = 0; // заполняю ячейки нулями
    }
    return 0;
}