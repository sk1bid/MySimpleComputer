#include "../include/mySimpleComputer.h"
#include "sc_memory.h"

extern int memory[MEMORY_SIZE];
extern CacheLine cache[CACHE_LINES];
extern uint64_t global_timer;

int sc_cacheLoadLine(int line_address, int cache_line_index)
{
    if (cache_line_index < 0 || cache_line_index >= CACHE_LINES) {
        return -1;
    }
    // Проверка адреса
    if (line_address < 0 || line_address >= MEMORY_SIZE
        || line_address % CACHE_LINE_SIZE != 0) {
        return -1;
    }

    // Сохраняем старую строку, если она была изменена
    if (sc_cacheSaveLine(cache_line_index) != 0) {
        return -1;
    }

    // Загружаем новую строку из основной памяти
    for (int i = 0; i < CACHE_LINE_SIZE; ++i) {
        int current_addr = line_address + i;
        if (current_addr >= 0 && current_addr < MEMORY_SIZE) {
            cache[cache_line_index].data[i] = memory[current_addr];
        } else {
            return -1;
        }
    }

    cache[cache_line_index].valid = 1;
    cache[cache_line_index].dirty = 0;
    cache[cache_line_index].tag = line_address;
    cache[cache_line_index].last_access_time = ++global_timer;

    return 0;
}