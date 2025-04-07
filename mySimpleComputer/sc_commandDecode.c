#include "sc_command.h"
#include "stdio.h"

/*
int sc_commandDecode (int value, int * sign, int* command, int * operand)
 – декодирует значение ячейки памяти как команду Simple Computer.
Если декодирование невозможно, то функция завершается со статусом -1
и выходные параметры не меняют своего значения.
Иначе статус завершения = 0;
*/

int sc_commandDecode(int value, int* sign, int* command, int* operand)
{
    // Проверка, что значение не выходит за пределы 15 бит
    if (value < 0 || value > 0x7FFF) { // 15 бит: 0–32767
        return -1;
    }

    // Декодирование
    *sign = (value >> 14) & 0x1;    // Извлекаем бит знака (14-й бит)
    *command = (value >> 7) & 0x7F; // Извлекаем код команды (7–13 биты)
    *operand = value & 0x7F;        // Извлекаем операнд (0–6 биты)

    return 0; // Успешное выполнение
}