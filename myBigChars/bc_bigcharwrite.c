#include "myBigChars.h"
#include <unistd.h>

int bc_bigcharwrite(int fd, int* big, int count)
{
    int flag;
    flag = write(fd, big, count * sizeof(int) * 2);
    if (flag != count * sizeof(int) * 2) {
        return -1;
    }
    return 0;
}