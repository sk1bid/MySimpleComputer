#include "../include/mySimpleComputer.h"
#include "../myBigChars/myBigChars.h"
#include "io.h"

#include <stdio.h>
void io_printBigCell(void)
{
    if (nowRedact < 0 || nowRedact > 127) {
        return;
    }

    int value;
    sc_memoryGet(nowRedact, &value); // номер редактируемой ячейки
    int sign;
    int command;
    int opperand;
    sc_commandDecode(value, &sign, &command, &opperand);
    if (sign == 0) {
        bc_printbigchar(bigchar[16], 64, 9, WHITE, BLACK);
    } else {
        bc_printbigchar(bigchar[17], 64, 9, WHITE, BLACK);
    }
    int command1, command2;
    int opperand1, opperand2;
    command1 = command >> 4;
    command2 = command & 0xF;
    opperand1 = opperand >> 4;
    opperand2 = opperand & 0xF;
    bc_printbigchar(bigchar[command1], 73, 9, WHITE, BLACK);
    bc_printbigchar(bigchar[command2], 82, 9, WHITE, BLACK);
    bc_printbigchar(bigchar[opperand1], 91, 9, WHITE, BLACK);
    bc_printbigchar(bigchar[opperand2], 100, 9, WHITE, BLACK);
    mt_gotoXY(64, 17);
    mt_setfgcolor(BLUE);
    printf("Номер редактируемой ячейки: %0*u", 3, nowRedact);
    fflush(stdout);
}