#include "../console/io.h"
#include "../include/mySimpleComputer.h"
#include "../myReadKey/myReadKey.h"
#include "sc_signals.h"
#include <ctype.h>
#include <unistd.h>

int is_reading = 0;    // Глобальная переменная
int read_address = -1; // Адрес для ввода

// Управляющий блок (Control Unit)
void CU()
{
    int ignore;
    sc_regGet(FLAG_IGNORE_CLOCK_TICKS, &ignore);
    if (ignore) {
        return;
    }

    int ic, value, sign, cmd, op;
    sc_icounterGet(&ic);
    char buffer[100];

    if (ic < 0 || ic >= 128) {
        sc_regSet(FLAG_MEMORY_ERROR, 1);
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);

        return;
    }

    if (sc_memoryGet(ic, &value)) {
        sc_regSet(FLAG_MEMORY_ERROR, 1);
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);

        return;
    }

    if (sc_commandDecode(value, &sign, &cmd, &op)) {
        sc_regSet(FLAG_INVALID_COMMAND, 1);
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        return;
    }

    switch (cmd) {
    case 0x00: // NOP (ничего не делать)
        break;
    case 0x01: // CPUINFO (вывод информации об авторе)
        mt_gotoXY(1, 30);
        printf("Автор: Иванов Артём Михайлович, группа ИС-342\n");
        break;
    case 0x0A: // READ
        enum keys key;
        is_reading = 1;
        read_address = op;
        io_printTerm(op, 1); // Вывод приглашения "XX< "

        mt_setbgcolor(GREEN);
        mt_setfgcolor(BLACK);
        mt_gotoXY(73, 24);
        write(1, "     ", 5); // Очистка области ввода
        mt_gotoXY(73, 24);

        char input[6] = {0};
        int pos = 0;

        while (pos < 5) {
            rk_readkey(&key);

            if (key == KEY_ENTER)
                break;
            if (key == KEY_ESCAPE) {
                is_reading = 0;
                read_address = -1;
                break;
            }

            // Преобразование key в символ
            char c = 0;
            if (key == KEY_PLUS) {
                c = '+';
            } else if (key == KEY_MINUS) {
                c = '-';
            } else if (key >= KEY_ZERO && key <= KEY_NINE) {
                c = '0' + (key - KEY_ZERO);
            } else if (key >= KEY_A && key <= KEY_F) {
                c = 'A' + (key - KEY_A);
            }

            if (c == 0)
                continue; // Невалидный символ

            if (pos == 0) { // Знак
                if (c == '+' || c == '-') {
                    input[pos++] = c;
                    write(1, &c, 1);
                }
            } else if (pos <= 2) { // Команда
                if (isxdigit(c)) {
                    input[pos++] = toupper(c);
                    write(1, &c, 1);
                }
            } else if (pos <= 4) { // Операнд
                if (isxdigit(c)) {
                    input[pos++] = toupper(c);
                    write(1, &c, 1);
                }
            }
        }

        input[pos] = '\0';

        if (pos == 5) {
            int sign = (input[0] == '-') ? 1 : 0;
            int command, operand;
            if (sscanf(input + 1, "%2x%2x", &command, &operand) == 2) {
                int encoded_value;
                if (sc_commandEncode(sign, command, operand, &encoded_value)
                    == 0) {
                    sc_memorySet(read_address, encoded_value);
                    io_printTerm(read_address, 0); // Обновляем IN-OUT
                }
            }
        } else {
            mt_gotoXY(73, 24);
            write(1, " ERR ", 5); // Вывод ошибки
        }

        mt_setdefaultcolor();
        is_reading = 0;
        read_address = -1;
        sc_regSet(
                FLAG_IGNORE_CLOCK_TICKS,
                0); // Сбрасываем флаг, чтобы таймер продолжил работу
        break;
    case 0x0B:               // WRITE (запись значения)
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
        if (ALU(cmd, op) != 0)
            return;
        break;
    case 0x28: // JUMP (переход)
        sc_icounterSet(op - 1);
        return;
    case 0x29: // JNEG (переход, если отрицательное)
        if (accumulator < 0)
            sc_icounterSet(op - 1);
        break;
    case 0x2B: // HALT (остановка)
        stop_timer();
        sc_regSet(
                FLAG_IGNORE_CLOCK_TICKS,
                1); // Устанавливаем флаг для остановки выполнения
        return;
    case 0x33: // NOT (логическое отрицание)
        accumulator = ~accumulator & 0x7FFF;
        break;
    default:
        sc_regSet(FLAG_INVALID_COMMAND, 1);
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        sc_icounterSet(ic + 1);
        sc_icounterGet(&ic); // Проверяем новое значение
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