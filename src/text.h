#ifndef VM_TEXT_H
#define VM_TEXT_H

#include <string.h>
#include <stdio.h>

#include "memory.h"
#include "cpu.h"

#define BITS(v,n,buf) for(int cb=0; cb < n; cb++){                  \
                            if(cb % 8 == 0 && cb != 0)              \
                                strcat(buf, " ");                   \
                            if(v & ((0x00000001u << (n - 1u)) >> cb)) \
                                strcat(buf, "1");                   \
                            else                                    \
                                strcat(buf, "0");                   \
                         }                                          \

void vm_print_instruction(const struct vm_cpu_instruction instruction) {
    char imm[16] = "";
    char flags[256] = "";

    const char* operators[] = {
        "HALT",
        "ADD",
        "SUB",
        "DIV",
        "MUL",
        "CMP",
        "MOV",
        "PUSH",
        "POP",
    };

    const char* registers[] = {
        "NONE",
        "GPA",
        "GPB",
        "GPC",
        "GPD",
        "GPE",
        "GPF",
        "GPG",
        "GPH",
        "GPI",
    };

    if(instruction.flags & CPU_FLAG_ZERO)
        strcat(flags, "ZERO |");
    if(instruction.flags & CPU_FLAG_POSITIVE)
        strcat(flags, "POSITIVE |");
    if(instruction.flags & CPU_FLAG_NEGATIVE)
        strcat(flags, "NEGATIVE |");
    if(instruction.flags & CPU_FLAG_FLOATING)
        strcat(flags, "FLOATING |");
    
    BITS(instruction.immediate, 12, imm);
    fprintf(stdout, "\nOP:\t%s\nFLGS:\t%s\nDR:\t%s\nSR:\t%s\nIMM12:\t%s (%uu)\n", operators[instruction.operation], flags, registers[instruction.destination], registers[instruction.source], imm, instruction.immediate);
}

void vm_print_memory(struct memory* mem) {
    fprintf(stdout, "\n");
    for(int i = 0; i < mem->size; i++) {
        char bits[64] = "";
        BITS(*(mem->stack + i), 32, bits);

        u8 b0 = (*(mem->stack + i)) & 0xFF000000;
        u8 b1 = (*(mem->stack + i)) & 0x00FF0000;
        u8 b2 = (*(mem->stack + i)) & 0x0000FF00;
        u8 b3 = (*(mem->stack + i)) & 0x000000FF;

        fprintf(stdout, "0x%02X\t => 0x%02X 0x%02X 0x%02X 0x%02X (%uu)\n", i, b0, b1, b2, b3, *(mem->stack + i));
    };
}

#endif