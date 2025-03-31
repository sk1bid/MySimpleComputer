#ifndef INCLUDE_MY_SIMPLE_COMPUTER_H
#define INCLUDE_MY_SIMPLE_COMPUTER_H

#include "../mySimpleComputer/sc_command.h"
#include "../mySimpleComputer/sc_memory.h"
#include "../mySimpleComputer/sc_reg.h"
#include "../mySimpleComputer/sc_signals.h"
#include "../myTerm/myTerm.h"

extern int memory[MEMORY_SIZE]; // Оперативная память
extern int accumulator;         // Аккумулятор
extern int regFlags;            // Регистр флагов
extern int instructionCounter;  // Счетчик команд
extern int is_reading;
extern int read_address;
#endif