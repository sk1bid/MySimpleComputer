#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 128

// Define opcodes for all commands
#define CMD_NOP     0x00
#define CMD_CPUINFO 0x01
#define CMD_READ    0x0A
#define CMD_WRITE   0x0B
#define CMD_LOAD    0x14
#define CMD_STORE   0x15
#define CMD_ADD     0x1E
#define CMD_SUB     0x1F
#define CMD_DIVIDE  0x20
#define CMD_MUL     0x21
#define CMD_AND     0x34
#define CMD_MUL2    0x50
#define CMD_DIV2    0x51
#define CMD_JUMP    0x28
#define CMD_JNEG    0x29
#define CMD_HALT    0x2B
#define CMD_NOT     0x33

int encode_command(const char* mnemonic, int operand)
{
    int opcode = -1;
    if (strcmp(mnemonic, "NOP") == 0) opcode = CMD_NOP;
    else if (strcmp(mnemonic, "CPUINFO") == 0) opcode = CMD_CPUINFO;
    else if (strcmp(mnemonic, "READ") == 0) opcode = CMD_READ;
    else if (strcmp(mnemonic, "WRITE") == 0) opcode = CMD_WRITE;
    else if (strcmp(mnemonic, "LOAD") == 0) opcode = CMD_LOAD;
    else if (strcmp(mnemonic, "STORE") == 0) opcode = CMD_STORE;
    else if (strcmp(mnemonic, "ADD") == 0) opcode = CMD_ADD;
    else if (strcmp(mnemonic, "SUB") == 0) opcode = CMD_SUB;
    else if (strcmp(mnemonic, "DIVIDE") == 0) opcode = CMD_DIVIDE;
    else if (strcmp(mnemonic, "MUL") == 0) opcode = CMD_MUL;
    else if (strcmp(mnemonic, "AND") == 0) opcode = CMD_AND;
    else if (strcmp(mnemonic, "MUL2") == 0) opcode = CMD_MUL2;
    else if (strcmp(mnemonic, "DIV2") == 0) opcode = CMD_DIV2;
    else if (strcmp(mnemonic, "JUMP") == 0) opcode = CMD_JUMP;
    else if (strcmp(mnemonic, "JNEG") == 0) opcode = CMD_JNEG;
    else if (strcmp(mnemonic, "HALT") == 0) opcode = CMD_HALT;
    else if (strcmp(mnemonic, "NOT") == 0) opcode = CMD_NOT;

    if (opcode == -1) {
        fprintf(stderr, "Invalid mnemonic: %s\n", mnemonic);
        return -1;
    }
    return (opcode << 7) | (operand & 0x7F);
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Usage: sat input.sa output.o\n");
        return 1;
    }

    FILE* in = fopen(argv[1], "r");
    FILE* out = fopen(argv[2], "wb");
    if (!in || !out) {
        fprintf(stderr, "Error opening files\n");
        fclose(in);
        fclose(out);
        return 1;
    }

    int memory[MEMORY_SIZE] = {0}; // Initialize all memory to 0
    char line[256];

    while (fgets(line, sizeof(line), in)) {
        char* comment_pos = strchr(line, ';');
        if (comment_pos) *comment_pos = '\0';

        char addr_str[10], token[10], operand_str[10];
        int addr;

        if (sscanf(line, "%s %s %s", addr_str, token, operand_str) == 3) {
            addr = atoi(addr_str);
            if (addr < 0 || addr >= MEMORY_SIZE) {
                fprintf(stderr, "Invalid address: %d\n", addr);
                continue;
            }

            if (strcmp(token, "=") == 0) {
                int value;
                if (sscanf(operand_str, "%d", &value) == 1) {
                    memory[addr] = value;
                } else {
                    fprintf(stderr, "Invalid value for address %d: %s\n", addr, operand_str);
                }
            } else {
                int operand;
                if (sscanf(operand_str, "%d", &operand) == 1 && operand >= 0 && operand <= 127) {
                    int encoded = encode_command(token, operand);
                    if (encoded != -1) {
                        memory[addr] = encoded;
                    } else {
                        fprintf(stderr, "Invalid mnemonic: %s\n", token);
                    }
                } else {
                    fprintf(stderr, "Invalid operand: %s\n", operand_str);
                }
            }
        }
    }

    fwrite(memory, sizeof(int), MEMORY_SIZE, out);
    fclose(in);
    fclose(out);
    return 0;
}