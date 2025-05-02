#include "../include/mySimpleComputer.h"
#include "../myTerm/myTerm.h"
#include <stdio.h>
#include <string.h>

char INOUT[5][15] = {"", "", "", "", ""};

void io_printTerm(int address, int input)
{
    char buffer[15];
    int value;
    int sign, command, operand;

    // Формирование строки в зависимости от типа операции
    if (input == 0) { // WRITE: вывод значения из памяти
        if (sc_memoryGet(address, &value) == -1) {
            snprintf(buffer, sizeof(buffer), "%02X> ERR", address);
        } else {
            if (sc_commandDecode(value, &sign, &command, &operand) == 0) {
                snprintf(
                        buffer,
                        sizeof(buffer),
                        "%02d> %c%02X%02X",
                        address,
                        (sign == 0) ? '+' : '-',
                        command,
                        operand);
            } else {
                snprintf(
                        buffer,
                        sizeof(buffer),
                        "%02d> %c%04X",
                        address,
                        (value >> 14) ? '-' : '+',
                        value & 0x3FFF);
            }
        }

    } else if (input == 1) { // READ: приглашение к вводу
        snprintf(buffer, sizeof(buffer), "%02d< ", address);
    }

    // Прокрутка истории
    for (int i = 0; i < 4; i++) {
        strncpy(INOUT[i], INOUT[i + 1], sizeof(INOUT[i]));
        INOUT[i][sizeof(INOUT[i]) - 1] = '\0';
    }

    // Обновление последней строки
    strncpy(INOUT[4], buffer, sizeof(INOUT[4]));
    INOUT[4][sizeof(INOUT[4]) - 1] = '\0';

    mt_setfgcolor(WHITE);
    mt_setbgcolor(BLACK);
    // Отрисовка блока IN-OUT
    int start_X = 69, start_Y = 20;
    for (int i = 0; i < 5; i++) {
        mt_gotoXY(start_X, start_Y + i);
        printf("%-9s", INOUT[i]);
        fflush(stdout);
    }
}