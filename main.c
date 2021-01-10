#include <stdio.h>

#include "./src/machine.h"
#include "./src/instructions.h"
#include "./src/registers.h"

#include <math.h>
#include <malloc.h>


int main() {
    /**
     * 
     *    Op     DR   SR   F       Imm12
     * |------| |--| |--| |--| |-----------|
     * 00000000 0000 0000 0000 0000 00000000
     */
    u32 sample[] = {
        0x06200004, // ; mov  gpb, 4
        0x07200000, // ; push gpb
        0x01200002, // ; add  gpb, 2
        0x02100001, // ; sub  gpa, 1
        0x00000000  // ; halt 
    };
    
    struct machine* vm;
    vm_machine_create(&vm);
    vm_machine_load(vm, sample, sizeof(sample) / sizeof(u32));

    vm_machine_exec(vm);

    fprintf(stdout, "[TRACE] Finishing...\n");
    vm_machine_free(vm);
    
    return 0;
}