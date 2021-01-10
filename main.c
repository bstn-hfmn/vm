#include <stdio.h>

#include "./src/machine.h"
#include "./src/instructions.h"
#include "./src/registers.h"

#include <math.h>
#include <malloc.h>

#define NTH_BIT(b, n)  ((b >> n) & 0x1)
#define BYTE_TO_BIN(b) ((b & 0x80)) |\
                       ((b & 0x40)) |\
                       ((b & 0x20)) |\
                       ((b & 0x10)) |\
                       ((b & 0x08)) |\
                       ((b & 0x04)) |\
                       ((b & 0x02)) |\
                       (b & 0x01)

#define MANTISSA(b)    ((b & 0x400000))  |\
                       (( b & 0x200000)) |\
                       (( b & 0x100000)) |\
                       (( b &  0x80000)) |\
                       (( b &  0x40000)) |\
                       (( b &  0x20000)) |\
                       (( b &  0x10000)) |\
                       (( b &  0x8000))  |\
                       (( b &  0x4000))  |\
                       (( b &  0x2000))  |\
                       (( b &  0x1000))  |\
                       (( b &  0x800))   |\
                       (( b &  0x400))   |\
                       (( b &  0x200))   |\
                       (( b &  0x100))   |\
                       (( b &  0x80))    |\
                       (( b &  0x40))    |\
                       (( b &  0x20))    |\
                       (( b &  0x10))    |\
                       (( b &  0x08))    |\
                       (( b &  0x04))    |\
                       (( b &  0x02))    |\
                       ( b & 0x01 )

typedef enum vm_cpu_flags {
    CPU_FLAG_ZERO,
    CPU_FLAG_POSITIVE,
    CPU_FLAG_NEGATIVE,

    CPU_FLAG_FLOATING,
} vm_cpu_flags;


typedef union IEEE754 {
    f32 original;

    struct {
        u32 mantissa : 23;
        u32 exponent : 8;
        u32 sign     : 1;
    } raw;
} IEEE754_t;

u32 ftou_ieee754(f32 f) {
    union IEEE754 convert;
    convert.original = f;

    return (((NTH_BIT(convert.raw.sign, 0) << 8) | (BYTE_TO_BIN(convert.raw.exponent))) << 23 ) | MANTISSA(convert.raw.mantissa);
}

f32 utof_ieee754(u32 u) {
    u32 sign   = (u & 0x80000000) >> 31;
    u32 exp    = (u & 0x7F800000) >> 23;
    u32 manti  = (u & 0x007FFFFF) >> 0; 

    float acc = ZERO;

    for(int i = 0; i < 23; i++) {
        if(manti & (0x00400000 >> i))
            acc = acc + (1.0 / powf(2, (i + 1)));
    };

    float n = powf(2, (float)(exp - 127));
    if(exp != ZERO)
        acc = 1.0 + acc;
    else
        n = powf(2, -126.0);
    
    if(sign > 0)
        return -(n * acc);
    else
        return n * acc;
}

int main() {
    /**
     * 
     * mov gpb, 2.4
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
    /*
    struct machine* vm;
    vm_machine_create(&vm);
    vm_machine_load(vm, sample, sizeof(sample) / sizeof(u32));

    vm_machine_exec(vm);

    fprintf(stdout, "[TRACE] Finishing...\n");
    vm_machine_free(vm);
    */

    u32 a = 0b01000000000001100000000000000000;
    fprintf(stdout, "COMPUTED: a: %u; b: %d; f: %f\n", a, ~a, utof_ieee754(a));
    
    fprintf(stdout, "UINT: %u\n", ftou_ieee754(2.093750));
    return 0;
}