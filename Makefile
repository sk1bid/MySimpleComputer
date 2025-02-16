CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
INCLUDE_DIR = include
MYSC_LIB_DIR = mySimpleComputer
MYSC_LIB_NAME = mySimpleComputer
MYSC_LIB = $(MYSC_LIB_DIR)/lib$(MYSC_LIB_NAME).a

# --- Source and Object Files ---

# Library mySimpleComputer source files (all sc_*.c in mySimpleComputer directory)
MYSC_SRC = $(wildcard $(MYSC_LIB_DIR)/sc_*.c)
# Library mySimpleComputer object files (.o files in mySimpleComputer directory)
MYSC_OBJ = $(patsubst $(MYSC_LIB_DIR)/%.c, $(MYSC_LIB_DIR)/%.o, $(MYSC_SRC))

# Console (io) source files (all io_*.c in console directory)
CONSOLE_SRC = $(wildcard console/io_*.c)
# Console (io) object files (.o files in console directory)
CONSOLE_IO_OBJ = $(patsubst console/%.c, console/%.o, $(CONSOLE_SRC))

# pr01.c is now in the root directory
PR01_SRC = console/main.c
# pr01.o object file in the root directory
PR01_OBJ = $(patsubst %.c, %.o, $(PR01_SRC))

# --- Build Targets ---

# Default target: build pr01 executable
all: pr01

# Static library target: libmySimpleComputer.a
$(MYSC_LIB): $(MYSC_OBJ)
	@mkdir -p $(MYSC_LIB_DIR) # Ensure library directory exists
	ar rcs $@ $^
	@echo "Static library created: $(MYSC_LIB)"

# Executable target: pr01
pr01: $(PR01_OBJ) $(CONSOLE_IO_OBJ) $(MYSC_LIB)
	$(CC) $(CFLAGS) -o $@ $(PR01_OBJ) $(CONSOLE_IO_OBJ) -L$(MYSC_LIB_DIR) -l$(MYSC_LIB_NAME)
	@echo "Executable created: pr01"

# --- Compilation Rules ---

# Rule to compile mySimpleComputer library source files (.c to .o in mySimpleComputer directory)
$(MYSC_LIB_DIR)/%.o: $(MYSC_LIB_DIR)/%.c $(MYSC_LIB_DIR)/%.h $(INCLUDE_DIR)/mySimpleComputer.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Rule to compile console (io) source files (.c to .o in console directory)
console/%.o: console/%.c console/io.h $(INCLUDE_DIR)/mySimpleComputer.h $(MYSC_LIB_DIR)/sc_memory.h $(MYSC_LIB_DIR)/sc_reg.h $(MYSC_LIB_DIR)/sc_command.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Rule to compile pr01.c in the root directory (.c to .o in the root directory)
$(PR01_OBJ): $(PR01_SRC) include/mySimpleComputer.h console/io.h
	$(CC) $(CFLAGS) -c -o $@ $<

# --- Utility Targets ---

# Clean target: remove all generated files
clean:
	rm -f pr01 $(PR01_OBJ) console/*.o $(MYSC_LIB) $(MYSC_OBJ) $(MYSC_LIB_DIR)/*.o
	@echo "Cleaned project files"

# Phony targets (not actual files)
.PHONY: all clean