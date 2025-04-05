#include "../include/mySimpleComputer.h"
#include "../myTerm/myTerm.h"
#include <stdio.h>

char INOUT[5][15] = {"", "", "", "", ""};

void io_printTerm(int address, int input) {
    int sign, command, operand, value;
    sc_memoryGet(address, &value);
    sc_commandDecode(value, &sign, &command, &operand);

    char buffer[15];
    if (input == 0) { // Вывод (WRITE)
        snprintf(buffer, sizeof(buffer), "%02X> %c%02d%02d", 
                 address, (sign == 0) ? '+' : '-', command, operand);
    } else if (input == 1) { // Ввод (READ, до ввода)
        snprintf(buffer, sizeof(buffer), "%02X< ", address);
    } else if (input == 2) { // После ввода для READ
        int value;
        sc_accumulatorGet(&value);
        snprintf(buffer, sizeof(buffer), "%02X< +%04X", address, value);
    }

    for (int i = 0; i < 4; i++) {
        snprintf(INOUT[i], sizeof(INOUT[i]), "%s", INOUT[i + 1]);
    }
    snprintf(INOUT[4], sizeof(INOUT[4]), "%s", buffer);

    int start_X = 69, start_Y = 19;
    for (int i = 0; i < 5; i++) {
        mt_gotoXY(start_X, start_Y + i);
        printf("%s", INOUT[i]);
        fflush(stdout);
    }
}