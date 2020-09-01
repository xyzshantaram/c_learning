#include <stdlib.h>
#include <stdio.h>

#include "lmc.h"
#include "string_utils.h"

const char* opcodes[] = {
    "HLT", "ADD", "SUB", "STA", "LDA", "BRA", "BRZ", "BRP", "INP", "OUT", "DAT"
};

const int opcode_count = 11;

Instruction parse_input(char* str) {

    Instruction i = {
        .op = 0,
        .val = 0
    };
    int pos = -1;
    if ((pos = str_array_fuzzy_contains(opcodes, opcode_count, str)) != -1) {
        size_t index = 0;
        char** instr = split_on_delim(str, &index, ' ');
        if (instr) {
            i.op = pos;
            i.val = atoi(instr[1]);
        }
        free(instr[0]);
        free(instr[1]);
        free (instr);
    }
    return i;
}

void add(lmc_state* state, byte address) {
    byte address_bounded = address % LMC_MEMORY_SIZE;
    if (state->is_neg) {
        if ((state->accumulator *- 1) + state->mem[address_bounded] >= 0) {
            state->is_neg = false;
        }
    }

    if (DEBUG) {
        printf("\nADD acc %i and *%i %i sum: %i", state->accumulator, address_bounded, state->mem[address_bounded], state->accumulator + state->mem[address_bounded]);
    }
    state->accumulator += state->mem[address_bounded];
}

void sub(lmc_state* state, byte address) {
    byte address_bounded = address % LMC_MEMORY_SIZE;

    if (DEBUG) printf("\nSUB acc %i and *%i %i", state->accumulator, address_bounded, state->mem[address_bounded]);
    if (!state->is_neg) {
        state->accumulator -= state->mem[address_bounded];
    }
    else {
        state->accumulator += state->mem[address_bounded];
    }
    if (state->accumulator < 0) {
        state->accumulator *= -1;
        state->is_neg = true;
        if (DEBUG) printf("\nacc %i < 0. flip -> %i, NEG: %s", state->accumulator*-1, state->accumulator, (state->is_neg ? "true" : "false"));
    }
}
void sta(lmc_state* state, byte address) {
    byte address_bounded = address % LMC_MEMORY_SIZE;
    if (state->accumulator < 0) {
        state->accumulator *= -1;
        state->is_neg = true;
        if (DEBUG) printf("\nacc %i < 0. flip -> %i, NEG: %s", state->accumulator, state->accumulator*-1, (state->is_neg ? "true" : "false"));
    }
    if (DEBUG) printf("\nSTA %i from acc into %i", state->accumulator, address_bounded);
    state->mem[address_bounded] = state->accumulator;
}

void lda(lmc_state* state, byte address) {
    byte address_bounded = address % LMC_MEMORY_SIZE;
    if(DEBUG) printf("\nLDA %i into acc from %i", state->mem[address_bounded], address_bounded);
    state->accumulator = state->mem[address_bounded];
    state->is_neg = false;
    if (state->accumulator < 0) {
        if (DEBUG) printf("\nacc %i < 0. flip -> %i, NEG: %s", state->accumulator, state->accumulator*-1, (state->is_neg ? "true" : "false"));
        state->accumulator *= -1;
        state->is_neg = true;
    }
}

void bra(lmc_state* state, byte address) {
    byte address_bounded = address % LMC_MEMORY_SIZE;
    state->program_counter = address_bounded;
}

void brz(lmc_state* state, byte address) {
    byte address_bounded = address % LMC_MEMORY_SIZE;
    if (state->accumulator == 0) state->program_counter = address_bounded;
}

void brp(lmc_state* state, byte address) {
    byte address_bounded = address % LMC_MEMORY_SIZE;
    if (!state->is_neg) state->program_counter = address_bounded;
}

void inp(lmc_state* state, byte address) {
    printf("\nINP> ");
    char* str = read_string('\n');
    state->accumulator = atoi(str);
    state->is_neg = false;
    if (state->accumulator < 0) {
        if (DEBUG) printf("\nacc %i < 0. flip -> %i, NEG: %s", state->accumulator, state->accumulator*-1, (state->is_neg ? "true" : "false"));
        state->accumulator *= -1;
        state->is_neg = true;
    }
    free(str);
}

void out(lmc_state* state, byte address) {
    printf("\nOUT: %i", state->accumulator);
}

void hlt(lmc_state* state, byte address) {
    printf("\n\n======== HLT AT %zu ========\n", state->program_counter);
}

lmc_functions functions[] = {
    &hlt, &add, &sub, &sta, &lda, &bra, &brz, &brp, &inp, &out
};

Opcode opcode_from_string(char* str) {
    return str_array_fuzzy_contains(opcodes, opcode_count, str);
}