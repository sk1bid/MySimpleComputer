#include "../include/mySimpleComputer.h"
#include "sc_signals.h"

int ALU(int command, int operand)
{
    int value, acc = accumulator;
    if (sc_memoryGet(operand, &value))
        return -1;

    switch (command) {
    case 0x1E: // ADD
        acc += value;
        if (acc > 0x7FFF || acc < -0x7FFF) {
            sc_regSet(FLAG_OVERFLOW, 1);
            return -1;
        }
        break;
    case 0x1F: // SUB
        acc -= value;
        if (acc > 0x7FFF || acc < -0x7FFF) {
            sc_regSet(FLAG_OVERFLOW, 1);
            return -1;
        }
        break;
    case 0x20: // DIVIDE
        if (value == 0) {
            sc_regSet(FLAG_DIVISION_BY_ZERO, 1);
            return -1;
        }
        acc /= value;
        break;
    case 0x21: // MUL
        acc *= value;
        if (acc > 0x7FFF || acc < -0x7FFF) {
            sc_regSet(FLAG_OVERFLOW, 1);
            return -1;
        }
        break;
    case 0x34: // AND
        acc &= value;
        break;
    default:
        return -1;
    }
    accumulator = acc & 0x7FFF;
    return 0;
}