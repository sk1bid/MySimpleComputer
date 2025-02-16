#include "stdio.h"
#include "io.h"
#include "../include/mySimpleComputer.h"

/*
void printAccumulator (void) – выводит значение аккумулятора;
*/

void io_printAccumulator(void){
    int value;
    sc_accumulatorGet(&value);
    printf("Аккумулятор: %d (0x%X)\n", value, value); // выводим значение
}