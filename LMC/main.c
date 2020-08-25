#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "string_utils.h"
#include "lmc.h"

unsigned int first_number(unsigned int num);

extern lmc_functions functions[];

int main (int argc, char* argv[]) {

    lmc_state state = {
        .mem = {9000},
        .program_counter = 0,
        .accumulator = 0
    };

    if (argc < 2) {
        printf("Please provide a filename containing LMC assembly.");
    }
    else {
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

        Instruction* instructions = malloc(count * sizeof(Instruction));

        if (!instructions) {
            return 1;
        }
        printf("\n========= LOADING =========");
        for (size_t x = 0; x < count; x++) {
            instructions[x] = parse_input(file_lines[x]);
            state.mem[x] = (instructions[x].op * 1000) + instructions[x].val;
            printf("\nstored %i in address %i", state.mem[x], x);
        }
        printf("\n====== LOADED, BEGIN ======\n");
        do {
            int current_mem_cell = state.mem[state.program_counter];
            Opcode inst = 0;
            if (current_mem_cell > 999) {
                inst = first_number(current_mem_cell);
            }

            if (inst == HLT) {
                printf("\n======== HLT AT %zu ========\n", state.program_counter);
                break;
            }
            
            int val = current_mem_cell - (inst * 1000);
            (*functions[inst])(&state, val);
            state.program_counter++;
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