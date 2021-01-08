#ifndef VM_MACHINE_H
#define VM_MACHINE_H

#include <malloc.h>

#include "instructions.h"
#include "types.h"
#include "cpu.h"
#include "memory.h"
#include "status.h"

typedef struct machine {
    struct cpu* unit;
    struct memory* mem;
} machine_t, *machine_ptr;

enum vm_status vm_machine_create(struct machine** dst) {
    *dst = (struct machine*)malloc(sizeof(struct machine));
    
    vm_cpu_create(&(*dst)->unit);
    vm_memory_create(&(*dst)->mem, UINT8_MAX);

    return VM_OK;
}

enum vm_status vm_machine_load(struct machine* vm, u32 program[], u16 size) {
    return vm_cpu_program(vm->unit, program, size);
}

enum vm_status vm_machine_exec(struct machine* vm) {
    if(vm == NULL ||
      (vm->unit == NULL && 
       vm->mem == NULL)) {
        
        fprintf(stderr, "[ERROR] Machine wasn't initialized.\n");
        return VM_ERROR_NOT_INITIALIZED;
    }

    struct cpu_instruction instruction;
    while(vm_cpu_next(vm->unit, &instruction) == VM_OK) {
        //i32 source = vm->unit->reg[instruction.source];

        switch(instruction.operation) {
            case INSTRUCTION_HALT: {
                return VM_INFO_CPU_HALT;
            } break;

            case INSTRUCTION_MOV: {
                vm->unit->reg[instruction.destination] = instruction.immediate;
            } break;

            case INSTRUCTION_POP: {
                u64 value;
                vm_memory_pop(vm->mem, &value);

                vm->unit->reg[instruction.destination] = value;
            } break;

            case INSTRUCTION_PUSH: {
                vm_memory_push(vm->mem, instruction.destination);
            } break;

            case INSTRUCTION_SUB: {        
                i32 dest   = vm->unit->reg[instruction.destination];
                vm_memory_push(vm->mem, dest - instruction.immediate);
            } break;

            case INSTRUCTION_ADD: {        
                i32 dest   = vm->unit->reg[instruction.destination];
                vm_memory_push(vm->mem, dest + instruction.immediate);
            } break;
        };
    };

    return VM_OK;
}

void vm_machine_free(struct machine* vm) {
    if(vm != NULL && vm->unit != NULL)
        vm_cpu_free(vm->unit);
    
    if(vm != NULL && vm->mem != NULL)
        vm_memory_free(vm->mem);

    if(vm != NULL)
        free(vm);
}

#endif