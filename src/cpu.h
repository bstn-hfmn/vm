#ifndef VM_CPU_H
#define VM_CPU_H

#include <string.h>
#include <malloc.h>

#include "types.h"
#include "status.h"
#include "registers.h"

typedef struct cpu_instruction {
    u32 original;
    u8 operation;

    u8 source;
    u8 destination;

    u16 immediate;
} cpu_instruction_t;

typedef struct cpu {
    u32 pc;
    
    u16 size;
    u32* program;

    u32 reg[REGISTERS];
} cpu_t, *cpu_ptr;

enum vm_status vm_cpu_create(struct cpu** dst) {
    *dst = (struct cpu*)malloc(sizeof(struct cpu));

    (*dst)->pc = ZERO;
    (*dst)->size = ZERO;
    (*dst)->program = NULL;
    
    (*dst)->reg[REGISTER_GPA] = ZERO;
    (*dst)->reg[REGISTER_GPB] = ZERO;
    (*dst)->reg[REGISTER_GPC] = ZERO;
    (*dst)->reg[REGISTER_GPD] = ZERO;
    (*dst)->reg[REGISTER_GPE] = ZERO;
    (*dst)->reg[REGISTER_GPF] = ZERO;
    (*dst)->reg[REGISTER_GPG] = ZERO;
    (*dst)->reg[REGISTER_GPH] = ZERO;
    (*dst)->reg[REGISTER_GPI] = ZERO;

    return VM_OK;
}

enum vm_status vm_cpu_program(struct cpu* unit, u32* program, u16 size) {
    if(program == NULL)
        return VM_ERROR_INVALID_PROGRAM;
    if(unit->program != NULL)
        return VM_ERROR_HAS_PROGRAM;

    unit->pc      = ZERO;
    unit->size    = size;
    unit->program = (u32*)malloc(sizeof(u32) * size);
    memcpy(unit->program, program, sizeof(u32) * size);

    return VM_OK;
}

enum vm_status vm_cpu_next(struct cpu* unit, struct cpu_instruction* inst) {
    if(unit->pc == unit->size)
        return VM_WARN_CPU_NO_INSTRUCTIONS;

    inst->original = *(unit->program + (unit->pc++));

    inst->operation   = (inst->original & 0xFF000000) >> 24;
    inst->destination = (inst->original & 0x00F00000) >> 20;
    inst->source      = (inst->original & 0x000F0000) >> 16; 
    
    inst->immediate   = (inst->original & 0x0000FFFF) >> 0;
    return VM_OK;
}

void vm_cpu_free(struct cpu* unit) {
    if(unit == NULL)
        return;

    if(unit->program != NULL)
        free(unit->program);
    
    free(unit);
}

#endif