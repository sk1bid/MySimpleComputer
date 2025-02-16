#include "sc_command.h"
#include "stdio.h"

/* int sc_commandEncode (int sign, int command, int operand, int * value)
– кодирует значение ячейки в соответствии с форматом команды
Simple Computer и с использованием в качестве значений
полей полученные знак,
номер команды и операнд и помещает результат в value.
Если указаны недопустимые значения для знака,
команды или операнда, то функция завершается со статусом -1
и значение value не изменяется.
В противном случае – статус завершения 0. Для знака, операнда и
команды допустимыми являются все значения,
 которые соответствуют формату команды Simple Computer;
*/

int sc_commandEncode(int sign, int command, int operand, int* value)
{
    if (value == NULL) {
        return -1; // неверный указатель
    }

    if (sign != 0 && sign != 1) {
        return -1; // неверный знак
    }

    if (command < 0 || command > 127) {
        return -1; // недопустимый код команды
    }

    if (operand < 0 || operand > 127) {
        return -1; // недопустимый операнд
    }

    *value = 0;
    if (sign) {
        *value |= SIGN_MASK; // устанавливаем бит знака, если sign==1
    }
    *value |= (command << 8); // сдвигаем команду на 8 бит влево
    *value |= operand; // добавляем операнд
    return 0;
}