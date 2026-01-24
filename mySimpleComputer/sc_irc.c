#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE

#include "../console/io.h"
#include "../include/mySimpleComputer.h"
#include "sc_signals.h"
#include <signal.h>
#include <string.h>
#include <sys/time.h>

void IRC(int sig)
{
    if (sig == SIGALRM) {
        int ignore;
        sc_regGet(FLAG_IGNORE_CLOCK_TICKS, &ignore);
        // Если процессор не игнорирует такты И idle_counter равен 0 И не идет
        // чтение, ТО ВЫПОЛНЯЕМ CU Иначе (если ignore=1 ИЛИ idle_counter>0 ИЛИ
        // is_reading=1), то просто уменьшаем idle_counter (если он > 0) и
        // выходим
        if (ignore || idle_counter > 0 || is_reading) {
            if (idle_counter > 0) {
                log_message("Пропуск CU  idle counter = %d\n", idle_counter);
                io_printCacheLoadProgress(idle_counter);

                io_printCommand();
                io_printCounters();
                idle_counter--; // Уменьшаем счетчик простоя

                return; // Пропускаем вызов CU
            }
        }
        log_message(" idle counter in irc: %d ", idle_counter);
        log_message("Заход в CU\n");
        // Если дошли сюда, значит idle_counter == 0 и можно выполнять команду
        CU();
    } else if (sig == SIGUSR1) {
        sc_memoryInit();
        sc_regInit();
        sc_accumulatorInit();
        sc_icounterInit();
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        stop_timer();
        instructionCounter = 0;
        accumulator = 0;
        idle_counter = 0;
        is_reading = 0;
        read_address = -1;
        current_cache_line = -1;
        for (int i = 0; i < 5; i++) {
            memset(INOUT[i], 0, sizeof(INOUT[i]));
        }
        io_printAccumulator();
        io_printCounters();
        io_printCommand();
        io_printFlags();
        io_printCache();
        for (int i = 0; i < 128; i++) {
            io_printCell(i, WHITE, BLACK);
        }
        io_printCell(nowRedact, BLACK, WHITE);
        int ic;
        sc_icounterGet(&ic);
        char buffer[100];
        int start_X = 69;
        int start_Y = 20;
        for (int i = 0; i < 5; i++) {
            mt_gotoXY(start_X, start_Y + i);
            printf("%-9s", INOUT[i]);
            fflush(stdout);
        }
        start_X = 2;
        start_Y = 20;
        for (int i = 0; i < CACHE_LINES; i++) {
            mt_gotoXY(start_X, start_Y + i);
            for (int j = 0; j <= CACHE_LINE_SIZE; j++) {
                for (int p = 0; p < 6; p++) {
                    printf(" ");
                }
            }
            printf("\n");
        }
        bc_box(1, 19, 66, 7, WHITE, BLACK, "Кэш процессора", GREEN, WHITE);
    }
}
void setup_interrupts()
{
    struct sigaction sa = {0};
    sa.sa_handler = IRC;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);
}

void start_timer()
{
    struct itimerval timer
            = {.it_interval = {.tv_usec = 500000},
               .it_value = {.tv_usec = 500000}};
    setitimer(ITIMER_REAL, &timer, NULL);
}

void stop_timer()
{
    struct itimerval timer
            = {.it_interval = {.tv_usec = 0}, .it_value = {.tv_usec = 0}};
    setitimer(ITIMER_REAL, &timer, NULL);
}