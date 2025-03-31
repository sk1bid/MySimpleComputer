#include "../console/io.h"
#include "../include/mySimpleComputer.h"
#include "sc_signals.h"
#include <unistd.h>

int is_reading = 0;    // Глобальная переменная
int read_address = -1; // Адрес для ввода

void CU()
{
    int ignore;
    sc_regGet(FLAG_IGNORE_CLOCK_TICKS, &ignore);
    if (ignore)
        return;

    int ic, value, sign, cmd, op;
    sc_icounterGet(&ic);
    if (ic < 0 || ic >= 128) {
        sc_regSet(FLAG_MEMORY_ERROR, 1);
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        return;
    }

    if (sc_memoryGet(ic, &value) || sc_commandDecode(value, &sign, &cmd, &op)) {
        sc_regSet(FLAG_INVALID_COMMAND, 1);
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        return;
    }

    switch (cmd) {
    case 0x00: // NOP
        break;
    case 0x01: // CPUINFO
        printf("Автор: Иванов Артём Михайлович, группа ИС-342");
        break;
    case 0x0A: // READ
        is_reading = 1;
        read_address = op;
        printTerm(op, 1); // Выводим "адрес< " в блок "IN-OUT"
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        return;
    case 0x0B: // WRITE
        printTerm(op, 0); // Выводим "адрес> +значение" в блок "IN-OUT"
        break;
    case 0x14: // LOAD
        sc_memoryGet(op, &accumulator);
        break;
    case 0x15: // STORE
        sc_memorySet(op, accumulator);
        break;
    case 0x1E: // ADD
    case 0x1F: // SUB
    case 0x20: // DIVIDE
    case 0x21: // MUL
    case 0x34: // AND
    case 0x50: // MUL2 (Пользовательская)
    case 0x51: // DIV2 (Пользовательская)
        if (ALU(cmd, op) != 0)
            return;
        break;
    case 0x28: // JUMP
        sc_icounterSet(op - 1);
        return;
    case 0x29: // JNEG
        if (accumulator < 0)
            sc_icounterSet(op - 1);
        break;
    case 0x2B: // HALT
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        stop_timer();
        rk_mytermregime(
                1, 0, 1, 1, 1); // Возвращаем терминал в канонический режим
        return;
    case 0x33: // NOT
        accumulator = ~accumulator & 0x7FFF;
        break;
    default:
        sc_regSet(FLAG_INVALID_COMMAND, 1);
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        return;
    }
    sc_icounterSet(ic + 1);
    io_printAccumulator();
    io_printCounters();
    io_printCommand();
    io_printFlags();
    for (int i = 0; i < 128; i++) {
        io_printCell(i, WHITE, BLACK);
    }
    io_printCell(nowRedact, BLACK, WHITE);
}