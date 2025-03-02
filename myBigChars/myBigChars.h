#ifndef MY_BIG_CHARS
#define MY_BIG_CHARS

#include "../myTerm/myTerm.h"
#include "mySimpleComputer.h"
extern int bigchar[18][2];
/*
int bc_strlen (char * str) – подсчитывает количество
символов в UTF-8 строке. Если декодирование какого-либо символа невозможно или
передан некорректный указатель, то функция возвращает 0.
*/
int bc_strlen(char* str);

/*
int bc_printA (char * str) - выводит строку символов с
использованием дополнительной кодировочной таблицы;
*/
int bc_printA(char* str);

/*
int bc_box(int x1, int y1, int x2, int y2, enum
colors box_fg, enum colors box_bg, char *header,
enum colors header_fg, enum colors header_bg) -
выводит на экран псевдографическую рамку, в которой левый
верхний угол располагается в строке x1 и столбце y1, а её ширина
и высота равна y2 столбцов и x2 строк. Цвет псевдографических
символов и их фон указан в параметрах box_fg, box_bg.
Если передан корректный указатель на header и полученная строка декодируется из
UTF-8, то в верхней строке рамки посередине выводится строка заголовка с
указанным цветом символов на указанном цвете фона;
*/

int bc_box(
        int x1,
        int y1,
        int x2,
        int y2,
        enum colors box_fg,
        enum colors bog_bg,
        char* header,
        enum colors header_fg,
        enum colors header_bg);

/*
int bc_setbigcharpos (int * big, int x, int y,
int value) – манипулирует элементом шрифта и устанавливает
в нем значение знакоместа "большого символа" в строке x и столбце y в значение
value;
*/

int bc_setbigcharpos(int* big, int x, int y, int value);

/*
int bc_getbigcharpos(int * big, int x, int y,
int *value) - возвращает значение позиции в "большом символе" в строке x и
столбце y;
*/

int bc_getbigcharpos(int* big, int x, int y, int* value);

/*int bc_printbigchar (int [2], int x, int y, enum
color, enum color) - выводит на экран "большой символ"
размером восемь строк на восемь столбцов, левый верхний угол ко-
83
торого располагается в строке x и столбце y. Третий и четвёртый
параметры определяют цвет и фон выводимых символов. "Символ"
выводится исходя из значений массива целых чисел следующим
образом. В первой строке выводится 8 младших бит первого числа,
во второй следующие 8, в третьей и 4 следующие. В 5 строке выводятся 8 младших
бит второго числа и т.д. При этом если значение бита = 0, то выводится символ
"пробел", иначе - символ, закрашивающий знакоместо (ACS_CKBOARD);
*/

int bc_printbigchar(int big[2], int x, int y, enum colors fg, enum colors bg);

/*
int bc_bigcharwrite (int fd, int * big, int
count) - записывает заданное число "больших символов" в файл.
Формат записи определяется пользователем. Символы записываются в бинарном виде;
*/

int bc_bigcharwrite(int fd, int* big, int count);

/*
int bc_bigcharread (int fd, int * big, int need_
count, int * count) считывает из файла заданное количество
"больших символов". Третий параметр указывает адрес переменной, в которую
помещается количество считанных символов или 0, в случае ошибки.
*/

int bc_bigcharread(int fd, int* big, int need_count, int* count);
#endif