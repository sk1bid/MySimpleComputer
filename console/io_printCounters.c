#include "../include/mySimpleComputer.h"
#include "../myTerm/myTerm.h"
#include "io.h"
#include <stdio.h>

void io_printCounters(void)
{
    mt_gotoXY(64, 5);

    int value;
    int sign;
    int command;
    int opperand;
    sc_icounterGet(&value);
    sc_commandDecode(value, &sign, &command, &opperand);

    printf("T: 00     IC: ");
    if (sign == 0) {
        putchar('+');
    } else {
        putchar('-');
    }
    printf("%0*X%0*x", 2, command, 2, opperand);
    fflush(stdout);
}