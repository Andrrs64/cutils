/*
 * A simple length based string implementation with a string builder and a writer
 */

#ifndef STRINGS_HEADER
#define STRINGS_HEADER

#include <stdint.h>
#include <stdbool.h>
#include "types.h"

// Length based strings

typedef struct {
    char*   buf;
    u64     length;
    b32     null_terminated;
} L_String;

L_String    string_from_cstring(char* str);
void        string_destroy(L_String str, Allocator allocator);
char*       string_to_cstring(L_String str, Allocator allocator);
bool        string_equal(L_String a, L_String b);


// String builder

typedef struct {
    char*   buf;
    u64     capacity;
    u64     length;
} String_Builder;

void    sb_init(String_Builder* sb, Allocator allocator);
/*
 * Initialize String_Builder values with a specific start capacity
 */
void    sb_init_capacity(String_Builder* sb, u64 init_cap, Allocator allocator);
void    sb_destroy(String_Builder* sb, Allocator allocator);
/*
 * Clears the builder so it can be reused multiple times
 */
void    sb_clear(String_Builder* sb);
/*
 * Writes a string to the String_Builder
 */
void    sb_write(String_Builder* sb, L_String str, Allocator allocator);
void    sb_write_cstring(String_Builder* sb, char* str, Allocator allocator);
void    sb_write_i32(String_Builder* sb, i32 val, Allocator allocator);
void    sb_write_i64(String_Builder* sb, i64 val, Allocator allocator);
void    sb_write_u32(String_Builder* sb, u32 val, Allocator allocator);
void    sb_write_u64(String_Builder* sb, u64 val, Allocator allocator);
/*
 * Returns the contents of the String_Builder as an L_String
 * NOTE: This does not alocate a new buffer, it is only a window into the String_Builder's buffer
 */
L_String sb_to_string(String_Builder* sb);
/*
 * Returns a copy of the String_Builder buffer as an L_String
 * NOTE: This allocates a new buffer and it will have to be freed with string_destroy()
 */
L_String sb_copy_to_string(String_Builder* sb, b32 null_terminated, Allocator allocator);

#endif // STRINGS_HEADER

#ifdef CUTILS_IMPLEMENTATION
#define L_STRINGS_IMPLEMENTATION
#endif

#ifdef L_STRINGS_IMPLEMENTATION

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

L_String string_from_cstring(char* str) {
    u64 length = 0;
    while (str[length] != '\0') length++;
    return (L_String){
        .buf                = str,
        .length             = length,
        .null_terminated    = true,
    };
}

void string_destroy(L_String str, Allocator allocator) {
    allocator.free(str.buf, allocator.ctx);
}

char* string_to_cstring(L_String str, Allocator allocator) {
    u64     out_size    = (str.length + 1) * sizeof(char);
    char*   out         = allocator.alloc(out_size, false, allocator.ctx);

    if (str.null_terminated) {
        snprintf(out, out_size, "%s", str.buf);
        return out;
    }

    memcpy(out, str.buf, str.length);
    out[str.length] = '\0';
    return out;
}

bool string_equal(L_String a, L_String b) {
    if (a.buf == b.buf) {
        return true;
    }
    if (a.length != b.length) {
        return false;
    }
    for (i32 i = 0; i < a.length; i++) {
        if (a.buf[i] != b.buf[i]) return false;
    }
    return true;
}


// Builder

void sb_init(String_Builder* sb, Allocator allocator) {
    sb_init_capacity(sb, 64, allocator);
}

void sb_init_capacity(String_Builder* sb, u64 init_cap, Allocator allocator) {
    sb->buf = allocator.alloc(init_cap * sizeof(L_String), false, allocator.ctx);
    if (sb->buf == NULL) {
        printf("[ERROR]: Could not allocate memory for string builder\n");
        abort();
    }
    sb->capacity    = init_cap;
    sb->length      = 0;
}

void sb_destroy(String_Builder* sb, Allocator allocator) {
    allocator.free(sb->buf, allocator.ctx);
    sb->buf         = NULL;
    sb->capacity    = 0;
    sb->length      = 0;
}

void sb_clear(String_Builder* sb) {
    sb->length = 0;
}

void sb_write(String_Builder* sb, L_String str, Allocator allocator) {
    if (sb->capacity < sb->length + str.length) {
        u64 new_size = sb->capacity * 2;
        sb->buf = allocator.realloc(sb->buf, sb->capacity, new_size, allocator.ctx);

        if (sb->buf == NULL) {
            printf("[ERROR]: Could not reallocate memory for string builder\n");
            abort();
        }
    }

    memcpy(sb->buf + sb->length, str.buf, str.length);
    sb->length += str.length;
}

void sb_write_cstring(String_Builder* sb, char* str, Allocator allocator) {
    sb_write(sb, string_from_cstring(str), allocator);
}

void sb_write_i32(String_Builder* sb, i32 val, Allocator allocator) {
    char tmp[12];
    int n = snprintf(tmp, sizeof(tmp), "%d", val);
    sb_write(sb, (L_String){ .buf = tmp, .length = (u64)n, .null_terminated = true }, allocator);
}
void sb_write_u32(String_Builder* sb, u32 val, Allocator allocator) {
    char tmp[11];
    int n = snprintf(tmp, sizeof(tmp), "%u", val);
    sb_write(sb, (L_String){ .buf = tmp, .length = (u64)n, .null_terminated = true }, allocator);
}
void sb_write_i64(String_Builder* sb, i64 val, Allocator allocator) {
    char tmp[21];
    int n = snprintf(tmp, sizeof(tmp), "%ld", val);
    sb_write(sb, (L_String){ .buf = tmp, .length = (u64)n, .null_terminated = true }, allocator);
}
void sb_write_u64(String_Builder* sb, u64 val, Allocator allocator) {
    char tmp[21];
    int n = snprintf(tmp, sizeof(tmp), "%lu", val);
    sb_write(sb, (L_String){ .buf = tmp, .length = (u64)n, .null_terminated = true }, allocator);
}

L_String sb_to_string(String_Builder* sb) {
    return (L_String) {
        .buf                = sb->buf,
        .length             = sb->length,
        .null_terminated    = false,
    };
}

L_String sb_copy_to_string(String_Builder* sb, b32 null_terminated, Allocator allocator) {
    u64 buffer_size = sb->length * sizeof(char);
    if (null_terminated) buffer_size += sizeof(char);

    L_String out = {
        .buf                = allocator.alloc(buffer_size, false, allocator.ctx),
        .length             = sb->length,
        .null_terminated    = null_terminated,
    };

    memcpy(out.buf, sb->buf, sb->length);

    if (null_terminated) out.buf[buffer_size-1] = '\0';
    return out;
}

#endif // L_STRINGS_IMPLEMENTATION
