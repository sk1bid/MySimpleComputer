#include "../console/io.h"
#include "../include/mySimpleComputer.h"
#include "../myBigChars/myBigChars.h"
#include "../myReadKey/myReadKey.h"
#include "../myTerm/myTerm.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

int total_cells = 128;
int num_cols = 10;
int num_rows = 13;
int current_cell = 0;
int accumulator = 0;
int instruction_counter = 0;

int main(int argc, char* argv[])
{
    if (!isatty(STDOUT_FILENO)) {
        printf("Not a terminal stdout\n");
        return 1;
    }
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws)) {
        printf("Can't get terminal size\n");
        return 2;
    }
    if (ws.ws_col < 115 || ws.ws_row < 30) {
        printf("Your terminal is too small!\n");
        return 3;
    }

    int font = (argc == 2) ? open(argv[1], O_RDONLY)
                           : open("font.bin", O_RDONLY);
    if (font == -1) {
        printf("Error: Can't open font file\n");
        return -1;
    }
    int count;
    if (bc_bigcharread(font, bigchar[0], 18, &count)) {
        printf("Error: Can't read font file\n");
        return -1;
    }
    close(font);

    // Initialize Simple Computer components
    sc_memoryInit();
    sc_regInit();
    sc_accumulatorInit();
    sc_icounterInit();
    sc_regSet(FLAG_OVERFLOW, 1);
    sc_regSet(FLAG_DIVISION_BY_ZERO, 1);
    sc_regSet(FLAG_MEMORY_ERROR, 0);
    sc_accumulatorSet(9876);
    sc_icounterSet(64);

    // Draw initial interface
    mt_clrscr();

    for (int i = 0; i < 128; i++) {
        io_printCell(i, WHITE, BLACK);
    }
    io_printCell(current_cell, BLACK, WHITE);
    io_printAccumulator();
    io_printCommand();
    io_printCounters();
    io_printFlags();
    io_printBigCell();

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
           47,
           12,
           WHITE,
           BLACK,
           "Редактируемая команда (увеличено)",
           RED,
           WHITE);
    bc_box(68, 19, 10, 7, WHITE, BLACK, "IN-OUT", GREEN, WHITE);
    bc_box(1, 19, 66, 7, WHITE, BLACK, "Кэш процессора", GREEN, WHITE);
    bc_box(79, 19, 31, 7, WHITE, BLACK, "Клавиши", GREEN, WHITE);

    mt_gotoXY(80, 20);
    printf("l - load  s - save  i - reset\n");
    fflush(stdout);
    mt_gotoXY(80, 21);
    printf("r - run  t - step\n");
    fflush(stdout);
    mt_gotoXY(80, 22);
    printf("ESC - выход\n");
    fflush(stdout);
    mt_gotoXY(80, 23);
    printf("F5 - accumulator\n");
    fflush(stdout);
    mt_gotoXY(80, 24);
    printf("F6  - instruction counter\n");
    fflush(stdout);

    rk_mytermsave();
    rk_mytermregime(0, 0, 1, 0, 0); // Non-canonical, no echo

    enum keys key;
    while (rk_readkey(&key), key != KEY_ESCAPE) {
        rk_mytermregime(0, 0, 1, 0, 0);
        if (key == KEY_OTHER) {
            continue;
        } else if (key == KEY_UP) {
            io_printCell(nowRedact, WHITE, BLACK);
            if (nowRedact == 9) {
                nowRedact = 118;

            } else if (nowRedact < 9) {
                nowRedact = (nowRedact - num_cols + total_cells) % total_cells;
                nowRedact += 1;

            } else {
                nowRedact = (nowRedact - num_cols + total_cells) % total_cells;
            }

        } else if (key == KEY_DOWN) {
            io_printCell(nowRedact, WHITE, BLACK);
            if (nowRedact == 118) {
                nowRedact = 9;

            } else if (nowRedact >= 119) {
                nowRedact = (nowRedact + num_cols) % total_cells;
                nowRedact -= 1;

            } else {
                nowRedact = (nowRedact + num_cols) % total_cells;
            }
        } else if (key == KEY_RIGHT) {
            io_printCell(nowRedact, WHITE, BLACK);
            nowRedact = (nowRedact + 1) % total_cells;
        } else if (key == KEY_LEFT) {
            io_printCell(nowRedact, WHITE, BLACK);
            nowRedact = (nowRedact - 1 + total_cells) % total_cells;
        } else if (key == KEY_ENTER) {
            mt_setbgcolor(GREEN);
            mt_setfgcolor(BLACK);
            int y = (nowRedact / 10) + 2;
            int x = (nowRedact % 10) * 6 + 1 + 1;
            mt_gotoXY(x, y);
            write(1, "     ", 5);
            mt_gotoXY(x, y);
            int value;
            if (!rk_readvalue(&value, 1)) {
                while (rk_readkey(&key),
                       key != KEY_ENTER && key != KEY_ESCAPE) {
                }
                if (key == KEY_ENTER) {
                    sc_memorySet(nowRedact, value);
                    mt_setdefaultcolor();
                    io_printTerm(nowRedact, 1);
                }
            }

        } else if (key == KEY_F5) {
            mt_setbgcolor(GREEN);
            mt_setfgcolor(BLACK);
            mt_gotoXY(68, 2);
            write(1, "     ", 5);
            mt_gotoXY(68, 2);
            int value;
            if (!rk_readvalue(&value, 1)) {
                while (rk_readkey(&key),
                       key != KEY_ENTER && key != KEY_ESCAPE) {
                }
                if (key == KEY_ENTER) {
                    sc_accumulatorSet(value);

                } else if (key == KEY_ESCAPE) {
                }
            }
            mt_setdefaultcolor();
            io_printAccumulator();
        } else if (key == KEY_F6) {
            mt_setbgcolor(GREEN);
            mt_setfgcolor(BLACK);
            mt_gotoXY(67, 5);
            write(1, "  ", 2);
            mt_gotoXY(67, 5);

            int value = 0;

            for (int i = 0; i != 2; i++) {
                while (rk_readkey(&key),
                       !((key >= KEY_ZERO && key <= KEY_NINE)
                         || (key >= KEY_A && key <= KEY_F))) {
                    if (key == KEY_ESCAPE) {
                        break;
                    }
                }
                if (key == KEY_ESCAPE) {
                    break;
                }

                int addCommand = 0;
                if (key >= KEY_ZERO && key <= KEY_NINE) {
                    addCommand = key - 48;
                } else {
                    addCommand = key - 55;
                }
                write(1, &key, 1);
                value <<= 4;
                value |= addCommand;
            }
            mt_setdefaultcolor();
            if (key != KEY_ESCAPE) {
                while (rk_readkey(&key),
                       key != KEY_ENTER && key != KEY_ESCAPE) {
                }
                if (key == KEY_ENTER) {
                    sc_icounterSet(value);

                } else if (key == KEY_ESCAPE) {
                }
            }

            io_printCounters();
            io_printCommand();
        } else if (key == KEY_s) {
            rk_mytermregime(1, 0, 0, 0, 1);
            mt_gotoXY(1, 26);
            mt_delline();
            char buffer[150];
            printf("Введите имя файла для сохранения: ");
            mt_setcursorvisible(1);
            fgets(buffer, sizeof(buffer), stdin);
            mt_setcursorvisible(0);
            mt_gotoXY(1, 26);
            mt_delline();
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
            }
            if (sc_memorySave(buffer)) {
                printf("Ошибка при сохранении файла %s", buffer);
            } else {
                printf("Файл %s успешно сохранен", buffer);
            }
            fflush(stdout);
            rk_mytermregime(0, 0, 1, 0, 0);
        } else if (key == KEY_l) {
            rk_mytermregime(1, 0, 0, 0, 1);
            mt_gotoXY(1, 26);
            mt_delline();
            char buffer[150];
            printf("Введите имя файла для загрузки: ");
            mt_setcursorvisible(1);
            fgets(buffer, sizeof(buffer), stdin);
            mt_setcursorvisible(0);
            mt_gotoXY(1, 26);
            mt_delline();
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
            }
            if (sc_memoryLoad(buffer)) {
                printf("Ошибка при чтении файла %s", buffer);
            } else {
                mt_gotoXY(1, 26);
                mt_delline();
                printf("Файл %s успешно считан", buffer);
                fflush(stdout);
                for (int i = 0; i != 128; i++) {
                    io_printCell(i, WHITE, BLACK);
                }
            }
            fflush(stdout);
            rk_mytermregime(0, 0, 1, 0, 0);
        } else if (key == KEY_i) {
            sc_memoryInit();
            sc_accumulatorInit();
            sc_regInit();
            sc_icounterInit();
            sc_regInit();
            for (int address = 0; address != 128; address++) {
                io_printCell(address, WHITE, BLACK);
            }
            io_printAccumulator();
            io_printCounters();
            io_printCommand();
            io_printFlags();
        }

        // выводим обновленную информацию
        io_printCell(nowRedact, BLACK, WHITE);
        io_printBigCell();
        int value;
        sc_memoryGet(nowRedact, &value);
        io_printDecodedCommand(value);
        mt_gotoXY(1, 30);
    }
    mt_setcursorvisible(1);
    rk_mytermregime(1, 0, 0, 0, 0);
    return 0;
}