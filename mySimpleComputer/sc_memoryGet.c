#include "sc_memory.h"
#include "stdio.h" // для NULL

extern int memory[MEMORY_SIZE];

/*
int sc_memoryGet (int address, int * value) – возвращает значение
указанной ячейки памяти в value. Если адрес выходит за допустимые
границы или передан неверный указатель на
value, то функция завершается со статусом -1. В случае успешного
выполнения функции она завершается со статусом 0.
*/

int sc_memoryGet(int address, int *value){
    if (address < 0 || address > MEMORY_SIZE){
        return -1; // адрес вышел за границы
    }

    if (value == NULL) {
        return -1; // неверный указатель
    }

    *value = memory[address];
    return 0;
}