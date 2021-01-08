#ifndef VM_INSTRUCTIONS_H
#define VM_INSTRUCTIONS_H

typedef enum vm_instructions {
    INSTRUCTION_HALT    = 0x00,
    INSTRUCTION_ADD     = 0x01,
    INSTRUCTION_SUB     = 0x02,
    INSTRUCTION_DIV     = 0x03,
    INSTRUCTION_MUL     = 0x04,

    INSTRUCTION_CMP     = 0x05,
    INSTRUCTION_MOV     = 0x06,

    INSTRUCTION_PUSH    = 0x07,
    INSTRUCTION_POP     = 0x08,

    INSTRUCTIONS
} vm_instructions_t;

#endif