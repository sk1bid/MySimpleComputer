#include "../myReadKey/myReadKey.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int rk_readvalue(int* value, int timeout)
{
    struct termios save;
    tcgetattr(1, &save);

    rk_mytermregime(0, 0, 1, 0, 0);
    enum keys key;

    // Ввод знака
    while (rk_readkey(&key), key != KEY_MINUS && key != KEY_PLUS) {
        if (key == KEY_ESCAPE) {
            tcsetattr(1, TCSANOW, &save);
            return 1;
        }
    }
    tcsetattr(1, TCSANOW, &save);
    write(1, (key == KEY_PLUS) ? "+" : "-", 1);

    int sign = (key == KEY_PLUS) ? 1 : -1;
    *value = 0;

    rk_mytermregime(0, 0, 1, 0, 0);

    // Ввод команды (2 символа, 0-9 или A-E)
    for (int i = 0; i < 2; i++) {
        while (rk_readkey(&key),
               !((key >= KEY_ZERO && key <= KEY_NINE)
                 || (key >= KEY_A && key <= KEY_F))) {
            if (key == KEY_ESCAPE) {
                tcsetattr(1, TCSANOW, &save);
                return 1;
            }
        }
        int hex_digit_value = (key >= KEY_ZERO && key <= KEY_NINE)
                ? (key - KEY_ZERO)
                : (key - KEY_A + 10);
        *value = (*value << 4) | hex_digit_value;

        tcsetattr(1, TCSANOW, &save);
        char c = (key >= KEY_ZERO && key <= KEY_NINE) ? ('0' + (key - KEY_ZERO))
                                                      : ('A' + (key - KEY_A));
        write(1, &c, 1);
        rk_mytermregime(0, 0, 1, 0, 0);
    }

    // Ввод операнда (2 символа, 0-9 или A-F)
    for (int i = 0; i < 2; i++) {
        while (rk_readkey(&key),
               !((key >= KEY_ZERO && key <= KEY_NINE)
                 || (key >= KEY_A && key <= KEY_F))) {
            if (key == KEY_ESCAPE) {
                tcsetattr(1, TCSANOW, &save);
                return 1;
            }
        }
        int hex_digit_value = (key >= KEY_ZERO && key <= KEY_NINE)
                ? (key - KEY_ZERO)
                : (key - KEY_A + 10);
        *value = (*value << 4) | hex_digit_value;

        tcsetattr(1, TCSANOW, &save);
        char c = (key >= KEY_ZERO && key <= KEY_NINE) ? ('0' + (key - KEY_ZERO))
                                                      : ('A' + (key - KEY_A));
        write(1, &c, 1);
        rk_mytermregime(0, 0, 1, 0, 0);
    }
    tcsetattr(1, TCSANOW, &save);
    *value *= sign;
    return 0;
}