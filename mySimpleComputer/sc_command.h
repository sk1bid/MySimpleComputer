#ifndef SC_COMMAND_H
#define SC_COMMAND_H

#define COMMAND_MASK 0x7F00 // Маска для выделения кода операции (биты 8-14)
#define OPERAND_MASK 0x007F // Маска для выделения операнда (биты 0-6)
#define SIGN_MASK 0x8000 // Маска для выделения знака (бит 15)

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

int sc_commandEncode(int sign, int command, int operand, int* value);

/*
int sc_commandDecode (int value, int * sign, int* command, int * operand)
 – декодирует значение ячейки памяти как команду Simple Computer.
Если декодирование невозможно, то функция завершается со статусом -1
и выходные параметры не меняют своего значения.
Иначе статус завершения = 0;
*/

int sc_commandDecode(int value, int* sign, int* command, int* operand);

/*
int sc_commandValidate (int command) – проверяет значение поля
«команда» на корректность.
Если значение некорректное, то возвращается -1.
Иначе возвращается 0;
*/

int sc_commandValidate(int command);

#endif