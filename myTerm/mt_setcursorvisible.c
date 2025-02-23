#include "myTerm.h"
#include <stdio.h>
#include <unistd.h>

int mt_setcursorvisible(int value)
{
    int flag = 0;
    if (value) {
        flag = write(1, "\033[?25h\033[?8c", 11);
    } else {
        flag = write(1, "\033[?25l\033[?1c", 11);
    }
    if (flag < 11) {
        return -1;
    }
    return 0;
}