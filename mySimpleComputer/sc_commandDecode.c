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
    if (sign == NULL || command == NULL || operand == NULL) {
        return -1; // неверный указатель
    }

    *sign = (value & SIGN_MASK) ? 1 : 0; // извлекаем знак (старший бит)
    *command
            = (value & COMMAND_MASK) >> 8; // извлекаем код операции (8-14) биты
    *operand = (value & OPERAND_MASK); // извлекаем операнд (младшие 7 бит)
    return 0;
}