#include "sc_reg.h"

extern int regFlags;

/*
int sc_regSet (int register, int value)
– устанавливает значение указанного регистра флагов.
Для номеров регистров флагов должны использоваться маски,
задаваемые макросами (#define).
Если указан недопустимый регистр, то функция завершается
со статусом -1 и значение флага не меняется.
Иначе статус завершения – 0.
Флаг меняется в соответствии с правилами определения
логического значения целой переменной, принятых в языке Си;
*/

int sc_regSet(int reg, int value) {
    // Проверяем допустимость регистра флагов
    if (reg != FLAG_OVERFLOW && reg != FLAG_DIVISION_BY_ZERO &&
        reg != FLAG_MEMORY_ERROR && reg != FLAG_IGNORE_CLOCK_TICKS &&
        reg != FLAG_INVALID_COMMAND) {
        return -1; // Ошибка: недопустимый регистр
    }
    if (value != 0 && value != 1) {
        return -1; // Ошибка: недопустимое значение флага
    }

    if (value) {
        regFlags |= reg; // Устанавливаем флаг (битовое ИЛИ)
    } else {
        regFlags &= ~reg; // Сбрасываем флаг (битовое И с инверсией)
    }
    return 0;
}