#define STRINGS_IMPLEMENTATION
#include <l_strings.h>
#include <types.h>
#include <assert.h>
#include <stdio.h>

int main() {
    L_String str = string_from_cstring("String literal");

    assert(str.null_terminated); // str points to a null terminated buffer
    printf("%s\n", str.buf); // If you know the string is null terminated

    // Safer approach
    // this copies the string into a new buffer that will have to be freed
    char* cs = string_to_cstring(str);
    printf("%s\n", cs);
    free(cs);

    L_String str2 = string_from_cstring("String literal");
    if (string_equal(str, str2)) {
        printf("Strings are equal\n");
    } else {
        printf("Strings are not equal\n");
    }

    // String builder
    String_Builder sb = {0};
    sb_init(&sb);

    sb_write(&sb, string_from_cstring("You can write L_Strings, "));
    sb_write_cstring(&sb, "and you can write cstrings.");

    L_String copied_string = sb_copy_to_string(&sb, true); // Copy to a null terminated string
    assert(str.null_terminated);
    printf("Builder result: %s\n", copied_string.buf);
    string_destroy(copied_string);

    sb_clear(&sb); // Clears the string builder so it can be reused

    sb_write_cstring(&sb, "You can also write integers: ");
    sb_write_u64(&sb, 420);
    sb_write_i32(&sb, -69);

    // Creates a window into the string builder's contents,
    // the String_Builder still owns the memory
    L_String str_window = sb_to_string(&sb);

    char* cs_str_window = string_to_cstring(str_window);
    printf("Builder result: %s\n", cs_str_window);
    free(cs_str_window);

    return 0;
}
