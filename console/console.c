#include "../console/io.h"
#include "../include/mySimpleComputer.h"
#include "../myBigChars/myBigChars.h"
#include "../myReadKey/myReadKey.h"
#include "../myTerm/myTerm.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>
#include <termios.h>

int total_cells = 128;
int num_cols = 10;
int num_rows = 13;
char logs[10][100]; // Массив для хранения 10 логов
int log_count = 0; // Количество логов в массиве

void print_log(const char* message) {
    // // Сдвигаем существующие логи вверх
    // for (int i = 0; i < 9; i++) {
    //     strncpy(logs[i], logs[i + 1], sizeof(logs[i]));
    // }
    // // Добавляем новый лог в конец массива
    // strncpy(logs[9], message, sizeof(logs[4]));
    // logs[9][sizeof(logs[9]) - 1] = '\0'; // Убеждаемся, что строка завершена

    // // Увеличиваем счетчик логов, но не больше 5
    // if (log_count < 10) {
    //     log_count++;
    // }

    // // Перерисовываем строки 26–35
    // for (int i = 0; i < 10; i++) {
    //     mt_gotoXY(1, 26 + i);
    //     mt_delline();
    //     if (i < log_count) {
    //         mt_gotoXY(1, 26 + i);
    //         printf("DEBUG: %s\n", logs[i]);
    //     }
    // }
    // fflush(stdout);
}

int main(int argc, char* argv[])
{
    if (!isatty(STDOUT_FILENO)) {
        printf("Not a terminal stdout\n");
        return 1;
    }
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws)) {
        printf("Can't get terminal size\n");
        return 2;
    }
    if (ws.ws_col < 115 || ws.ws_row < 30) {
        printf("Your terminal is too small!\n");
        return 3;
    }

    int font = (argc == 2) ? open(argv[1], O_RDONLY) : open("font.bin", O_RDONLY);
    if (font == -1) {
        printf("Error: Can't open font file\n");
        return -1;
    }
    int count;
    if (bc_bigcharread(font, bigchar[0], 18, &count)) {
        printf("Error: Can't read font file\n");
        return -1;
    }
    close(font);

    sc_memoryInit();
    sc_regInit();
    sc_accumulatorInit();
    sc_icounterInit();
    sc_regSet(FLAG_OVERFLOW, 0);
    sc_regSet(FLAG_DIVISION_BY_ZERO, 0);
    sc_regSet(FLAG_MEMORY_ERROR, 0);
    sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
    sc_regSet(FLAG_INVALID_COMMAND, 0);

    mt_clrscr();

    for (int i = 0; i < 128; i++) {
        io_printCell(i, WHITE, BLACK);
    }
    io_printCell(nowRedact, BLACK, WHITE);
    io_printAccumulator();
    io_printCommand();
    io_printCounters();
    io_printFlags();
    io_printBigCell();
    io_printDecodedCommand(0);

    mt_gotoXY(1, 30);
    bc_box(1, 1, 61, 15, WHITE, BLACK, "Оперативная память", RED, BLACK);
    bc_box(1, 16, 61, 3, WHITE, BLACK, "Редактируемая ячейка (формат)", RED, WHITE);
    bc_box(63, 1, 23, 3, WHITE, BLACK, "Аккумулятор", RED, BLACK);
    bc_box(87, 1, 23, 3, WHITE, BLACK, "Регистр флагов", RED, BLACK);
    bc_box(63, 4, 23, 3, WHITE, BLACK, "Счетчик команд", RED, BLACK);
    bc_box(87, 4, 23, 3, WHITE, BLACK, "Команда", RED, BLACK);
    bc_box(63, 7, 47, 12, WHITE, BLACK, "Редактируемая команда (увеличено)", RED, WHITE);
    bc_box(68, 19, 10, 7, WHITE, BLACK, "IN-OUT", GREEN, WHITE);
    bc_box(1, 19, 66, 7, WHITE, BLACK, "Кэш процессора", GREEN, WHITE);
    bc_box(79, 19, 31, 7, WHITE, BLACK, "Клавиши", GREEN, WHITE);

    mt_gotoXY(80, 20);
    printf("l - load  s - save  i - reset\n");
    fflush(stdout);
    mt_gotoXY(80, 21);
    printf("r - run  t - step\n");
    fflush(stdout);
    mt_gotoXY(80, 22);
    printf("ESC - выход\n");
    fflush(stdout);
    mt_gotoXY(80, 23);
    printf("F5 - accumulator\n");
    fflush(stdout);
    mt_gotoXY(80, 24);
    printf("F6  - instruction counter\n");
    fflush(stdout);

    setup_interrupts();

    rk_mytermsave();
    rk_mytermregime(0, 0, 1, 0, 0); // Non-canonical, no echo

    enum keys key;
    while (rk_readkey(&key), key != KEY_ESCAPE) {
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Key pressed: %d", key);
        print_log(buffer);
        rk_mytermregime(0, 0, 1, 0, 0);

        // Обработка ввода для READ
        if (is_reading && read_address >= 0) {
            snprintf(buffer, sizeof(buffer), "Waiting for input, is_reading = %d, read_address = %d", is_reading, read_address);
            print_log(buffer);
            int value;
            rk_mytermregime(1, 0, 0, 0, 1);
            mt_gotoXY(1, 30); // Ввод всегда на строке 30
            printf("Input value for cell %02X: ");
            if (!rk_readvalue(&value, 1)) {
                int sign = (value < 0) ? 1 : 0;
                int command = (abs(value) >> 7) & 0x7F;
                int operand = abs(value) & 0x7F;
                int encoded_value;
                if (sc_commandEncode(sign, command, operand, &encoded_value) == 0) {
                    sc_memorySet(read_address, encoded_value);
                    io_printTerm(read_address, 2);
                    io_printCell(read_address, WHITE, BLACK);
                    snprintf(buffer, sizeof(buffer), "Value %04X written to cell %02X", encoded_value, read_address);
                    print_log(buffer);
                    is_reading = 0;
                    read_address = -1;
                    sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 0);
                    mt_gotoXY(1, 30);
                    printf("                              ");
                }
            } else {
                mt_gotoXY(1, 30);
                printf("Error: Invalid input          ");
                is_reading = 0;
                read_address = -1;
                sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 0);
            }
            rk_mytermregime(0, 0, 1, 0, 0);
            io_printFlags();
            continue;
        }

        // Обработка других клавиш
        if (key == KEY_OTHER) {
            continue;
        } else if (key == KEY_UP) {
            io_printCell(nowRedact, WHITE, BLACK);
            if (nowRedact == 9) {
                nowRedact = 118;
            } else if (nowRedact < 9) {
                nowRedact = (nowRedact - num_cols + total_cells) % total_cells;
                nowRedact += 1;
            } else {
                nowRedact = (nowRedact - num_cols + total_cells) % total_cells;
            }
        } else if (key == KEY_DOWN) {
            io_printCell(nowRedact, WHITE, BLACK);
            if (nowRedact == 118) {
                nowRedact = 9;
            } else if (nowRedact >= 119) {
                nowRedact = (nowRedact + num_cols) % total_cells;
                nowRedact -= 1;
            } else {
                nowRedact = (nowRedact + num_cols) % total_cells;
            }
        } else if (key == KEY_RIGHT) {
            io_printCell(nowRedact, WHITE, BLACK);
            nowRedact = (nowRedact + 1) % total_cells;
        } else if (key == KEY_LEFT) {
            io_printCell(nowRedact, WHITE, BLACK);
            nowRedact = (nowRedact - 1 + total_cells) % total_cells;
        } else if (key == KEY_ENTER) {
            int ignore;
            sc_regGet(FLAG_IGNORE_CLOCK_TICKS, &ignore);
            if (!ignore) continue;
        
            mt_setbgcolor(GREEN);
            mt_setfgcolor(BLACK);
            int y = (nowRedact / 10) + 2;
            int x = (nowRedact % 10) * 6 + 1 + 1;
            mt_gotoXY(x, y);
            write(1, "     ", 5); // Очистка места для ввода
            mt_gotoXY(x, y);
        
            char input[6] = {0}; // Буфер для ввода (5 символов + '\0')
            int pos = 0;         // Текущая позиция в буфере
        
            rk_mytermregime(0, 0, 1, 0, 0); // Устанавливаем неканонический режим
        
            while (pos < 5) {
                char c;
                read(0, &c, 1); // Читаем один символ
        
                if (c == '\n') { // Если Enter, завершаем ввод
                    break;
                } else if (pos == 0) { // Первый символ: знак
                    if (c == '+' || c == '-') {
                        input[pos++] = c;
                        write(1, &c, 1); // Выводим символ на экран
                    }
                } else if (pos >= 1 && pos <= 2) { // Символы кода команды (1A)
                    if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F')) {
                        input[pos++] = c;
                        write(1, &c, 1);
                    }
                } else if (pos >= 3 && pos <= 4) { // Символы операнда (34)
                    if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F')) {
                        input[pos++] = c;
                        write(1, &c, 1);
                    }
                }
            }
        
            input[pos] = '\0'; // Завершаем строку
        
            // Парсим введенную строку
            if (pos == 5 && (input[0] == '+' || input[0] == '-')) {
                int sign = (input[0] == '-') ? 1 : 0;
                int command, operand;
                if (sscanf(input + 1, "%2x%2x", &command, &operand) == 2) {
                    int encoded_value;
                    if (sc_commandEncode(sign, command, operand, &encoded_value) == 0) {
                        sc_memorySet(nowRedact, encoded_value);
                        io_printTerm(nowRedact, 0);
                        io_printFlags();
                        snprintf(buffer, sizeof(buffer), "Memory at address %d = %04X  %s\n", nowRedact, encoded_value, input);
                        print_log(buffer);
                    }
                } else {
                    snprintf(buffer, sizeof(buffer), "Invalid input: %s", input);
                    print_log(buffer);
                }
            } else {
                snprintf(buffer, sizeof(buffer), "Invalid format: %s", input);
                print_log(buffer);
            }
            mt_setdefaultcolor();
        } else if (key == KEY_F5) {
            mt_setbgcolor(GREEN);
            mt_setfgcolor(BLACK);
            mt_gotoXY(68, 2);
            write(1, "     ", 5);
            mt_gotoXY(68, 2);
            int value;
            if (!rk_readvalue(&value, 1)) {
                while (rk_readkey(&key), key != KEY_ENTER && key != KEY_ESCAPE) {}
                if (key == KEY_ENTER) {
                    sc_accumulatorSet(value);
                    io_printFlags();
                }
            }
            mt_setdefaultcolor();
            io_printAccumulator();
        } else if (key == KEY_F6) {
            mt_setbgcolor(GREEN);
            mt_setfgcolor(BLACK);
            mt_gotoXY(67, 5);
            write(1, "  ", 2);
            mt_gotoXY(67, 5);
            int value = 0;
            for (int i = 0; i != 2; i++) {
                while (rk_readkey(&key), !((key >= KEY_ZERO && key <= KEY_NINE) || (key >= KEY_A && key <= KEY_F))) {
                    if (key == KEY_ESCAPE) break;
                }
                if (key == KEY_ESCAPE) break;
                int addCommand = (key >= KEY_ZERO && key <= KEY_NINE) ? (key - 48) : (key - 55);
                write(1, &key, 1);
                value <<= 4;
                value |= addCommand;
            }
            mt_setdefaultcolor();
            if (key != KEY_ESCAPE) {
                while (rk_readkey(&key), key != KEY_ENTER && key != KEY_ESCAPE) {}
                if (key == KEY_ENTER) {
                    sc_icounterSet(value);
                    io_printFlags();
                }
            }
            io_printCounters();
            io_printCommand();
        } else if (key == KEY_s) {
            rk_mytermregime(1, 0, 0, 0, 1);
            mt_gotoXY(1, 26);
            mt_delline();
            char buffer[150];
            printf("Введите имя файла для сохранения: ");
            mt_setcursorvisible(1);
            fgets(buffer, sizeof(buffer), stdin);
            mt_setcursorvisible(0);
            mt_gotoXY(1, 26);
            mt_delline();
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') buffer[len - 1] = '\0';
            if (sc_memorySave(buffer)) {
                printf("Ошибка при сохранении файла %s", buffer);
            } else {
                printf("Файл %s успешно сохранен", buffer);
            }
            fflush(stdout);
            rk_mytermregime(0, 0, 1, 0, 0);
            io_printFlags();
        } else if (key == KEY_l) {
            rk_mytermregime(1, 0, 0, 0, 1);
            mt_gotoXY(1, 26);
            mt_delline();
            char buffer[150];
            printf("Введите имя файла для загрузки: ");
            mt_setcursorvisible(1);
            fgets(buffer, sizeof(buffer), stdin);
            mt_setcursorvisible(0);
            mt_gotoXY(1, 26);
            mt_delline();
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') buffer[len - 1] = '\0';
            if (sc_memoryLoad(buffer)) {
                printf("Ошибка при чтении файла %s", buffer);
            } else {
                mt_gotoXY(1, 26);
                mt_delline();
                printf("Файл %s успешно считан", buffer);
                fflush(stdout);
                for (int i = 0; i != 128; i++) {
                    io_printCell(i, WHITE, BLACK);
                }
                io_printFlags();
            }
            fflush(stdout);
            rk_mytermregime(0, 0, 1, 0, 0);
        } else if (key == KEY_i) {
            // Очищаем логи
            log_count = 0;
            for (int i = 0; i < 5; i++) {
                logs[i][0] = '\0';
                mt_gotoXY(1, 26 + i);
                mt_delline();
            }
            raise(SIGUSR1);
            io_printFlags();
            int ic;
            sc_icounterGet(&ic);
            char buffer[100];
            snprintf(buffer, sizeof(buffer), "After KEY_i, instructionCounter = %d", ic);
            print_log(buffer);
            // Проверяем содержимое памяти на адресах 0 и 1
            int value0, value1;
            sc_memoryGet(0, &value0);
            sc_memoryGet(1, &value1);
            snprintf(buffer, sizeof(buffer), "Memory at address 0 = %04X, address 1 = %04X", value0, value1);
            print_log(buffer);
        } else if (key == KEY_r) {
            if (!is_reading && read_address == -1) {
                start_timer();
                sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 0); // Сбрасываем флаг перед запуском
                rk_mytermregime(0, 0, 1, 1, 0);
                print_log("Starting program with KEY_r");
            }
            io_printFlags();
        } else if (key == KEY_t) {
            if (!is_reading && read_address == -1) {
                stop_timer();
                idle_counter = 0;
                sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 0); // Сбрасываем флаг перед вызовом CU
                CU();
                print_log("Step executed with KEY_t");
            }
            io_printFlags();
        }

        io_printCell(nowRedact, BLACK, WHITE);
        io_printBigCell();
        int value;
        sc_memoryGet(nowRedact, &value);
        snprintf(buffer, sizeof(buffer), "Memory at address %d = %04X\n", nowRedact, value);
        print_log(buffer);
        io_printDecodedCommand(value);
        io_printFlags();
        mt_gotoXY(1, 30);
    }

    mt_setcursorvisible(1);
    rk_mytermregime(1, 0, 0, 0, 0);
    for (int i = 26; i <= 30; i++) {
        mt_gotoXY(1, i);
        mt_delline();
    }
    return 0;
}