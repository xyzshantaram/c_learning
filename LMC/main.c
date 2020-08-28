#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "string_utils.h"
#include "lmc.h"

unsigned int first_number(unsigned int num);

extern lmc_functions functions[];
extern const char* opcodes[];

int main (int argc, char* argv[]) {

    if (argc < 2) {
        printf("Please provide a filename containing LMC assembly.");
    }
    else {

        // ================================= Load assembly =================================

        char* file = file_into_str(argv[1]);
        if (!file) {
            return 1;
        }

        size_t count = 0;
        char** file_lines = split_on_delim(file, &count, '\n');
        
        free(file);

        if (!file_lines) {
            return 1;
        }

        // ==================================== LMC vars =====================================

        lmc_state state = {
            .mem = {0},
            .program_counter = 0,
            .accumulator = 0,
            .is_neg = false
        };

        Instruction* instructions = malloc(count * sizeof(Instruction));

        if (!instructions) {
            return 1;
        }

        // ================================ Load into memory =================================

        printf("\n========= LOADING =========");
        for (size_t x = 0; x < count; x++) {
            instructions[x] = parse_input(file_lines[x]);
            
            if (instructions[x].op == 10) state.mem[x] = instructions[x].val;
            else state.mem[x] = (instructions[x].op * 1000) + instructions[x].val;

            printf("\nstored %i in address %i", state.mem[x], x);
        }
        printf("\n\n====== LOADED, BEGIN ======\n");

        // ================================= LMC execution  =================================

        do {
            int current_mem_cell = state.mem[state.program_counter];
            Opcode inst = instructions[state.program_counter].op;
            if (inst <= OUT) {
                int val = current_mem_cell - (inst * 1000);
                (*functions[inst])(&state, val);
                if (DEBUG) printf("\n executed instruction %s %i, program counter at %i", opcodes[inst], val, state.program_counter);
            }
            else {
            }
            if (/* inst != BRZ &&  */inst != BRP && inst != BRA) state.program_counter ++;
            if (inst == HLT) break;

        } while(state.program_counter != LMC_MEMORY_SIZE - 1);


        // ===================================== CLEANUP =====================================
        for (size_t x = 0; x < count; x++) {
            free(file_lines[x]);
        }
        free(file_lines);
        free(instructions);
    }
    return 0;
}

unsigned int first_number(unsigned int num) {
    unsigned int temp = num;
    while (temp >= 10) {
        temp /= 10;
    }
    return temp;
}