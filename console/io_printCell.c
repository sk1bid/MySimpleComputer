#include "../include/mySimpleComputer.h"
#include "../myTerm/myTerm.h"
#include <stdio.h>

void io_printCell(int address, enum colors fg, enum colors bg) {
    mt_setfgcolor(fg);
    mt_setbgcolor(bg);

    int y = (address / 10) + 2;
    int x = (address % 10) * 6 + 1 + 1;
    mt_gotoXY(x, y);

    int value;
    sc_memoryGet(address, &value);
    int sign, command, operand;
    sc_commandDecode(value, &sign, &command, &operand);
    putchar(sign == 0 ? '+' : '-');
    printf("%02X%02X", command, operand);
    fflush(stdout);
    mt_setdefaultcolor();
}