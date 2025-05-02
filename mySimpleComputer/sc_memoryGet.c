#include "../include/mySimpleComputer.h"
#include "sc_memory.h"
#include <stdio.h>

extern CacheLine cache[CACHE_LINES];

// Возвращает:
// 1 - при кэш-попадании
// 10 - при кэш-промахе
// -1 - при ошибке
int sc_memoryGet(int address, int* value)
{
    if (address < 0 || address >= MEMORY_SIZE) {
        sc_regSet(FLAG_MEMORY_ERROR, 1);
        return -1;
    }

    if (value == NULL) {
        return -1;
    }

    int line_addr = (address / CACHE_LINE_SIZE)
            * CACHE_LINE_SIZE; // Адрес начала строки
    int offset = address % CACHE_LINE_SIZE; // Смещение внутри строки

    int cache_index = sc_cacheFindLine(line_addr);

    if (cache_index != -1) {
        // Cache Hit
        *value = cache[cache_index].data[offset];
        return 1;
    } else {
        // Cache Miss
        log_message("\n-------------CacheGet miss-------------\n\n");
        int progress_X = 8;
        int progress_Y = 21 + current_cache_line;
        log_message("index = %d\n", current_cache_line);
        mt_gotoXY(progress_X, progress_Y);
        printf("Loading: ");
        fflush(stdout);
        int lru_index = sc_cacheFindLRU();

        if (sc_cacheLoadLine(line_addr, lru_index) != 0) {
            sc_regSet(FLAG_MEMORY_ERROR, 1);
            return -1;
        }

        *value = cache[lru_index].data[offset];
        return 10;
    }
}