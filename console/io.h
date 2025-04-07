#ifndef IO_H_
#define IO_H_

#include "../myTerm/myTerm.h"
extern int nowRedact;
extern char INOUT[5][15];

void io_printCell(int address, enum colors fg, enum colors bg);
void io_printFlags(void);
void io_printDecodedCommand(int value);
void io_printAccumulator(void);
void io_printCounters(void);
void io_printTerm(int address, int input);
void io_printCommand(void);
void io_printBigCell(void);
void print_log(const char* message);

#endif