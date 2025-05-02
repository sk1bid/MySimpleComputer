#include "../include/mySimpleComputer.h"
#include "../mySimpleComputer/sc_reg.h"
#include <stdio.h>

void io_printFlags(void)
{
    mt_gotoXY(91, 2);

    int ind = 0;
    sc_regGet(FLAG_OVERFLOW, &ind);
    if (ind == 0) {
        putchar('_');
    } else {
        putchar('P');
    }
    printf("  ");
    sc_regGet(FLAG_DIVISION_BY_ZERO, &ind);
    if (ind == 0) {
        putchar('_');
    } else {
        putchar('0');
    }
    printf("  ");
    sc_regGet(FLAG_MEMORY_ERROR, &ind);
    if (ind == 0) {
        putchar('_');
    } else {
        putchar('M');
    }
    printf("  ");
    sc_regGet(FLAG_IGNORE_CLOCK_TICKS, &ind);
    if (ind == 0) {
        putchar('_');
    } else {
        putchar('T');
    }
    printf("  ");
    sc_regGet(FLAG_INVALID_COMMAND, &ind);
    if (ind == 0) {
        putchar('_');
    } else {
        putchar('E');
    }
    fflush(stdout);
}
