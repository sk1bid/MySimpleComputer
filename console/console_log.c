#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* log_terminal_file = NULL;

// Инициализурет внешний терминал по его пути (напиши tty в консоли)
int init_log_terminal(const char* terminal_device_path)
{
    log_terminal_file = fopen(terminal_device_path, "w");
    if (log_terminal_file == NULL) {
        fprintf(stderr,
                "Ошибка: не удалось открыть терминал %s для логов: %s\n",
                terminal_device_path,
                strerror(errno));
        return -1;
    }
    setvbuf(log_terminal_file, NULL, _IONBF, 0);
    return 0; // Возвращаем код успеха
}

// Функция для вывода лог-сообщения
void log_message(const char* format, ...)
{
    if (log_terminal_file != NULL) {
        va_list args;
        va_start(args, format);
        vfprintf(log_terminal_file, format, args);
        va_end(args);
    }
}

// Функция для закрытия файла логов при завершении программы
void close_log_terminal()
{
    if (log_terminal_file != NULL) {
        fclose(log_terminal_file);
        log_terminal_file = NULL;
    }
}