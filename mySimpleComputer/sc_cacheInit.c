#include "../include/mySimpleComputer.h"
#include <string.h>

extern CacheLine cache[CACHE_LINES];
extern uint64_t global_timer;

// Инициализирует кэш, устанавливая все строки в невалидное состояние
int sc_cacheInit(void)
{
    for (int i = 0; i < CACHE_LINES; ++i) {
        cache[i].valid = 0;
        cache[i].dirty = 0;
        cache[i].tag = -1;
        cache[i].last_access_time = 0;
        memset(cache[i].data, 0, sizeof(int) * CACHE_LINE_SIZE);
    }
    global_timer = 0;
    return 0;
}