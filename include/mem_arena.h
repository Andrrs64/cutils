#ifndef MEM_ARENA_HEADER
#define MEM_ARENA_HEADER

#include "types.h"

typedef struct {
    u64 capacity, pos;
} Arena;

Arena*  arena_alloc(const u64 capacity);
void    arena_destroy(Arena* arena);
void*   arena_push(Arena* arena, const u64 size, b32 zero_init);
void    arena_pop(Arena* arena, const u64 size);
void    arena_pop_to(Arena* arena, const u64 pos);
void    arena_clear(Arena* arena);

#endif // MEM_ARENA_HEADER

#ifdef CUTILS_IMPLEMENTATION
#define MEM_ARENA_IMPLEMENTATION
#endif

#ifdef MEM_ARENA_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "macros.h"

Arena* arena_alloc(const u64 capacity) {
    Arena* arena = (Arena*)malloc(capacity);
    if (arena == NULL) {
        printf("[ERROR]: Unable to allocate memory arena");
#ifdef DONT_ABORT_ON_ARENA_FAILURE
        printf(">   Returning NULL\n");
        return NULL;
#else
        abort();
#endif
    }
    arena->capacity = capacity;
    arena->pos      = sizeof(Arena);

    return arena;
}

void arena_destroy(Arena* arena) {
    arena->capacity = 0;
    arena->pos      = sizeof(Arena);
    free((void*)arena);
}

void* arena_push(Arena* arena, const u64 size, b32 zero_init) {
    u64 aligned = ALIGN_UP_POW2(arena->pos, sizeof(void*));
    u64 new_pos = aligned + size;

    if (new_pos > arena->capacity) {
        static const char* units[] = { "B", "KiB", "MiB", "GiB" };

        u64 cap_val     = arena->capacity;
        u8  unit_idx    = 0;

        while (cap_val >= 1024 && unit_idx < 4) {
            cap_val /= 1024;
            unit_idx++;
        }
        printf("[ERROR]: Arena allocations exceeded capacity of %llu%s\n",
                cap_val, units[unit_idx]);
#ifdef DONT_ABORT_ON_ARENA_FAILURE
        printf(">   Returning NULL\n");
        return NULL;
#else
        abort();
#endif
    }

    u8* out = (u8*)arena + aligned;
    if (zero_init) {
        memset(out, 0, size);
    }
    arena->pos = new_pos;
    return out;
}

void arena_pop(Arena* arena, const u64 size) {
    arena->pos -= MIN(size, arena->pos - sizeof(Arena));
}

void arena_pop_to(Arena* arena, const u64 pos) {
    if (pos < arena->pos) {
        arena->pos = MAX(pos, (u64)arena + sizeof(Arena));
    }
}

void arena_clear(Arena* arena) {
    arena->pos = sizeof(Arena);
}

#endif // MEM_ARENA_IMPLEMENTATION
