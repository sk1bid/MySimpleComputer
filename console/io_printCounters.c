#include <stdio.h>          // Для стандартного вывода (printf)
#include "io.h"             // Заголовочный файл модуля io
#include "../include/mySimpleComputer.h" // Общий заголовочный файл библиотеки

/*
void printCounters (void) – выводит значение счетчика
команд.*/

void io_printCounters(void) {
    int icounter;
    sc_icounterGet(&icounter); // Получаем значение счетчика команд
    printf("Счетчик команд: %d (0x%X)\n", icounter, icounter);
}