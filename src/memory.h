#ifndef VM_MEMORY_H
#define VM_MEMORY_H

#include <string.h>
#include <malloc.h>

#include "types.h"
#include "status.h"

typedef struct memory {
    u32* stack;

    u16 top;
    u16 size;
} memory_t, *memory_ptr;

enum vm_status vm_memory_create(struct memory** dst, u16 size) {
    *dst = (struct memory*)malloc(sizeof(struct memory));
    
    (*dst)->top  = 0;
    (*dst)->size = size;
    
    (*dst)->stack  = (u32*)malloc(sizeof(u32) * size);

    return VM_OK;
}

enum vm_status vm_memory_push(struct memory* mem, u32 val) {
    if(mem->top == mem->size - 1)
        return VM_ERROR_STACK_OVERFLOW;

    *(mem->stack + (++mem->top)) = val;
    return VM_OK;
}

enum vm_status vm_memory_pop(struct memory* mem, u32* out) {
    if(mem->top == ZERO)
        return VM_ERROR_SEGMENTATION_FAULT;
    
    *out = *(mem->stack + (mem->top--));
    return VM_OK;
}

enum vm_status vm_memory_value(struct memory* mem, u32 address, u32* val) {
    if(address == mem->size - 1)
        return VM_ERROR_SEGMENTATION_FAULT;
    
    *val = mem->stack[address];
    return VM_OK;
}

void vm_memory_free(struct memory* mem) {
    if(mem == NULL)
        return;

    if(mem->stack != NULL)
        free(mem->stack);
    
    free(mem);
}

#endif