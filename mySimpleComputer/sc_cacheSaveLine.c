#include "../include/mySimpleComputer.h"
#include "sc_memory.h"

extern int memory[MEMORY_SIZE];
extern CacheLine cache[CACHE_LINES];

int sc_cacheSaveLine(int cache_line_index)
{
    if (cache_line_index < 0 || cache_line_index >= CACHE_LINES) {
        return -1;
    }

    if (cache[cache_line_index].valid && cache[cache_line_index].dirty) {
        int start_addr = cache[cache_line_index].tag;
        if (start_addr != -1) {
            for (int i = 0; i < CACHE_LINE_SIZE; ++i) {
                int current_addr = start_addr + i;
                if (current_addr >= 0 && current_addr < MEMORY_SIZE) {
                    memory[current_addr] = cache[cache_line_index].data[i];
                } else {
                    return -1;
                }
            }
            cache[cache_line_index].dirty = 0;
        } else {
            return -1;
        }
    }
    return 0;
}