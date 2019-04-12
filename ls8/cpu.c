#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address) {
    return cpu->ram[address];
}

// not sure if this is meant to return anything. void for now.
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char val) {
    cpu->ram[address] = val;
    // return cpu->ram[address];
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
int cpu_load(struct cpu *cpu, int argc, char *argv)
{
    FILE *fp;
    char line[1024];
    int address = 0;

    if (argc != 2) {
        printf("USE FORMAT: ./file file_name.extension\n");
        return 1;
    }

    fp = fopen(argv, "r");

    if (fp == NULL) {
        printf("Cannot open file\n");
        return 2;
    }

    while (fgets(line, 1024, fp) != NULL) {
        char *endptr;
        unsigned char val = strtoul(line, &endptr, 2);

        if (line == endptr) {
            continue;
        }
        cpu->ram[address++] = val;
    }

    fclose(fp);
    return 0;
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
    switch (op) {
        case ALU_MUL:
            // TODO
            // turns out `*=` is a thing
            cpu->registers[regA] *= cpu->registers[regB];
            break;
        case ALU_CMP:
            if (cpu->registers[regA] == cpu->registers[regB]) {
                cpu->FL = cpu->FL | 0b00000001;
            } else {
                cpu->FL = cpu->FL & 0b11111110;
            }
            if (cpu->registers[regA] > cpu->registers[regB]) {
                cpu->FL = cpu->FL | 0b00000010;
            } else {
                cpu->FL = cpu->FL & 0b11111101;
            }
            if (cpu->registers[regA] < cpu->registers[regB]) {
                cpu->FL = cpu->FL | 0b00000100;
            } else {
                cpu->FL = cpu->FL & 0b11111011;
            }
            break;
            // TODO: implement more ALU ops
    }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
    int running = 1; // True until we get a HLT instruction

    while (running) {
        // printf("Registers:\n[ ");
        // for (int i = 0; i<8; i++){
        //     printf("%d ", cpu->registers[i]);
        // }
        // printf("]\n");

        // TODO
        // 1. Get the value of the current instruction (in address PC).
        // 2. Figure out how many operands this next instruction requires
        // 3. Get the appropriate value(s) of the operands following this instruction
        // 4. switch() over it to decide on a course of action.
        // 5. Do whatever the instruction should do according to the spec.
        // 6. Move the PC to the next instruction.
        unsigned char IR = cpu_ram_read(cpu, cpu->PC);
        unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
        unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);

        switch(IR) {
            case LDI:
                // printf("LDI:\nIR: %d, operandA: %d, operandB: %d\n", IR, operandA, operandB);
                cpu->registers[operandA] = operandB;
                // need to increment PC because IR is just the value at PC
                cpu->PC += 3;
                break;
            case PRN:
                printf("%d\n", cpu->registers[operandA]);
                cpu->PC += 2;
                break;
            case MUL:
                alu(cpu, ALU_MUL, operandA, operandB);
                cpu->PC += 3;
                break;
            case CMP:
                alu(cpu, ALU_CMP, operandA, operandB);
                cpu->PC += 3;
                break;
            case JMP:
                cpu->PC = cpu->registers[operandA];
                break;
            case PUSH:
                cpu->registers[7]--;
                cpu->ram[cpu->registers[7]] = cpu->registers[operandA];
                cpu->PC += 2;
                break;
            case POP:
                cpu->registers[operandA] = cpu->ram[cpu->registers[7]];
                cpu->registers[7]++;
                cpu->PC += 2;
                break;
            case HLT:
                // printf("HALTING (HLT)\n");
                running = 0;
                break;
            default:
                printf("Bad instruction\n");
                exit(1);
        }
    }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
    // TODO: Initialize the PC and other special registers
    cpu->PC = 0;
    cpu->FL = 0;
    memset(cpu->registers, 0, sizeof(cpu->registers));
    memset(cpu->ram, 0, sizeof(cpu->ram));
    cpu->registers[7] = 0xF4;
}
