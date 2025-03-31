#ifndef SIGNALS_H
#define SIGNALS_H
#include <signal.h>
#include <stdio.h>
#include <sys/time.h>

extern int idle_counter;

void CU(void);
/*
void CU (void) – реализует алгоритм работы одного такта
устройства управления.
*/

int ALU(int command, int operand);
/*
int ALU (int command, int operand) – реализует алгоритм работы одного такта
арифметико-логического устройства.
*/

void IRC(int signum);
/*
void IRC (int signum) – которая реализует
алгоритм работы контроллера прерываний. Считаем, что от генератора
импульсов будет поступать сингал SIGALRM, от Reset – сигнал
SIGUSR1;
*/
void setup_interrupts(void);

void stop_timer(void);

void start_timer(void);
#endif