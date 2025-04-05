#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE

#include "../console/io.h"
#include "../include/mySimpleComputer.h"
#include "sc_signals.h"
#include <signal.h>
#include <string.h>
#include <sys/time.h>

// Обработчик сигналов (Interrupt Request Controller)
void IRC(int sig) {
    if (sig == SIGALRM) {
        int ignore;
        sc_regGet(FLAG_IGNORE_CLOCK_TICKS, &ignore);
        if (ignore || idle_counter > 0 || is_reading) {
            if (idle_counter > 0)
                idle_counter--;
            return;
        }
        CU();
    } else if (sig == SIGUSR1) {
        sc_memoryInit();
        sc_regInit();
        sc_accumulatorInit();
        sc_icounterInit();
        sc_regSet(FLAG_IGNORE_CLOCK_TICKS, 1);
        instructionCounter = 0;
        accumulator = 0;
        idle_counter = 0;
        is_reading = 0;
        read_address = -1;
        for (int i = 0; i < 5; i++) {
            memset(INOUT[i], 0, sizeof(INOUT[i]));
        }
        io_printAccumulator();
        io_printCounters();
        io_printCommand();
        io_printFlags();
        for (int i = 0; i < 128; i++) {
            io_printCell(i, WHITE, BLACK);
        }
        io_printCell(nowRedact, BLACK, WHITE);
        int ic;
        sc_icounterGet(&ic);
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "After reset, instructionCounter = %d", ic);
        print_log(buffer);
    }
}
// Настройка обработчиков сигналов
void setup_interrupts() {
    struct sigaction sa = {0};
    sa.sa_handler = IRC;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);
}

// Запуск таймера (генерация SIGALRM каждые 0.5 секунды)
void start_timer() {
    struct itimerval timer = {
        .it_interval = {.tv_usec = 500000},
        .it_value = {.tv_usec = 500000}
    };
    setitimer(ITIMER_REAL, &timer, NULL);
}

// Остановка таймера
void stop_timer() {
    struct itimerval timer = {
        .it_interval = {.tv_usec = 0},
        .it_value = {.tv_usec = 0}
    };
    setitimer(ITIMER_REAL, &timer, NULL);
}