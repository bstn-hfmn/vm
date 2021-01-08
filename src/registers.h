#ifndef VM_REGISTERS_H
#define VM_REGISTERS_H

typedef enum vm_registers {
    REGISTER_GPA = 0x0,
    REGISTER_GPB = 0x1,
    REGISTER_GPC = 0x2,
    REGISTER_GPD = 0x3,
    REGISTER_GPE = 0x4,
    REGISTER_GPF = 0x5,
    REGISTER_GPG = 0x6,
    REGISTER_GPH = 0x7,
    REGISTER_GPI = 0x8,

    REGISTERS    = 9,
} vm_registers_t;

#endif