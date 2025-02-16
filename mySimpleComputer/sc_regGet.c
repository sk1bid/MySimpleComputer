#include "sc_reg.h"
#include "stdio.h"
extern int regFlags;

int sc_regGet(int reg, int* value)
{
    if (reg != FLAG_OVERFLOW && reg != FLAG_DIVISION_BY_ZERO
        && reg != FLAG_MEMORY_ERROR && reg != FLAG_IGNORE_CLOCK_TICKS
        && reg != FLAG_INVALID_COMMAND) {
        return -1; // Ошибка: недопустимый регистр
    }

    if (value == NULL) {
        return -1; // Ошибка: неверный указатель
    }

    *value = (regFlags & reg) ? 1 : 0;
    // получаем значение аккумулятора
    return 0;
}