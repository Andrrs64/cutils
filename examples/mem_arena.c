#define MEM_ARENA_IMPLEMENTATION
#define DONT_ABORT_ON_ARENA_FAILURE
#include <mem_arena.h>
#include <stdbool.h>
#include <stdio.h>

int main() {
    Arena*  arena   = arena_alloc(KiB(10));
    u8*     buf_2K  = arena_push(arena, KiB(2), false); // 2 Kilobytes of uninitialized memory

    u8* buf_10B = arena_push(arena, 10, true); // 10 bytes of zero initialized memory
    printf("Should all be zero:");
    for (int i = 0; i < 10; i++) printf(" %d,", buf_10B[i]);
    printf("\n");
    arena_clear(arena); // clears the arena, but does not deallocate

    // FAILS (1MiB > 1KiB)
    // calls abort() unless DONT_ABORT_ON_ARENA_FAILURE is defined, then it returns NULL
    u8* buf_1M  = arena_push(arena, MiB(1), false);

    arena_destroy(arena); // deallocates the arena
    return 0;
}
