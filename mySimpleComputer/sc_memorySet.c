#include "../include/mySimpleComputer.h"
#include "sc_memory.h"

extern CacheLine cache[CACHE_LINES];

// Возвращает:
// 1 - при кэш-попадании
// 10 - при кэш-промахе (с загрузкой)
// -1 - при ошибке
int sc_memorySet(int address, int value)
{
    if (address < 0 || address >= MEMORY_SIZE) {
        sc_regSet(FLAG_MEMORY_ERROR, 1);
        return -1;
    }

    // Проверка диапазона значения value (15 бит: -16384 до 16383)
    if (value < -16384 || value > 16383) {
        sc_regSet(FLAG_OVERFLOW, 1);
        return -1;
    }

    int line_addr = (address / CACHE_LINE_SIZE)
            * CACHE_LINE_SIZE; // Адрес начала строки
    int offset = address % CACHE_LINE_SIZE; // Смещение внутри строки

    int cache_index = sc_cacheFindLine(line_addr);

    if (cache_index != -1) {
        // Cache Hit
        cache[cache_index].data[offset] = value;
        cache[cache_index].dirty = 1;
        return 1; // Запись в кэш за 1 такт
    } else {
        // Cache Miss
        log_message(
                "\n-------------CacheLoad miss in %d-------------\n\n",
                current_cache_line);
        int lru_index = sc_cacheFindLRU();

        if (sc_cacheLoadLine(line_addr, lru_index) != 0) {
            sc_regSet(FLAG_MEMORY_ERROR, 1);
            return -1;
        }

        cache[lru_index].data[offset] = value;
        cache[lru_index].dirty = 1;

        return 10; // Загрузка строки + запись заняли 10 тактов
    }
}
