# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -g

# Директории исходных файлов
SRC_DIR = .
CONSOLE_SRC_DIR = console
INCLUDE_DIR = include

# Имена исполняемых файлов
CONSOLE_EXEC = test

# Исходные файлы
CONSOLE_SRC = $(CONSOLE_SRC_DIR)/test.c

# Объектные файлы
CONSOLE_OBJ = $(CONSOLE_SRC:.c=.o)

# Цель по умолчанию
all: $(CONSOLE_EXEC)

# Компиляция исходного кода в объектные файлы
$(CONSOLE_SRC_DIR)/%.o: $(CONSOLE_SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Сборка объектных файлов в исполняемый файл
$(CONSOLE_EXEC): $(CONSOLE_OBJ)
	$(CC) $(CFLAGS) $^ -o $@


# Правило clean
clean:
	rm -f $(CONSOLE_OBJ) $(CONSOLE_EXEC)

.PHONY: all clean