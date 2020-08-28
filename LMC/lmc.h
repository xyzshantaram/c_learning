#include <stdint.h>
#include <stdbool.h>

#define LMC_MEMORY_SIZE 100
#define DEBUG true
#pragma once

typedef struct mstate {
    unsigned int mem[LMC_MEMORY_SIZE];
    unsigned int program_counter;
    unsigned int accumulator;
    bool is_neg;
} lmc_state;

typedef enum op {
    HLT, ADD, SUB, STA, LDA, BRA, BRZ, BRP, INP, OUT, DAT
} Opcode;

extern const char* opcodes[];
extern const int opcode_count;

typedef struct instruction {
    int val;
    Opcode op;
} Instruction;

typedef uint8_t byte;

typedef void (*lmc_functions)(lmc_state*, byte);

void add(lmc_state* state, byte address);
void sub(lmc_state* state, byte address);
void sta(lmc_state* state, byte address);
void lda(lmc_state* state, byte address);
void bra(lmc_state* state, byte address);
void brz(lmc_state* state, byte address);
void brp(lmc_state* state, byte address);
void inp(lmc_state* state, byte address);
void out(lmc_state* state, byte address);

extern lmc_functions functions[];

Opcode opcode_from_string(char* str);
Instruction parse_input (char* str);
int get_input();