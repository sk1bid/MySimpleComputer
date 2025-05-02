#include "../include/mySimpleComputer.h"
#include "../myTerm/myTerm.h"
#include <stdio.h>
#include <stdlib.h>

void io_printAccumulator(void)
{
    mt_gotoXY(64, 2);

    int value = 0;
    sc_accumulatorGet(&value);

    // Интерпретируем 32-битное число как число в диапазоне Simple Computer
    int number = value;
    // Приводим к диапазону -16384..16383
    if (number > 16383)
        number = 16383;
    if (number < -16384)
        number = -16384;

    int encoded_value;
    int sign = (number < 0) ? 1 : 0;
    number = abs(number);
    if (sc_commandEncode(
                sign, (number >> 7) & 0x7F, number & 0x7F, &encoded_value)
        != 0) {
        printf("sc: ERR  hex: %04X", value & 0xFFFF);
        fflush(stdout);
        return;
    }

    int decoded_sign, command, operand;
    sc_commandDecode(encoded_value, &decoded_sign, &command, &operand);

    int display_number = (command << 7) | operand;
    if (decoded_sign)
        display_number = -display_number;

    printf("sc: ");
    if (decoded_sign == 0) {
        putchar('+');
    } else {
        putchar('-');
    }
    printf("%04d hex: %04X", abs(display_number), encoded_value);
    fflush(stdout);
}