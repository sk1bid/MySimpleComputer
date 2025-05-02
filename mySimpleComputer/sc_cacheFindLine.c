#include "../include/mySimpleComputer.h"

extern CacheLine cache[CACHE_LINES];
extern uint64_t global_timer;

int sc_cacheFindLine(int line_address)
{
    for (int i = 0; i < CACHE_LINES; ++i) {
        if (cache[i].valid && cache[i].tag == line_address) {
            cache[i].last_access_time = ++global_timer;
            return i;
        }
    }
    return -1;
}