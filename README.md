# Simple Computer Simulator

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Language](https://img.shields.io/badge/language-C-blue.svg)
![Build Status](https://img.shields.io/badge/build-passing-green.svg)

A low-level computer simulator written in C. This project implements a virtual machine with its own memory, CPU registers, and command set, along with a TUI (Text User Interface) console, a simple assembler (`sat`), and a simple basic compiler (`sbc`).

## Features

- **Virtual CPU**: Implements a custom instruction set (math, logic, I/O, control flow).
- **Memory Management**: 100-cell memory model.
- **TUI Console**: Visual interface to view registers, memory, and flags in real-time.
- **Custom Terminal Library**: Handling colors and cursor positioning (`myTerm`).
- **Big Characters**: Support for rendering large bitmapped characters (`myBigChars`).
- **Assembler**: Compiles assembly code (`.sa`) into object code (`.o`).
- **Basic Compiler**: Compiles a simplified Basic (`.sb`) into assembly language.

## Project Structure

- `mySimpleComputer/` - Core library (SC) implementing memory and CPU logic.
- `myTerm/` - Library for terminal control (colors, cursor).
- `myBigChars/` - Library for drawing large pseudo-graphic characters.
- `myReadKey/` - Library for handling keyboard input.
- `console/` - The main executable (TUI) integrating all libraries.
- `simpleassembler/` - Assembler (`sat`) source code.
- `simplebasic/` - Basic compiler (`sbc`) source code.
- `examples/` - Example programs (Basic code).

## Build

To build the project, simply run:

```bash
make
```

This will generate:
- `console/console` - The simulator application.
- `sat` - The assembler.
- `sbc` - The Basic compiler.

## Usage

### 1. Compile a Basic Program
Use the Basic compiler (`sbc`) to convert a `.sb` file to an assembly `.sa` file:

```bash
./sbc examples/factorial.sb factorial.sa
```

### 2. Assemble to Machine Code
Use the Assembler (`sat`) to convert the `.sa` file to an object file `.o` (machine code):

```bash
./sat factorial.sa factorial.o
```

### 3. Run the Simulator
Start the console application to run the virtual machine.
*Note: This requires a terminal window of sufficient size (at least 80x25).*

```bash
./console/console
```

Once inside the console, you can load the object file (if the load feature is implemented via UI or CLI arguments) or manually input commands.

## Cleaning Up
To remove build artifacts:

```bash
make clean
```

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Author
Иванов Артём Михайлович — ИС-342