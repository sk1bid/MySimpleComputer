# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -g

# Директории и файлы
SRC_DIR = .
CONSOLE_SRC_DIR = console
INCLUDE_DIR = include

# Имя исполняемого файла
CONSOLE_EXEC = test

# Исходные файлы
CONSOLE_SRC = $(CONSOLE_SRC_DIR)/test.c

# Объектные файлы
CONSOLE_OBJ = $(CONSOLE_SRC:.c=.o)

# Цель по умолчанию
all: $(CONSOLE_EXEC)

# Компиляция объектных файлов
$(CONSOLE_OBJ): $(CONSOLE_SRC)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Сборка исполняемого файла
$(CONSOLE_EXEC): $(CONSOLE_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# Очистка
clean:
	rm -f $(CONSOLE_OBJ) $(CONSOLE_EXEC)

.PHONY: all clean