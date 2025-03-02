# Компилятор и флаги
CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -Iinclude -ImyTerm -ImySimpleComputer -ImyBigChars -Iconsole

# Каталоги
INCLUDE_DIR      = include
MYSC_LIB_DIR     = mySimpleComputer
MYTERM_LIB_DIR   = myTerm
MYBIGCHARS_LIB_DIR = myBigChars
CONSOLE_DIR      = console

# Параметры библиотеки mySimpleComputer
MYSC_LIB_NAME    = mySimpleComputer
MYSC_LIB         = $(MYSC_LIB_DIR)/lib$(MYSC_LIB_NAME).a
MYSC_SRC         = $(wildcard $(MYSC_LIB_DIR)/sc_*.c)
MYSC_OBJ         = $(patsubst $(MYSC_LIB_DIR)/%.c, $(MYSC_LIB_DIR)/%.o, $(MYSC_SRC))

# Параметры библиотеки myTerm
MYTERM_LIB_NAME  = myTerm
MYTERM_LIB       = $(MYTERM_LIB_DIR)/lib$(MYTERM_LIB_NAME).a
MYTERM_SRC       = $(wildcard $(MYTERM_LIB_DIR)/mt_*.c)
MYTERM_OBJ       = $(patsubst $(MYTERM_LIB_DIR)/%.c, $(MYTERM_LIB_DIR)/%.o, $(MYTERM_SRC))

# Параметры библиотеки myBigChars
MYBIGCHARS_LIB_NAME  = myBigChars
MYBIGCHARS_LIB       = $(MYBIGCHARS_LIB_DIR)/lib$(MYBIGCHARS_LIB_NAME).a
MYBIGCHARS_SRC       = $(wildcard $(MYBIGCHARS_LIB_DIR)/bc_*.c)
MYBIGCHARS_OBJ       = $(patsubst $(MYBIGCHARS_LIB_DIR)/%.c, $(MYBIGCHARS_LIB_DIR)/%.o, $(MYBIGCHARS_SRC))

# Исходники и объектники для консоли
CONSOLE_SRC      = $(wildcard $(CONSOLE_DIR)/*.c)
CONSOLE_OBJ      = $(patsubst $(CONSOLE_DIR)/%.c, $(CONSOLE_DIR)/%.o, $(CONSOLE_SRC))

# Цель по умолчанию
all: $(MYSC_LIB) $(MYTERM_LIB) $(MYBIGCHARS_LIB) $(CONSOLE_DIR)/console

# Сборка библиотеки mySimpleComputer
$(MYSC_LIB): $(MYSC_OBJ)
	ar rcs $@ $^

$(MYSC_LIB_DIR)/%.o: $(MYSC_LIB_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Сборка библиотеки myTerm
$(MYTERM_LIB): $(MYTERM_OBJ)
	ar rcs $@ $^

$(MYTERM_LIB_DIR)/%.o: $(MYTERM_LIB_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Сборка библиотеки myBigChars
$(MYBIGCHARS_LIB): $(MYBIGCHARS_OBJ)
	ar rcs $@ $^

$(MYBIGCHARS_LIB_DIR)/%.o: $(MYBIGCHARS_LIB_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<


# Компиляция исходников консоли
$(CONSOLE_DIR)/%.o: $(CONSOLE_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Линковка исполняемого файла консоли
$(CONSOLE_DIR)/console: $(CONSOLE_OBJ) $(MYSC_LIB) $(MYTERM_LIB) $(MYBIGCHARS_LIB)
	$(CC) -o $@ $(filter-out $(CONSOLE_DIR)/font.o, $(CONSOLE_OBJ))  -L$(MYSC_LIB_DIR) -l$(MYSC_LIB_NAME) -L$(MYTERM_LIB_DIR) -l$(MYTERM_LIB_NAME) -L$(MYBIGCHARS_LIB_DIR) -l$(MYBIGCHARS_LIB_NAME) # Excluded font.o and added explicit dependency

# Правило очистки
clean:
	rm -f $(CONSOLE_DIR)/console $(CONSOLE_DIR)/*.o \
	      $(MYSC_LIB) $(MYSC_LIB_DIR)/*.o \
	      $(MYTERM_LIB) $(MYTERM_LIB_DIR)/*.o \
		  $(MYBIGCHARS_LIB) $(MYBIGCHARS_LIB_DIR)/*.o # Corrected library name in clean

format:
	find . -type f \( -name "*.c" -o -name "*.h" \) | xargs clang-format -i
	@echo "Code formatted with clang-format"