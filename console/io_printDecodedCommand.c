#include "../include/mySimpleComputer.h"
#include "io.h"
#include "stdio.h"
/*
void printDecodedCommand (int value) – выводит переданное значение в десятичной
системе счисления, в восьмеричной системе счисления, в шестнадцатиричной системе
счисления и в двоичной системе счисления.
*/

void io_printDecodedCommand(int value)
{
    printf("Десятичная: %d, Восьмеричная: %o, Шестнадцатеричная: %X, "
           "Двоичная: ",
           value,
           value,
           value);
    for (int i = 14; i >= 0; i--) { // выводим двоичное по битам
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}