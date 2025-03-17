
#include "myReadKey.h"
#include <termios.h>
#include <unistd.h>

int rk_readkey(enum keys* key)
{
    char buffer[8];
    int nbytes;
    nbytes = read(0, buffer, 7);
    *key = KEY_OTHER;

    if (nbytes == 1) {
        switch (buffer[0]) {
        case '\n':
            *key = KEY_ENTER;
            break;
        case '\033':
            *key = KEY_ESCAPE;
            break;
        case '+':
            *key = KEY_PLUS;
            break;
        case '-':
            *key = KEY_MINUS;
            break;
        case '0':
            *key = KEY_ZERO;
            break;
        case '1':
            *key = KEY_ONE;
            break;
        case '2':
            *key = KEY_TWO;
            break;
        case '3':
            *key = KEY_THREE;
            break;
        case '4':
            *key = KEY_FOUR;
            break;
        case '5':
            *key = KEY_FIVE;
            break;
        case '6':
            *key = KEY_SIX;
            break;
        case '7':
            *key = KEY_SEVEN;
            break;
        case '8':
            *key = KEY_EIGHT;
            break;
        case '9':
            *key = KEY_NINE;
            break;
        case 'A':
            *key = KEY_A;
            break;
        case 'B':
            *key = KEY_B;
            break;
        case 'C':
            *key = KEY_C;
            break;
        case 'D':
            *key = KEY_D;
            break;
        case 'E':
            *key = KEY_E;
            break;
        case 'F':
            *key = KEY_F;
            break;
        case 'l':
            *key = KEY_l;
            break;
        case 's':
            *key = KEY_s;
            break;
        case 'i':
            *key = KEY_i;
            break;
        }
    }
    if (nbytes == 3) {
        if (buffer[0] == '\033' && buffer[1] == '[') {
            switch (buffer[2]) {
            case 'A':
                *key = KEY_UP;
                break;
            case 'B':
                *key = KEY_DOWN;
                break;
            case 'C':
                *key = KEY_RIGHT;
                break;
            case 'D':
                *key = KEY_LEFT;
                break;
            }
        }
    } else if (nbytes == 5) {
        if (buffer[0] == '\033' && buffer[1] == '[' && buffer[2] == '1'
            && buffer[4] == '~') {
            switch (buffer[3]) {
            case '5':
                *key = KEY_F5;
                break;
            case '7':
                *key = KEY_F6;
                break;
            }
        }
    }
    return 0;
}