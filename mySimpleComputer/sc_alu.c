#include "../include/mySimpleComputer.h"
#include "sc_signals.h"

int ALU(int command, int operand) {
    int value, acc = accumulator;
    if (sc_memoryGet(operand, &value)) return -1;

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
        case 0x50: // MUL2 (пользовательская: умножение на 2)
            acc *= 2;
            if (acc > 0x7FFF || acc < -0x7FFF) {
                sc_regSet(FLAG_OVERFLOW, 1);
                return -1;
            }
            break;
        case 0x51: // DIV2 (пользовательская: деление на 2)
            acc /= 2;
            break;
        default:
            return -1;
    }
    accumulator = acc & 0x7FFF;
    return 0;
}