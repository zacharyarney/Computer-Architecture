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
void cpu_load(struct cpu *cpu)
{
    char data[DATA_LEN] = {
        // From print8.ls8
        0b10000010, // LDI R0,8
        0b00000000,
        0b00001000,
        0b01000111, // PRN R0
        0b00000000,
        0b00000001  // HLT
    };

    int address = 0;

    for (int i = 0; i < DATA_LEN; i++) {
        cpu->ram[address++] = data[i];
    }

    // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
    switch (op) {
        case ALU_MUL:
            // TODO
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
        printf("Registers:\n[ ");
        for (int i = 0; i<8; i++){
            printf("%d ", cpu->registers[i]);
        }
        printf("]\n");
        // TODO
        // 1. Get the value of the current instruction (in address PC).
        // 2. Figure out how many operands this next instruction requires
        // 3. Get the appropriate value(s) of the operands following this instruction
        // 4. switch() over it to decide on a course of action.
        // 5. Do whatever the instruction should do according to the spec.
        // 6. Move the PC to the next instruction.
        unsigned int IR = cpu_ram_read(cpu, cpu->PC);
        unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
        unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);

        switch(IR) {
            case LDI:
                printf("LDI:\nIR: %c, operandA: %c, operandB: %c\n", IR, operandA, operandB);
                cpu_ram_write(cpu, operandA, operandB);
                IR += 3;
                break;
            case PRN:
                printf("PRN: %c\n", cpu_ram_read(cpu, operandA));
                IR += 2;
                break;
            case HLT:
                printf("HALTING (HLT)");
                running = 0;
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
    memset(cpu->registers, 0, sizeof(cpu->registers));
    memset(cpu->ram, 0, sizeof(cpu->ram));
}
