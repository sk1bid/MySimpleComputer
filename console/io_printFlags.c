#include "../include/mySimpleComputer.h"
#include "io.h"
#include "stdio.h"

/*
void printFlags (void) – выводит значения флагов. Формат
должен соответствовать заданию (выводятся либо _, либо буквы в
заданной последовательности);
*/

void io_printFlags(void)
{
    int overflow, divisionByZero, memoryError, ignoreClock, invalidCommand;

    // Получаем флаги
    sc_regGet(FLAG_OVERFLOW, &overflow);
    sc_regGet(FLAG_DIVISION_BY_ZERO, &divisionByZero);
    sc_regGet(FLAG_MEMORY_ERROR, &memoryError);
    sc_regGet(FLAG_IGNORE_CLOCK_TICKS, &ignoreClock);
    sc_regGet(FLAG_INVALID_COMMAND, &invalidCommand);

    printf("Флаги: %c%c%c%c%c\n",
           overflow ? 'P' : '_',       // P - Переполнение
           divisionByZero ? '0' : '_', // 0 - Деление на 0
           memoryError ? 'M' : '_',    // M - Ошибка памяти
           ignoreClock ? 'T' : '_', // T - Игнорирование тактов
           invalidCommand ? 'E' : '_' // E - Неверная команда
    );
}