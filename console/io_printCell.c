#include "stdio.h"
#include "io.h"
#include "../include/mySimpleComputer.h"

/*
void printCell (int address) – выводит на экран содержимое ячейки оперативной памяти по указанному адресу. Формат
вывода должен соответствовать заданию (ячейка выводится в декодированном виде);
*/

void io_printCell(int adress){
    int value; 
    if (sc_memoryGet(adress, &value) == 0){ // получаем значение из памяти
        int sign, command, operand;
        if (sc_commandDecode(value, &sign, &command, &operand) == 0){ // декодируем ячейку как команду
            printf("Адрес %02X(%d): %c%02X : %02X\n", adress, adress, sign ? '-' : '+', command, operand);
        }
        else{
            printf("Адрес %02X: Ошибка декодирования\n", adress); // ошибка декодирования
        }
    
    } else {
        printf("Ошибка чтения памяти по адресу %02X\n", adress); //ошибка чтения памяти 
    }
}