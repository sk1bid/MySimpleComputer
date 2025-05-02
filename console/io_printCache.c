#include "../include/mySimpleComputer.h"
#include "../myTerm/myTerm.h"
#include <stdio.h>
#include <string.h>

extern CacheLine cache[CACHE_LINES];

void io_printCache()
{
    mt_setfgcolor(WHITE);
    mt_setbgcolor(BLACK);

    int start_X = 2;
    int start_Y = 20;
    for (int i = 0; i < CACHE_LINES; i++) {
        mt_gotoXY(start_X, start_Y + i);
        if (cache[i].tag == -1) {
            printf(" -1:");
        } else {
            printf("%03d: ", cache[i].tag);
            for (int j = 0; j < CACHE_LINE_SIZE; j++) {
                int sign, command, operand;
                sc_commandDecode(cache[i].data[j], &sign, &command, &operand);
                putchar(sign == 0 ? '+' : '-');
                printf("%02X%02X", command, operand);
                if (j < CACHE_LINE_SIZE - 1)
                    printf(" ");
            }
        }
        fflush(stdout);
    }
}