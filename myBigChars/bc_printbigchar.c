#include "../myTerm/myTerm.h"
#include "myBigChars.h"
#include <stdio.h>
#include <unistd.h>

int bc_printbigchar(int big[2], int x, int y, enum colors fg, enum colors bg)
{
    write(1, "\033(0", 3);

    mt_setfgcolor(fg);
    mt_setbgcolor(bg);

    for (int y1 = 0; y1 != 8; y1++) {
        mt_gotoXY(x, y + y1);
        for (int x1 = 0; x1 != 8; x1++) {
            int value;
            bc_getbigcharpos(big, x1, y1, &value);
            printf("%c", value ? 'a' : ' '); // закрашиваем
        }
        fflush(stdout);
    }
    write(1, "\033(B", 3);
    mt_setdefaultcolor();
    return 0;
}
