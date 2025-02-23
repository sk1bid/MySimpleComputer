#include "io.h"
#include "mySimpleComputer.h"
#include <stdio.h>
void io_printCommand()
{
    int value;
    int sign;
    int command;
    int opperand;
    sc_icounterGet(&value);
    sc_commandDecode(value, &sign, &command, &opperand);
    mt_gotoXY(91, 5);
    if (opperand > 128) {
        printf("! + FF : FF");
        fflush(stdout);
        return;
    }
    char buffer[50];
    snprintf(
            buffer,
            sizeof(buffer),
            "%c %0*X : %0*X",
            (sign == 0) ? '+' : '-',
            2,
            command,
            2,
            opperand);
    printf("%s", buffer);
    fflush(stdout);
}