#include "myBigChars.h"
#include <stdio.h>

int bc_getbigcharpos(int* big, int x, int y, int* value)
{
    if (big == NULL) {
        return -1;
    }
    char* byte;
    byte = (char*)big + (y / 4) * sizeof(int) + y % 4;
    *value = (*byte >> (7 - x)) & 1;
    return 0;
}