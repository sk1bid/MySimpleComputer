#include "../include/mySimpleComputer.h"

int sc_cacheFlush(void)
{
    int result = 0;
    for (int i = 0; i < CACHE_LINES; ++i) {
        if (sc_cacheSaveLine(i) != 0) {
            result = -1;
        }
    }
    return result;
}