CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
INCLUDE_DIR = include
MYSC_LIB_DIR = mySimpleComputer
MYSC_LIB_NAME = mySimpleComputer
MYSC_LIB = $(MYSC_LIB_DIR)/lib$(MYSC_LIB_NAME).a

MYSC_SRC = $(wildcard $(MYSC_LIB_DIR)/sc_*.c)

MYSC_OBJ = $(patsubst $(MYSC_LIB_DIR)/%.c, $(MYSC_LIB_DIR)/%.o, $(MYSC_SRC))


CONSOLE_SRC = $(wildcard console/io_*.c)

CONSOLE_IO_OBJ = $(patsubst console/%.c, console/%.o, $(CONSOLE_SRC))


PR01_SRC = console/main.c

PR01_OBJ = $(patsubst %.c, %.o, $(PR01_SRC))

all: pr01

$(MYSC_LIB): $(MYSC_OBJ)
	@mkdir -p $(MYSC_LIB_DIR) # Ensure library directory exists
	ar rcs $@ $^
	@echo "Static library created: $(MYSC_LIB)"

pr01: $(PR01_OBJ) $(CONSOLE_IO_OBJ) $(MYSC_LIB)
	$(CC) $(CFLAGS) -o $@ $(PR01_OBJ) $(CONSOLE_IO_OBJ) -L$(MYSC_LIB_DIR) -l$(MYSC_LIB_NAME)
	@echo "Executable created: pr01"

$(MYSC_LIB_DIR)/%.o: $(MYSC_LIB_DIR)/%.c $(MYSC_LIB_DIR)/%.h $(INCLUDE_DIR)/mySimpleComputer.h
	$(CC) $(CFLAGS) -c -o $@ $<

console/%.o: console/%.c console/io.h $(INCLUDE_DIR)/mySimpleComputer.h $(MYSC_LIB_DIR)/sc_memory.h $(MYSC_LIB_DIR)/sc_reg.h $(MYSC_LIB_DIR)/sc_command.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(PR01_OBJ): $(PR01_SRC) include/mySimpleComputer.h console/io.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f pr01 $(PR01_OBJ) console/*.o $(MYSC_LIB) $(MYSC_OBJ) $(MYSC_LIB_DIR)/*.o
	@echo "Cleaned project files"

format:
	find . -type f -name "*.c" -o -name "*.h" | xargs clang-format -i
	@echo "Code formatted with clang-format using .clang-format"
	
.PHONY: all clean