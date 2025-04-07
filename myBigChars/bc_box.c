#include "../myBigChars/myBigChars.h"
#include "../myTerm/myTerm.h"
#include "stdio.h"
#include "unistd.h"

int bc_box(
        int x1,
        int y1,
        int x2,
        int y2,
        enum colors box_fg,
        enum colors box_bg,
        char* header,
        enum colors header_fg,
        enum colors header_bg)
{
    if (x1 <= 0 || y1 <= 0 || x2 <= 0 || y2 <= 0) {
        return -1;
    }
    if (x2 < 2 || y2 < 2) {
        return -1;
    }

    write(1, "\033(0)", 3);
    mt_setbgcolor(box_bg);
    mt_setfgcolor(box_fg);
    // рисуем верхнюю крышку
    mt_gotoXY(x1, y1);
    putchar('l'); // левый угол
    for (int i = 0; i <= x2 - 2; i++) {
        putchar('q'); // горизонтальная линия
    }
    putchar('k'); // правый угол
    fflush(stdout);

    for (int i = y1 + y2 - 2; i != y1; i--) { // вывод боковых сторон
        mt_gotoXY(x1, i);
        putchar('x');
        fflush(stdout);
        mt_gotoXY(x1 + x2, i);
        putchar('x');
        fflush(stdout);
    }

    mt_gotoXY(x1, y1 + y2 - 1); // рисуем нижнию крышку
    putchar('m');
    for (int i = 0; i <= x2 - 2; i++) {
        putchar('q');
    }
    putchar('j');
    fflush(stdout);

    write(1, "\033(B", 3);
    // заголовок по центру
    mt_setfgcolor(header_fg);

    mt_setbgcolor(header_bg);

    int lenHeader = bc_strlen(header);

    if (lenHeader == -1 || lenHeader > x2 - 2) {
        return 0;
    }

    mt_gotoXY(x1 + (x2 - lenHeader) / 2, y1);
    printf("%s", header);
    fflush(stdout);

    mt_setdefaultcolor();
    return 0;
}