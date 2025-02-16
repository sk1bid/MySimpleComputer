#ifndef IO_H
#define I0_H

/*
void printCell (int address) – выводит на экран содержимое ячейки оперативной
памяти по указанному адресу. Формат вывода должен соответствовать заданию
(ячейка выводится в декодированном виде);
*/

void io_printCell(int address);

/*
void printFlags (void) – выводит значения флагов. Формат
должен соответствовать заданию (выводятся либо _, либо буквы в
заданной последовательности);
*/

void io_printFlags(void);

/*
void printDecodedCommand (int value) – выводит переданное значение в десятичной
системе счисления, в восьмеричной системе счисления, в шестнадцатиричной системе
счисления и в двоичной системе счисления.
*/

void io_printDecodedCommand(int value);

/*
void printAccumulator (void) – выводит значение аккумулятора;
*/

void io_printAccumulator(void);

/*
void printCounters (void) – выводит значение счетчика
команд.*/

void io_printCounters(void);

#endif