#include "../include/mySimpleComputer.h"
#include "../myTerm/myTerm.h"
#include <stdio.h>

// Вывод декодированной команды
void io_printDecodedCommand(int value)
{
    mt_gotoXY(2, 17);
    printf("dec: %05d | oct: %05o | hex: %04X | bin: ", value, value, value);
    for (int i = 15; i >= 0; i--) { // Исправлено с 15-1 на 15
        putchar((value >> i) & 1 ? '1' : '0');
    }
    fflush(stdout);
}