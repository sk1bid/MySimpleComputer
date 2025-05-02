#ifndef SC_CACHE_H
#define SC_CACHE_H

#include <stdint.h>

#define CACHE_LINES 5 // Количество строк в кэше
#define CACHE_LINE_SIZE 10 // Размер строки кэша (в значениях)

// Структура для одной строки кэша
typedef struct {
    int valid; // Флаг валидности строки
    int dirty; // Флаг: 1 - строка изменена, 0 - не изменена
    int tag; // Адрес начала блока памяти, хранящегося в строке (-1 если
             // невалидна)
    int data[CACHE_LINE_SIZE]; // Данные строки кэша
    uint64_t last_access_time; // Время последнего доступа (для LRU)
} CacheLine;

// Инициализация кэша
int sc_cacheInit(void);

// Поиск строки в кэше по адресу памяти
int sc_cacheFindLine(int memory_address);

// Поиск наименее используемой строки (LRU)
int sc_cacheFindLRU(void);

// Загрузка строки из ОП в кэш
int sc_cacheLoadLine(int memory_address, int cache_line_index);

// Сохранение строки из кэша в ОП (если dirty)
int sc_cacheSaveLine(int cache_line_index);

// Сброс (запись всех измененных строк) кэша в ОП
int sc_cacheFlush(void);

#endif // SC_CACHE_H