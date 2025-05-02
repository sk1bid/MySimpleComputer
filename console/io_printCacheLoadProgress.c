
#include "../include/mySimpleComputer.h"
#include "../myTerm/myTerm.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

extern int idle_counter;

// Отображает прогресс-бар загрузки кэша во время тактов простоя.

void io_printCacheLoadProgress(int remaining_idle_ticks)
{
    int progress_X = 17;
    int progress_Y = 20 + current_cache_line;
    log_message("index = %d\n", current_cache_line);
    const int bar_length = 10;

    mt_gotoXY(progress_X, progress_Y);
    mt_setdefaultcolor();

    int filled_length = 0;
    if (remaining_idle_ticks > 0) {
        filled_length = bar_length - remaining_idle_ticks;
        if (filled_length < 0)
            filled_length = 0;
        if (filled_length > bar_length)
            filled_length = bar_length;
    }
    int empty_length = bar_length - filled_length;

    // Рисуем заполненную часть

    mt_setbgcolor(GREEN);
    for (int i = 0; i < filled_length; ++i) {
        write(1, "         ", i);
    }

    // Рисуем пустую часть
    mt_setbgcolor(BLACK);
    for (int i = 0; i < empty_length; ++i) {
        write(1, "         ", i);
    }

    mt_setdefaultcolor();
}