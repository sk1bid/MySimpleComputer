#include "../include/mySimpleComputer.h"
#include <limits.h> // Для UINT64_MAX

extern CacheLine cache[CACHE_LINES];

int sc_cacheFindLRU(void)
{
    int lru_index = 0;
    uint64_t min_time = UINT64_MAX;

    for (int i = 0; i < CACHE_LINES; ++i) {
        if (!cache[i].valid) {
            return i;
        }
        if (cache[i].last_access_time < min_time) {
            min_time = cache[i].last_access_time;
            lru_index = i;
        }
    }
    return lru_index;
}