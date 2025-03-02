#include "../console/io.h"
#include "../include/mySimpleComputer.h"
#include "../myBigChars/myBigChars.h"
#include "../myTerm/myTerm.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (!isatty(1)) {
        printf("not term stdout");
        return 1;
    }
    struct winsize WS;

    if (ioctl(1, TIOCGWINSZ, &WS)) {
        printf("can't get term size");
        return 2;
    }

    if (WS.ws_col < 115 && WS.ws_row < 30) {
        printf("your term is too small!");
        return 3;
    }

    // загружаем шрифт
    int font;
    if (argc > 2) {
        printf("too many command argument");
        return -1;
    } else if (argc == 2) {
        font = open(argv[1], O_RDONLY);
    } else {
        font = open("font.bin", O_RDONLY);
    }
    if (font == -1) {
        printf("error: can't open file");
        return -1;
    }

    int count;
    if (bc_bigcharread(font, bigchar[0], 18, &count)) {
        printf("error: can't read file");
        return -1;
    }

    mt_clrscr();
    nowRedact = 10;
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

    bc_box(1, 1, 61, 15, WHITE, BLACK, "Оперативная память", RED, BLACK);
    bc_box(1,
           16,
           61,
           3,
           WHITE,
           BLACK,
           "Редактируемая ячейка (формат)",
           RED,
           WHITE);
    bc_box(63, 1, 23, 3, WHITE, BLACK, "Аккумулятор", RED, BLACK);
    bc_box(87, 1, 23, 3, WHITE, BLACK, "Регистр флагов", RED, BLACK);
    bc_box(63, 4, 23, 3, WHITE, BLACK, "Счетчик команд", RED, BLACK);
    bc_box(87, 4, 23, 3, WHITE, BLACK, "Команда", RED, BLACK);
    bc_box(63,
           7,
           46,
           12,
           WHITE,
           BLACK,
           "Редактируемая команда (увеличено)",
           RED,
           WHITE);
    bc_box(68, 19, 9, 7, WHITE, BLACK, "IN-OUT", GREEN, WHITE);
    bc_box(1, 19, 66, 7, WHITE, BLACK, "Кэш процессора", GREEN, WHITE);
    bc_box(78, 19, 31, 7, WHITE, BLACK, "Клавиши", GREEN, WHITE);

    io_printBigCell();
    mt_gotoXY(1, 30);
    mt_setdefaultcolor();
    return 0;
}