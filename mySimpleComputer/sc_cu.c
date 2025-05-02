#include "../console/io.h"
#include "../include/mySimpleComputer.h"
#include "../myReadKey/myReadKey.h"
#include "sc_signals.h"
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>

int is_reading = 0;
int read_address = -1;

void CU()
{
    int ignore;
    sc_regGet(FLAG_IGNORE_CLOCK_TICKS, &ignore);
    if (ignore) {
        return;
    }

    int ic, value, sign, cmd, op;
    int ticks = 0;

    sc_icounterGet(&ic);
    char buffer[100];
    if (ic < 0 || ic >= 128) {
        sc_regSet(FLAG_MEMORY_ERROR, 1);
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);

        return;
    }

    ticks = sc_memoryGet(ic, &value);
    if (ticks == -1) {
        sc_regSet(FLAG_MEMORY_ERROR, 1);
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        return;
    } else {
        idle_counter = ticks - 1;
        int line_addr = (ic / CACHE_LINE_SIZE) * CACHE_LINE_SIZE;
        current_cache_line = sc_cacheFindLine(line_addr);
        if (ticks == 10) {
            return;
        }
    }

    if (sc_commandDecode(value, &sign, &cmd, &op)) {
        sc_regSet(FLAG_INVALID_COMMAND, 1);
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        return;
    }

    bool jumped = false;
    switch (cmd) {
    case 0x00: // NOP (ничего не делать)
        break;
    case 0x01: // CPUINFO (вывод информации об авторе)
        mt_gotoXY(1, 30);
        printf("Автор: Иванов Артём Михайлович, группа ИС-342\n");
        break;
    case 0x0A: // READ
        enum keys key;
        is_reading = 1;
        read_address = op;
        io_printTerm(op, 1);

        mt_setbgcolor(GREEN);
        mt_setfgcolor(BLACK);
        mt_gotoXY(73, 24);
        write(1, "     ", 5);
        mt_gotoXY(73, 24);

        char input[6] = {0};
        int pos = 0;

        while (pos < 5) {
            rk_readkey(&key);

            if (key == KEY_ENTER)
                break;
            if (key == KEY_ESCAPE) {
                is_reading = 0;
                read_address = -1;
                break;
            }

            char c = 0;
            if (key == KEY_PLUS) {
                c = '+';
            } else if (key == KEY_MINUS) {
                c = '-';
            } else if (key >= KEY_ZERO && key <= KEY_NINE) {
                c = '0' + (key - KEY_ZERO);
            } else if (key >= KEY_A && key <= KEY_F) {
                c = 'A' + (key - KEY_A);
            }

            if (c == 0)
                continue;

            if (pos == 0) { // Знак
                if (c == '+' || c == '-') {
                    input[pos++] = c;
                    write(1, &c, 1);
                }
            } else if (pos <= 2) { // Команда
                if (isxdigit(c)) {
                    input[pos++] = toupper(c);
                    write(1, &c, 1);
                }
            } else if (pos <= 4) { // Операнд
                if (isxdigit(c)) {
                    input[pos++] = toupper(c);
                    write(1, &c, 1);
                }
            }
        }

        input[pos] = '\0';

        if (pos == 5) {
            int sign = (input[0] == '-') ? 1 : 0;
            int command, operand;
            if (sscanf(input + 1, "%2x%2x", &command, &operand) == 2) {
                int encoded_value;
                if (sc_commandEncode(sign, command, operand, &encoded_value)
                    == 0) {
                    ticks = sc_memorySet(read_address, encoded_value);
                    if (ticks == -1) {
                        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
                    } else {
                        idle_counter = ticks - 1;
                        int line_addr
                                = (ic / CACHE_LINE_SIZE) * CACHE_LINE_SIZE;
                        current_cache_line = sc_cacheFindLine(line_addr);
                        if (ticks == 10) {
                            return;
                        }
                    }
                    io_printTerm(read_address, 0); // Обновляем IN-OUT
                }
            }
        } else {
            mt_gotoXY(73, 24);
            write(1, " ERR ", 5);
        }

        mt_setdefaultcolor();
        is_reading = 0;
        read_address = -1;
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 0);
        break;
    case 0x0B: // WRITE (вывод значения)
        io_printTerm(op, 0);
        break;
    case 0x14: // LOAD (загрузка в аккумулятор)
        ticks = sc_memoryGet(op, &accumulator);

        if (ticks == -1) {
            sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        } else {
            idle_counter = ticks - 1;
            int line_addr = (ic / CACHE_LINE_SIZE) * CACHE_LINE_SIZE;
            current_cache_line = sc_cacheFindLine(line_addr);
            if (ticks == 10) {
                return;
            }
        }
        break;
    case 0x15: // STORE (сохранение из аккумулятора)
        ticks = sc_memorySet(op, accumulator);

        if (ticks == -1) {
            sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        } else {
            idle_counter = ticks - 1;
            int line_addr = (ic / CACHE_LINE_SIZE) * CACHE_LINE_SIZE;
            current_cache_line = sc_cacheFindLine(line_addr);
            if (ticks == 10) {
                return;
            }
        }
        break;
    case 0x1E: // ADD
    case 0x1F: // SUB
    case 0x20: // DIVIDE
    case 0x21: // MUL
    case 0x34: // AND
        if (ALU(cmd, op) != 0)
            return;
        break;
    case 0x28: // JUMP (переход)
        sc_icounterSet(op);
        sc_icounterGet(&ic);
        jumped = true;
        break;
    case 0x29: // JNEG (переход, если отрицательное)
        if (accumulator < 0) {
            jumped = true;
            sc_icounterSet(op);
            sc_icounterGet(&ic);
        }

        break;

    case 0x3A: // JP (переход, если четное)
        if (accumulator % 2 == 0) {
            jumped = true;
            sc_icounterSet(op);
            sc_icounterGet(&ic);
        }
        break;

    case 0x3B: // JNP (переход, если нечетное)
        if (accumulator % 2 != 0) {
            jumped = true;
            sc_icounterSet(op);
            sc_icounterGet(&ic);
        }
        break;

    case 0x2B: // HALT (остановка)
        sc_cacheFlush();
        stop_timer();
        sc_regSet(
                FLAG_IGNORE_CLOCK_TICKS,
                1); // Устанавливаем флаг для остановки выполнения
        sc_cacheFlush();
        io_printAccumulator();
        io_printCounters();
        io_printCommand();
        io_printBigCell();
        io_printFlags();
        io_printCache();
        for (int i = 0; i < 128; i++) {
            io_printCell(i, WHITE, BLACK);
        }
        return;
    case 0x33: // NOT (логическое отрицание)
        accumulator = ~accumulator & 0x7FFF;
        break;
    default:
        sc_regSet(FLAG_INVALID_COMMAND, 1);
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        sc_icounterSet(ic + 1);
        sc_icounterGet(&ic);
        break;
    }

    if (jumped == false) {
        sc_icounterSet(ic + 1);
        sc_icounterGet(&ic);
    }

    sc_cacheFlush();
    io_printAccumulator();
    io_printCounters();
    io_printBigCell();
    io_printCommand();
    io_printFlags();
    io_printCache();
    for (int i = 0; i < 128; i++) {
        io_printCell(i, WHITE, BLACK);
    }
    io_printCell(nowRedact, BLACK, WHITE);
    jumped = false;
    log_message("Команда: %d: ", ic);
    log_message("Command %02X%02X", cmd, op);
}