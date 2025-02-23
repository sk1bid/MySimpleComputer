#ifndef MY_TERM_H
#define MY_TERM_H

enum colors {
    BLACK = 0,
    RED = 1,
    GREEN = 2,
    YELLOW = 3,
    BLUE = 4,
    PURPLE = 5,
    LIGHTBLUE = 6,
    WHITE = 7
};

int mt_clrscr(void);
int mt_gotoXY(int row, int col);
int mt_getscreensize(int* rows, int* cols);
int mt_setfgcolor(enum colors color);
int mt_setbgcolor(enum colors color);
int mt_setdefaultcolor(void);
int mt_setcursorvisible(int value);
int mt_delline(void);

#endif