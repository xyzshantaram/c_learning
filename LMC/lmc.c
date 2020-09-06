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
    byte address_bounded = address/*  % LMC_MEMORY_SIZE */;
    if (DEBUG) {
        printf("\nADD acc %i and *%i %i sum: %i", state->accumulator, address_bounded, state->mem[address_bounded], state->accumulator + state->mem[address_bounded]);
    }
    state->accumulator += state->mem[address_bounded];

    	state->is_neg = state->accumulator > MEMORY_CELL_SIZE;
	    if (state->is_neg) state->accumulator -= MEMORY_CELL_SIZE + 1;
}

void sub(lmc_state* state, byte address) {
    byte address_bounded = address/*  % LMC_MEMORY_SIZE */;
    if (DEBUG) printf("\nSUB acc %i and *%i %i", state->accumulator, address_bounded, state->mem[address_bounded]);
    state->accumulator -= state->mem[address_bounded];
    state->is_neg = state->accumulator < 0;
	if (state->is_neg)
		state->accumulator += LMC_MEMORY_SIZE + 1;
}
void sta(lmc_state* state, byte address) {
    byte address_bounded = address/*  % LMC_MEMORY_SIZE */;
    if (DEBUG) printf("\nSTA %i from acc into %i", state->accumulator, address_bounded);
    state->mem[address_bounded] = state->accumulator;
}

void lda(lmc_state* state, byte address) {
    byte address_bounded = address/*  % LMC_MEMORY_SIZE */;
    if(DEBUG) printf("\nLDA %i into acc from %i", state->mem[address_bounded], address_bounded);
    state->accumulator = state->mem[address_bounded];
}

void bra(lmc_state* state, byte address) {
    byte address_bounded = address/*  % LMC_MEMORY_SIZE */;
    state->program_counter = address_bounded;
}

void brz(lmc_state* state, byte address) {
    byte address_bounded = address/*  % LMC_MEMORY_SIZE */;
    if (state->accumulator == 0) state->program_counter = address_bounded;
}

void brp(lmc_state* state, byte address) {
    byte address_bounded = address/*  % LMC_MEMORY_SIZE */;
    if (!state->is_neg) state->program_counter = address_bounded;
}

void inp(lmc_state* state, byte address) {
    printf("\033[0;32m");
    printf("\nINP> ");
    char* str = read_string('\n');
    state->accumulator = atoi(str);
    printf("\033[0m");
    free(str);
}

void out(lmc_state* state, byte address) {
    printf("\033[0;32m");
    printf("\nOUT: %i", state->accumulator);
    printf("\033[0m");
}

void hlt(lmc_state* state, byte address) {
    printf("\033[0;32m");
    printf("\n\n======== HLT AT %zu ========\n", state->program_counter);
    printf("\033[0m");
}

lmc_functions functions[] = {
    &hlt, &add, &sub, &sta, &lda, &bra, &brz, &brp, &inp, &out
};

Opcode opcode_from_string(char* str) {
    return str_array_fuzzy_contains(opcodes, opcode_count, str);
}
