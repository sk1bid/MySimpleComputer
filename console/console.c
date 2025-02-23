#include "../console/io.h"
#include "../include/mySimpleComputer.h"
#include "../myTerm/myTerm.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main()
{
    if (!isatty(1)) {
        printf("Dont term stdout");
        return 1;
    }
    struct winsize WS;

    if (ioctl(1, TIOCGWINSZ, &WS)) {
        printf("can't get term size");
        return 2;
    }

    if (WS.ws_col < 115 && WS.ws_row < 30) {
        printf("cant open interface on your term");
        return 3;
    }
    sc_memoryInit(); // Инициализация оперативной памяти
    sc_regInit(); // Инициализация регистра флагов
    sc_accumulatorInit(); // Инициализация аккумулятора
    sc_icounterInit();
    sc_memorySet(10, 12345); // Запись значения в ячейку памяти 10
    sc_memorySet(20, -500); // Запись отрицательного значения
    sc_memorySet(50, 0); // Запись нуля
    sc_memorySet(127, 16383);
    sc_regSet(FLAG_OVERFLOW, 1);
    sc_regSet(FLAG_DIVISION_BY_ZERO, 1);
    sc_regSet(FLAG_MEMORY_ERROR, 0);
    sc_accumulatorSet(9876);
    sc_icounterSet(64);
    mt_clrscr();
    for (int i = 0; i != 128; i++) {
        io_printCell(i, WHITE, BLACK);
    }
    io_printCell(0, BLACK, WHITE);
    int value;
    sc_memoryGet(0, &value);
    io_printDecodedCommand(accumulator);

    io_printAccumulator();
    io_printCounters();
    sc_regInit();
    io_printFlags();
    io_printTerm(4, 1);
    io_printTerm(32, 1);
    io_printTerm(5, 1);
    io_printTerm(3, 1);
    io_printTerm(6, 1);
    io_printTerm(34, 1);
    io_printTerm(0, 1);
    io_printCommand();
    mt_gotoXY(1, 30);
    return 0;
}