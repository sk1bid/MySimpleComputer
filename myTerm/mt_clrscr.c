#include <stdio.h>
#include <unistd.h>
#include "myTerm.h"

int mt_clrscr(void)
{
    int flag = 0;
    flag = write(1, "\033[H\033[J", 6);
    if (flag < 6) {
        return -1;
    }
    return 0;
}