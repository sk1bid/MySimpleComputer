#include "../include/mySimpleComputer.h"
#include "../myTerm/myTerm.h"
#include <stdio.h>

void io_printDecodedCommand(int value)
{
    mt_gotoXY(2, 17);

    printf("dec: %0*u | oct: %0*o | hex: %0*X | bin: ",
           5,
           value,
           5,
           value,
           4,
           value);
    for (int i = 15 - 1; i >= 0; i--) {
        putchar((value >> i) & 1 ? '1' : '0');
    }
    fflush(stdout);
}
