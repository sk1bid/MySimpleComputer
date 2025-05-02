#include "sc_command.h"

/*
int sc_commandValidate (int command) – проверяет значение поля
«команда» на корректность.
Если значение некорректное, то возвращается -1.
Иначе возвращается 0;
*/

int sc_commandValidate(int command)
{
    if (command < 0 || command > 128) {
        return -1;
    }
    return 0;
}