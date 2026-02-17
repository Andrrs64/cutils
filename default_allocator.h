#ifndef DEFAULT_ALLOCATOR_HEADER
#define DEFAULT_ALLOCATOR_HEADER

#include "types.h"

Allocator get_default_allocator();

#endif // ALLOCATOR_HEADER

#ifdef CUTILS_IMPLEMENTATION
#define DEFAULT_ALLOCATOR_IMPLEMENTATION
#endif // CUTILS_IMPLEMENTATION

#ifdef DEFAULT_ALLOCATOR_IMPLEMENTATION

#include <stdlib.h>

void* _default_alloc(u64 size, b32 zero_init, void* ctx) {
    return zero_init ? calloc(size, sizeof(u8)) : malloc(size);
}

void* _default_realloc(void* ptr, u64 old_size, u64 new_size, void* ctx) {
    return realloc(ptr, new_size);
}

void _default_free(void* ptr, void* ctx) {
    free(ptr);
}

Allocator get_default_allocator() {
    Allocator a;
    a.alloc     = _default_alloc;
    a.realloc   = _default_realloc;
    a.free      = _default_free;
    return a;
}

#endif // DEFAULT_ALLOCATOR_IMPLEMENTATION
