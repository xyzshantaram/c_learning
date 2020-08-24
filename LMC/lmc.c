#include <stdlib.h>
#include <stdio.h>

#include "lmc.h"
#include "string_utils.h"

const char* opcodes[] = {
    "ADD", "SUB", "STA", "LDA", "BRA", "BRZ", "BRP", "INP", "OUT", "HLT"
};
const int opcode_count = 10;

Instruction parse_input(char* str) {

    Instruction i;
    int pos = -1;
    if ((pos = str_array_fuzzy_contains(opcodes, opcode_count, str)) != -1) {
        size_t index = 0;
        char** instr = split_on_delim(str, &index, ' ');
        i.op = pos;
        i.val = atoi(instr[1]);
    }
    return i;
}

void add(lmc_state* state, byte address) {
    byte address_bounded = address % LMC_MEMORY_SIZE;
    state->accumulator += state->mem[address_bounded];
}

void sub(lmc_state* state, byte address) {
    byte address_bounded = address % LMC_MEMORY_SIZE;
    state->accumulator -= state->mem[address_bounded];
}

void sta(lmc_state* state, byte address) {
    byte address_bounded = address % LMC_MEMORY_SIZE;
    state->mem[address_bounded] = state->accumulator;
}

void lda(lmc_state* state, byte address) {
    byte address_bounded = address % LMC_MEMORY_SIZE;
    state->accumulator = state->mem[address_bounded];
}

void bra(lmc_state* state, byte address) {
    byte address_bounded = address % LMC_MEMORY_SIZE;
    state->program_counter = state->mem[address_bounded];
}

void brz(lmc_state* state, byte address) {
    byte address_bounded = address % LMC_MEMORY_SIZE;
    if (state->accumulator == 0) state->program_counter = state->mem[address_bounded];
}

void brp(lmc_state* state, byte address) {
    byte address_bounded = address % LMC_MEMORY_SIZE;
    if (state->accumulator > 0) state->program_counter = state->mem[address_bounded];
}

void inp(lmc_state* state, byte address) {
    printf("\nINP> ");
    char* str = read_string('\n');
    state->accumulator = atoi(str);
    printf("Accumulator set to: %i", state->accumulator);
}

void out(lmc_state* state, byte address) {
    printf("OUT: %i", state->accumulator);
}

byte next_instruction(lmc_state* state) {
    
}

void exec_cell(lmc_state* state) {
    
}

lmc_functions functions[] = {
    &add, &sub, &sta, &lda, &bra, &brz, &brp, &inp, &out
};

Opcode opcode_from_string(char* str) {
    return str_array_fuzzy_contains(opcodes, opcode_count, str);
}