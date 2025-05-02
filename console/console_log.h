#ifndef CONSOLE_LOG_H
#define CONSOLE_LOG_H

int init_log_terminal(const char* terminal_device_path);
void log_message(const char* format, ...);
void close_log_terminal(void);
#endif