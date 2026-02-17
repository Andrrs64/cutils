#ifndef TYPES_HEADER
#define TYPES_HEADER

#include <stdint.h>

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;
typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;
typedef int64_t     i64;
typedef u8          b8;
typedef u32         b32;

#define KiB(n) ((u64)(n) << 10)
#define MiB(n) ((u64)(n) << 20)
#define GiB(n) ((u64)(n) << 30)

typedef struct {
    void*   (*alloc)(u64 size, b32 zero_init, void* ctx);
    void*   (*realloc)(void* ptr, u64 old_size, u64 new_size, void* ctx);
    void    (*free)(void* ptr, void* ctx);
    void*   ctx;
} Allocator;

#endif // TYPES_HEADER
