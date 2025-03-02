#include "myBigChars.h"

int bc_setbigcharpos(int* big, int x, int y, int value)
{
    char* byte;
    byte = (char*)big + (y / 4) * sizeof(int) + y % 4;
    if (value) {
        *byte |= (1 << 7 - x);
    } else {
        *byte &= ~(1 << 7 - x);
    }
    return 0;
}