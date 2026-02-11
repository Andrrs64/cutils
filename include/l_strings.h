/*
 * A simple length based string implementation with a string builder and a writer
 */

#ifndef STRINGS_HEADER
#define STRINGS_HEADER

#include <stdint.h>
#include <stdbool.h>

typedef int32_t     i32;
typedef int64_t     i64;
typedef uint32_t    u32;
typedef uint64_t    u64;
typedef uint32_t    b32;
typedef uint64_t    b64;


// Length based strings

typedef struct {
    char*   buf;
    u64     length;
    b32     null_terminated;
} L_String;

L_String    string_literal(char* str);
void        string_destroy(L_String str);
void        string_to_cstring(L_String str, char** out, u64 out_size);
bool        string_equal(L_String a, L_String b);


// String builder

typedef struct {
    char*   buf;
    u64     capacity;
    u64     length;
} String_Builder;

void    sb_init(String_Builder* sb);
/*
 * Initialize String_Builder values with a specific start capacity
 */
void    sb_init_capacity(String_Builder* sb, u64 init_cap);
void    sb_destroy(String_Builder* sb);
/*
 * Clears the builder so it can be reused multiple times
 */
void    sb_clear(String_Builder* sb);
/*
 * Writes a string to the String_Builder
 */
void    sb_write(String_Builder* sb, L_String str);
void    sb_write_cstring(String_Builder* sb, char* str);
void    sb_write_i32(String_Builder* sb, i32 val);
void    sb_write_i64(String_Builder* sb, i64 val);
void    sb_write_u32(String_Builder* sb, u32 val);
void    sb_write_u64(String_Builder* sb, u64 val);
/*
 * Returns the contents of the String_Builder as an L_String
 * NOTE: This does not alocate a new buffer, it is only a window into the String_Builder's buffer
 */
L_String sb_to_string(String_Builder* sb);
/*
 * Returns a copy of the String_Builder buffer as an L_String
 * NOTE: This allocates a new buffer and it will have to be freed with string_destroy()
 */
L_String sb_copy_to_string(String_Builder* sb, b32 null_terminated);

// String writer

typedef struct {
    char*   buf;
    u64     capacity;
    u64     length;
    u64     cursor;
} String_Writer;

void        sw_init(String_Writer* sw);
void        sw_init_capacity(String_Writer* sw, u64 init_cap);
void        sw_destroy(String_Writer* sw);
void        sw_write_char(String_Writer* sw, char c);
void        sw_write_string(String_Writer* sw, L_String str);
void        sw_delete(String_Writer* sw);
void        sw_delete_len(String_Writer* sw, u64 len);
void        sw_delete_to(String_Writer* sw, u64 pos);
L_String    sw_to_string(String_Writer* sw);

#endif // STRINGS_HEADER

#ifdef STRINGS_IMPLEMENTATION

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

L_String string_literal(char* str) {
    u64 length = 0;
    while (str[length] != '\0') length++;
    return (L_String){
        .buf                = str,
        .length             = length,
        .null_terminated    = true,
    };
}

void string_destroy(L_String str) {
    free(str.buf);
}

void string_to_cstring(L_String str, char** out, u64 out_size) {
    if (str.null_terminated) {
        *out = str.buf;
    }
    bool terminated = false;
    for (i32 i = 0; i < str.length; i++) {
        if (i == out_size - 1) {
            *out[i] = '\0';
            terminated = true;
            break;
        }
        *out[i] = str.buf[i];
    }
    if (!terminated) *out[str.length+1] = '\0';
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

void sb_init(String_Builder* sb) {
    sb_init_capacity(sb, 64);
}

void sb_init_capacity(String_Builder* sb, u64 init_cap) {
    sb->buf = malloc(init_cap * sizeof(L_String));
    if (sb->buf == NULL) {
        printf("[ERROR]: Could not allocate memory for string builder\n");
        abort();
    }
    sb->capacity    = init_cap;
    sb->length      = 0;
}

void sb_destroy(String_Builder* sb) {
    free(sb->buf);
    sb->buf         = NULL;
    sb->capacity    = 0;
    sb->length      = 0;
}

void sb_clear(String_Builder* sb) {
    sb->length = 0;
}

void sb_write(String_Builder* sb, L_String str) {
    if (sb->capacity < sb->length + str.length) {
        sb->capacity *= 2;
        sb->buf = realloc(sb->buf, sb->capacity * sizeof(L_String));

        if (sb->buf == NULL) {
            printf("[ERROR]: Could not reallocate memory for string builder\n");
            abort();
        }
    }

    memcpy(sb->buf + sb->length, str.buf, str.length);
    sb->length += str.length;
}

void sb_write_cstring(String_Builder* sb, char* str) {
    sb_write(sb, string_literal(str));
}

#define MAX_NUM_STRLEN 64
void sb_write_i32(String_Builder* sb, i32 val) {
    char tmp[MAX_NUM_STRLEN];
    int n = sprintf(tmp, "%d", val);
    sb_write(sb, (L_String){ .buf = tmp, .length = (u64)n, .null_terminated = true });
}
void sb_write_i64(String_Builder* sb, i64 val) {
    char tmp[MAX_NUM_STRLEN];
    int n = sprintf(tmp, "%lld", val);
    sb_write(sb, (L_String){ .buf = tmp, .length = (u64)n, .null_terminated = true });
}
void sb_write_u32(String_Builder* sb, u32 val) {
    char tmp[MAX_NUM_STRLEN];
    int n = sprintf(tmp, "%u", val);
    sb_write(sb, (L_String){ .buf = tmp, .length = (u64)n, .null_terminated = true });
}
void sb_write_u64(String_Builder* sb, u64 val) {
    char tmp[MAX_NUM_STRLEN];
    int n = sprintf(tmp, "%llu", val);
    sb_write(sb, (L_String){ .buf = tmp, .length = (u64)n, .null_terminated = true });
}

L_String sb_to_string(String_Builder* sb) {
    return (L_String) {
        .buf                = sb->buf,
        .length             = sb->length,
        .null_terminated    = false,
    };
}

L_String sb_copy_to_string(String_Builder* sb, b32 null_terminated) {
    u64 buffer_size = sb->length * sizeof(char);
    if (null_terminated) buffer_size += sizeof(char);

    L_String out = {
        .buf                = malloc(buffer_size),
        .length             = sb->length,
        .null_terminated    = null_terminated,
    };

    memcpy(out.buf, sb->buf, sb->length);

    if (null_terminated) out.buf[buffer_size-1] = '\0';
    return out;
}


// Writer

void sw_init(String_Writer* sw) {
    sw_init_capacity(sw, 128);
}

void sw_init_capacity(String_Writer* sw, u64 init_cap) {
    sw->buf = malloc(init_cap * sizeof(char));
    if (sw->buf == NULL) {
        printf("[ERROR]: Could not allocate memory for string writer");
        abort();
    }
    sw->capacity    = init_cap;
    sw->length      = 0;
}

void sw_destroy(String_Writer* sw) {

}

void sw_write_char(String_Writer* sw, char c) {

}

void sw_write_string(String_Writer* sw, L_String str) {

}

void sw_delete(String_Writer* sw) {

}

void sw_delete_len(String_Writer* sw, u64 len) {

}

void sw_delete_to(String_Writer* sw, u64 pos) {

}

L_String sw_to_string(String_Writer* sw) {
    return (L_String){0};
}

#endif // STRINGS_IMPLEMENTATION
