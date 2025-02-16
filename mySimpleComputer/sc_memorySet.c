#include "sc_memory.h"

extern int memory[MEMORY_SIZE];

/*
int sc_memorySet (int address, int value) – задает
значение указанной ячейки памяти как value. Если адрес выходит за
допустимые границы или value не соответствует допустимому диапазону
значений, то функция возвращает -1, иначе завершается
корректно и возвращает 0;
*/

int sc_memorySet(int address, int value){

    if (address < 0 || address >= MEMORY_SIZE){
        return -1; // адрес выходит за допустимые границы
    }
    
    if (value > 16383 || value < -16384){
        return -1; /*value не соответсвует допустимому диапазону значений
        15 бит*/
    }
    memory[address] = value;
    return 0;
}
