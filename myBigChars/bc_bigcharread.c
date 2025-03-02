#include "myBigChars.h"
#include <unistd.h>

int bc_bigcharread(int fd, int* big, int need_count, int* count)
{
    if (!big || !count)
        return -1;
    int flag;
    flag = read(fd, big, need_count * sizeof(int) * 2);
    if ((flag / (sizeof(int) * 2)) == need_count) {
        *count = flag / (sizeof(int) * 2);
        return 0;
    }
    *count = 0;
    return -1;
}