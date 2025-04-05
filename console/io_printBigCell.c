#include "../include/mySimpleComputer.h"
#include "../myBigChars/myBigChars.h"
#include "io.h"

#include <stdio.h>
void io_printBigCell(void) {
    if (nowRedact < 0 || nowRedact > 127) return;

    int value;
    sc_memoryGet(nowRedact, &value);
    int sign, command, operand;
    sc_commandDecode(value, &sign, &command, &operand);

    if (sign == 0) {
        bc_printbigchar(bigchar[16], 64, 9, WHITE, BLACK); // '+'
    } else {
        bc_printbigchar(bigchar[17], 64, 9, WHITE, BLACK); // '-'
    }
    int command1 = command >> 4;
    int command2 = command & 0xF;
    int operand1 = operand >> 4;
    int operand2 = operand & 0xF;
    bc_printbigchar(bigchar[command1], 73, 9, WHITE, BLACK);
    bc_printbigchar(bigchar[command2], 82, 9, WHITE, BLACK);
    bc_printbigchar(bigchar[operand1], 91, 9, WHITE, BLACK);
    bc_printbigchar(bigchar[operand2], 100, 9, WHITE, BLACK);
    mt_gotoXY(64, 17);
    mt_setfgcolor(BLUE);
    printf("Номер редактируемой ячейки: %03d", nowRedact);
    fflush(stdout);
    mt_setfgcolor(WHITE);
}