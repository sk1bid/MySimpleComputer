#include "../include/mySimpleComputer.h"
#include "../myTerm/myTerm.h"
#include <stdio.h>

void io_printCell(int address, enum colors fg, enum colors bg)
{
    mt_setfgcolor(fg);
    mt_setbgcolor(bg);

    int y = (address / 10) + 2;
    int x = (address % 10) * 6 + 1 + 1;
    mt_gotoXY(x, y);

    int value;
    sc_memoryGet(address, &value);
    int sign = 0;
    int command = 0;
    int opperand = 0;
    if (sign == 0) {
        putchar('+');
    } else {
        putchar('-');
    }
    sc_commandDecode(value, &sign, &command, &opperand);
    printf("%0*X%0*X", 2, command, 2, opperand);
    fflush(stdout);
    mt_setdefaultcolor();
}