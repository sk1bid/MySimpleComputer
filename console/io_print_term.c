#include "../include/mySimpleComputer.h"
#include "../myTerm/myTerm.h"
#include <stdio.h>

char INOUT[5][15] = {"", "", "", "", ""};

void io_printTerm(int address, int input)
{
    int sign;
    int command;
    int opperand;
    int value;
    sc_memoryGet(address, &value);
    sc_commandDecode(value, &sign, &command, &opperand);

    char buffer[10];
    snprintf(
            buffer,
            sizeof(buffer),
            "%0*X> %c%0*d%0*d",
            2,
            address,
            (sign == 0) ? '+' : '-',
            2,
            command,
            2,
            opperand);

    for (int i = 0; i < 5 - 1; i++) {
        snprintf(INOUT[i], sizeof(INOUT[i]), "%s", INOUT[i + 1]);
    }
    snprintf(INOUT[5 - 1], sizeof(INOUT[5 - 1]), "%s", buffer);
    int start_X = 69;
    int start_Y = 19;
    for (int i = 0; i != 5; i++) {
        printf("%s", INOUT[i]);
        fflush(stdout);
        start_Y++;
        mt_gotoXY(start_X, start_Y);
    }
    fflush(stdout);
}