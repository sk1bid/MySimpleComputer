#include "myTerm.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int mt_setbgcolor(enum colors color)
{
    char buffer[50];
    sprintf(buffer, "\033[4%dm", color);

    unsigned int byte_count = strlen(buffer);
    int flag = 0;

    flag = write(1, buffer, byte_count);
    if (flag < byte_count) {
        return -1;
    }
    return 0;
}
