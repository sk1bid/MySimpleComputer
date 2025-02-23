#include "myTerm.h"
#include <stdio.h>
#include <unistd.h>

int mt_setdefaultcolor()
{
    int flag = 0;
    flag = write(1, "\033[0m", 4);
    if (flag < 4) {
        return -1;
    }
    return 0;
}