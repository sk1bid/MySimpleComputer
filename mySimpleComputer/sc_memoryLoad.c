#include "sc_memory.h"
#include "stdio.h"

extern int memory[MEMORY_SIZE];

/*
int sc_memoryLoad (char * filename) – загружает из
указанного файла содержимое оперативной памяти (используя
функцию read или fread). Если передан неверный указатель на имя
файла или произошла какая-либо ошибка чтения данных из файла,
то функция завершается со статусом -1,
при этом содержимое «оперативной памяти» никак не изменяется
(т.е. оно не должно портиться).
В случае успеха функция завершается со статусом 0;
*/

int sc_memoryLoad(char* filename)
{
    if (filename == NULL) {
        return -1; // неверный имя файла
    }

    FILE* file = fopen(filename, "rb"); // бинарный файл

    if (file == NULL) {
        return -1; // ошибка при открытии файла
    }

    size_t data = fread(memory, sizeof(int), MEMORY_SIZE, file);
    // загрузили из файла оперативную память (её содержимое)

    fclose(file);

    if (data != MEMORY_SIZE) {
        return -1; // ошибка при загрузке
    }

    return 0;
}