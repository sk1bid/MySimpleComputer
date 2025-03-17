#include "../myReadKey/myReadKey.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
// я считаю в моей версии timeout не нужен

/*считаю, что функцию можно было бы реализовать намного проще если бы она не
 * проверяла каждый введенный пользователем символ*/

int rk_readvalue(int* value, int timeout)
{
    struct termios Save;
    tcgetattr(1, &Save);

    rk_mytermregime(0, 0, 1, 0, 0);
    enum keys key;

    //ввод знака + или -
    while (rk_readkey(&key), key != KEY_MINUS && key != KEY_PLUS) {
        if (key == KEY_ESCAPE) {
            tcsetattr(1, TCSANOW, &Save);
            return 1;
        }
    }
    tcsetattr(1, TCSANOW, &Save);
    write(1, (key == KEY_PLUS) ? "+" : "-", 1);

    int sign = (key == KEY_PLUS) ? 1 : -1;
    *value = 0; // Инициализируем значение

    rk_mytermregime(0, 0, 1, 0, 0);

    //ввод команды 4 16-ных знака
    for (int i = 0; i < 4; i++) {
        while (rk_readkey(&key),
               !((key >= KEY_ZERO && key <= KEY_NINE)
                 || (key >= KEY_A && key <= KEY_F))) {
            if (key == KEY_ESCAPE) {
                tcsetattr(1, TCSANOW, &Save);
                return 1;
            }
        }
        int hex_digit_value;
        if (key >= KEY_ZERO && key <= KEY_NINE) {
            hex_digit_value = key - KEY_ZERO;
        } else {
            hex_digit_value = key - KEY_A + 10;
        }

        *value = (*value << 4) | hex_digit_value; // Сдвигаем на 4 бита и добавляем новую цифру

        tcsetattr(1, TCSANOW, &Save);
        char c = key;
        write(1, &c, 1);
        rk_mytermregime(0, 0, 1, 0, 0);
    }
    tcsetattr(1, TCSANOW, &Save);
    *value *= sign; // Учитываем знак
    return 0;
}