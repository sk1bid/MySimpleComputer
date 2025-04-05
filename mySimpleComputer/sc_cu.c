#include "../console/io.h"
#include "../include/mySimpleComputer.h"
#include "sc_signals.h"
#include <unistd.h>

int is_reading = 0;    // Глобальная переменная
int read_address = -1; // Адрес для ввода

// Управляющий блок (Control Unit)
void CU() {
    int ignore;
    sc_regGet(FLAG_IGNORE_CLOCK_TICKS, &ignore);
    if (ignore) {
        print_log("CU skipped due to FLAG_IGNORE_CLOCK_TICKS");
        return;
    }

    int ic, value, sign, cmd, op;
    sc_icounterGet(&ic);
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "instructionCounter = %d", ic);
    print_log(buffer);
    if (ic < 0 || ic >= 128) {
        sc_regSet(FLAG_MEMORY_ERROR, 1);
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        snprintf(buffer, sizeof(buffer), "Invalid instructionCounter: %d", ic);
        print_log(buffer);
        return;
    }

    if (sc_memoryGet(ic, &value)) {
        sc_regSet(FLAG_MEMORY_ERROR, 1);
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        snprintf(buffer, sizeof(buffer), "Failed to get memory at address %d", ic);
        print_log(buffer);
        return;
    }
    snprintf(buffer, sizeof(buffer), "Value at address %d = %04X", ic, value);
    print_log(buffer);

    if (sc_commandDecode(value, &sign, &cmd, &op)) {
        sc_regSet(FLAG_INVALID_COMMAND, 1);
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        snprintf(buffer, sizeof(buffer), "Failed to decode command at address %d: %04X", ic, value);
        print_log(buffer);
        return;
    }
    snprintf(buffer, sizeof(buffer), "Decoded command: sign = %d, cmd = %02X, op = %02X", sign, cmd, op);
    print_log(buffer);

    switch (cmd) {
        case 0x00: // NOP (ничего не делать)
            break;
        case 0x01: // CPUINFO (вывод информации об авторе)
            mt_gotoXY(1, 40);
            printf("Автор: Иванов Артём Михайлович, группа ИС-342\n");
            break;
        case 0x0A: // READ (чтение значения)
            is_reading = 1;
            read_address = op;
            io_printTerm(op, 1); // Вывод "адрес< " в блок "IN-OUT"
            sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
            mt_gotoXY(1, 26);
            fflush(stdout);
            return;
        case 0x0B: // WRITE (запись значения)
            io_printTerm(op, 0); // Вывод "адрес> +значение" в блок "IN-OUT"
            break;
        case 0x14: // LOAD (загрузка в аккумулятор)
            sc_memoryGet(op, &accumulator);
            break;
        case 0x15: // STORE (сохранение из аккумулятора)
            sc_memorySet(op, accumulator);
            break;
        case 0x1E: // ADD
        case 0x1F: // SUB
        case 0x20: // DIVIDE
        case 0x21: // MUL
        case 0x34: // AND
        case 0x50: // MUL2 (пользовательская команда)
        case 0x51: // DIV2 (пользовательская команда)
            if (ALU(cmd, op) != 0) return;
            break;
        case 0x28: // JUMP (переход)
            sc_icounterSet(op - 1);
            return;
        case 0x29: // JNEG (переход, если отрицательное)
            if (accumulator < 0) sc_icounterSet(op - 1);
            break;
        case 0x2B: // HALT (остановка)
            sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
            stop_timer();
            rk_mytermregime(1, 0, 1, 1, 1); // Возврат в канонический режим
            return;
        case 0x33: // NOT (логическое отрицание)
            accumulator = ~accumulator & 0x7FFF;
            break;
        default:
            snprintf(buffer, sizeof(buffer), "Unknown command: %02X", cmd);
            print_log(buffer);
            sc_regSet(FLAG_INVALID_COMMAND, 1);
            sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
            sc_icounterSet(ic + 1);
            sc_icounterGet(&ic); // Проверяем новое значение
            snprintf(buffer, sizeof(buffer), "After increment, instructionCounter = %d", ic);
            print_log(buffer);
            break;
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