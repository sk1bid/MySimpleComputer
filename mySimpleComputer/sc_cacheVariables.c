#include "../include/mySimpleComputer.h"

CacheLine cache[CACHE_LINES];
uint64_t global_timer = 0;
int current_cache_line = -1;