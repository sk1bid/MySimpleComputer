#include "sc_memory.h"
#include "stdio.h"

extern int memory[MEMORY_SIZE];

/*
int sc_memorySave (char * filename) – сохраняет содержимое памяти
в файл в бинарном виде (используя функцию write или fwrite).
Если передан неверный указатель на имя файла 
или произошла какая-либо ошибка записи данных в файл,
то функция завершается со статусом -1.
В случае успеха функция завершается со статусом 0;
*/

int sc_memorySave(char *filename){

    if (filename == NULL){
        return -1; // неверный указатель
    }

    FILE *file = fopen(filename, "wb"); // открываю файл для
    //бинарной записи

    if (file == NULL){
        return -1; //ошибка открытия
    }

    size_t data = fwrite(memory, sizeof(int),  MEMORY_SIZE, file);
    // записываю массив в файл

    fclose(file);

    if (data != MEMORY_SIZE){
        return -1; //ошибка при записи
    }
    
    return 0;

}