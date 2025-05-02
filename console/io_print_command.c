#include "io.h"
#include "mySimpleComputer.h"
#include <stdio.h>
void io_printCommand()
{
    int value;
    int sign;
    int command;
    int operand;
    int ic;
    sc_icounterGet(&ic);
    sc_memoryGetDirect(ic, &value);
    sc_commandDecode(value, &sign, &command, &operand);
    mt_gotoXY(95, 5);
    if (operand > 128) {
        printf("! + FF : FF");
        fflush(stdout);
        return;
    }
    putchar(sign == 0 ? '+' : '-');
    printf(" %02X : %02X", command, operand);
    fflush(stdout);
}