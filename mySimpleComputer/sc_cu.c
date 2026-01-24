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
    io_printCache();
    int ic, value, sign, cmd, op;
    int ticks = 0;

    sc_icounterGet(&ic);
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
        if (ticks == 10) {
            current_cache_line = sc_cacheFindLine(line_addr);
            return;
        }
    }

    if (sc_commandDecode(value, &sign, &cmd, &op)) {
        sc_regSet(FLAG_INVALID_COMMAND, 1);
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        return;
    }
    ticks = sc_memoryGet(op, &value);
    if (ticks == -1) {
        sc_regSet(FLAG_MEMORY_ERROR, 1);
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        return;
    } else {
        idle_counter = ticks - 1;
        int line_addr = (op / CACHE_LINE_SIZE) * CACHE_LINE_SIZE;
        if (ticks == 10) {
            current_cache_line = sc_cacheFindLine(line_addr);
            return;
        }
    }

    bool jumped = false;
    switch (cmd) {
    case 0x00: // NOP
        break;
    case 0x01: // CPUINFO
        mt_gotoXY(1, 30);
        printf("Автор: Иванов Артём Михайлович, группа ИС-342\n");
        break;
    case 0x0A: // READ
    {
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
            if (key == KEY_PLUS)
                c = '+';
            else if (key == KEY_MINUS)
                c = '-';
            else if (key >= KEY_ZERO && key <= KEY_NINE)
                c = '0' + (key - KEY_ZERO);
            else if (key >= KEY_A && key <= KEY_F)
                c = 'A' + (key - KEY_A);
            if (c == 0)
                continue;
            if (pos == 0) {
                if (c == '+' || c == '-') {
                    input[pos++] = c;
                    write(1, &c, 1);
                }
            } else if (pos <= 2) {
                if (isxdigit(c)) {
                    input[pos++] = toupper(c);
                    write(1, &c, 1);
                }
            } else if (pos <= 4) {
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
                int encoded;
                if (sc_commandEncode(sign, command, operand, &encoded) == 0) {
                    ticks = sc_memorySet(read_address, encoded);
                    if (ticks == -1) {
                        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
                    } else {
                        mt_setdefaultcolor();
                        idle_counter = ticks - 1;
                        int line_addr = (read_address / CACHE_LINE_SIZE)
                                * CACHE_LINE_SIZE;
                        current_cache_line = sc_cacheFindLine(line_addr);
                        if (ticks == 10)
                            current_cache_line = sc_cacheFindLine(line_addr);
                    }
                    io_printTerm(read_address, 0);
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
    } break;
    case 0x0B: // WRITE
        io_printTerm(op, 0);
        break;
    case 0x14: // LOAD
        ticks = sc_memoryGet(op, &accumulator);
        if (ticks == -1)
            sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        else {
            idle_counter = ticks - 1;
            if (ticks == 10)
                return;
        }
        break;
    case 0x15: // STORE
        ticks = sc_memorySet(op, accumulator);
        if (ticks == -1)
            sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        else {
            idle_counter = ticks - 1;
            if (ticks == 10)
                return;
        }
        break;
    case 0x1E:
    case 0x1F:
    case 0x20:
    case 0x21:
    case 0x34:
        if (ALU(cmd, op) != 0)
            return;
        break;
    case 0x28: // JUMP
        sc_icounterSet(op);
        sc_icounterGet(&ic);
        jumped = true;
        break;
    case 0x29: // JNEG
        if (accumulator < 0) {
            sc_icounterSet(op);
            sc_icounterGet(&ic);
            jumped = true;
        }
        break;
    case 0x2A: // JZ
        if (accumulator == 0) {
            sc_icounterSet(op);
            sc_icounterGet(&ic);
            jumped = true;
        }
        break;
    case 0x2B: // HALT
        sc_cacheFlush();
        stop_timer();
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        io_printAccumulator();
        io_printCounters();
        io_printCommand();
        io_printBigCell();
        io_printFlags();
        io_printCache();
        for (int i = 0; i < 128; i++)
            io_printCell(i, WHITE, BLACK);
        return;
    case 0x3A: // JP (четное)
        if ((accumulator & 1) == 0) {
            sc_icounterSet(op);
            sc_icounterGet(&ic);
            jumped = true;
        }
        break;
    case 0x3B: // JNP (нечетное)
        if (accumulator & 1) {
            sc_icounterSet(op);
            sc_icounterGet(&ic);
            jumped = true;
        }
        break;
    default:
        sc_regSet(FLAG_INVALID_COMMAND, 1);
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        sc_icounterSet(ic + 1);
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 0);
        break;
    }

    if (!jumped) {
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
    for (int i = 0; i < 128; i++)
        io_printCell(i, WHITE, BLACK);
    
    // Highlight current instruction
    sc_icounterGet(&ic);
    nowRedact = ic;
    io_printCell(nowRedact, BLACK, WHITE);
}
