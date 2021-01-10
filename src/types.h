#ifndef VM_TYPES_H
#define VM_TYPES_H

#include <inttypes.h>

#define ZERO 0

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

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;

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

#endif