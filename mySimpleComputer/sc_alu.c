#include "../include/mySimpleComputer.h"
#include "sc_signals.h"

int ALU(int command, int operand)
{
    int value, acc = accumulator;
    int ticks;
    ticks = sc_memoryGet(operand, &value);

    if (ticks == -1) {
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
    } else {
        idle_counter = ticks - 1;
        int line_addr
                = (instructionCounter / CACHE_LINE_SIZE) * CACHE_LINE_SIZE;
        current_cache_line = sc_cacheFindLine(line_addr);
        if (ticks == 10) {
            return 0;
        }
    }

    switch (command) {
    case 0x1E: // ADD (сложение)
        acc += value;
        if (acc > 0x7FFF || acc < -0x7FFF) {
            sc_regSet(FLAG_OVERFLOW, 1);
            return -1;
        }
        break;
    case 0x1F: // SUB (вычитание)
        acc -= value;
        if (acc > 0x7FFF || acc < -0x7FFF) {
            sc_regSet(FLAG_OVERFLOW, 1);
            return -1;
        }
        break;
    case 0x20: // DIVIDE (деление)
        if (value == 0) {
            sc_regSet(FLAG_DIVISION_BY_ZERO, 1);
            return -1;
        }
        acc /= value;
        break;
    case 0x21: // MUL (умножение)
        acc *= value;
        if (acc > 0x7FFF || acc < -0x7FFF) {
            sc_regSet(FLAG_OVERFLOW, 1);
            return -1;
        }
        break;
    case 0x34: // AND (логическое И)
        acc &= value;
        break;
    default:
        return -1;
    }
    accumulator = acc;
    return 0;
}