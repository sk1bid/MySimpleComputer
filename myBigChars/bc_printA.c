#include "myBigChars.h"
#include "stdio.h"
#include "unistd.h"

int bc_printA(char* str)
{
    if (str == NULL) {
        return -1;
    }

    if (write(1, "\033(0)", 3) == -1) { // вход в режим псевдографики
        return -1;
    }

    printf("%s", str);
    fflush(stdout);

    if (write(1, "\033(0)", 3) == -1) { // выход из режима псевдографики
        return -1;
    }
    return 0;
}