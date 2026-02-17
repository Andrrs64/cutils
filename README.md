# CUTILS

A collection of STB-style header only libraries for general purpose programming in C. If you're
interested in using it, there are other options that have more features and are better tested, I
just wanted to make my own in order to have full control over how they work, and for learning
purposes.

## Libraries

These are the only libraries so far, but I will be adding more as I see them fit.

- types.h: A collection of common typedef's that I didn't want to keep rewriting.
- macros.h: A few macros that might be of use, pretty similar to types.h in motive.
- l_strings.h: Length based strings with utilities like a string builder
- mem_arena.h: A dead simple memory arena allocator implementation
- default_allocator.h: A simple malloc and free based allocator if you don't want to use arenas

## Usage

Just as with other STB style libraries, all you need to do is to include the headers, and define
the implementation macro in one file to make them work.

```C
#define L_STRINGS_IMPLEMENTATION
#include <cutils/l_strings.h>
```
> [!NOTE]
> Headers types.h and macros.h don't require any implementation

If you're going to use more than one library, you can also use the CUTILS_IMPLEMENTATION to include
implementations for multiple libraries at once.

```C
#define CUTILS_IMPLEMENTATION
#include <cutils/l_strings.h>
#include <cutils/mem_arena.h>
```

## Building the examples

I have set up bash scripts to build the examples and output them to examples/build/, but they are
very rigid and simple. They use gcc so as long as you have that in the path they should work, but if
they don't it's very simple to compile them manually.

The examples can be built individually using build_example.sh:
```bash
./build_example.sh l_strings
```

or all at once with build_all_examples.sh:
```bash
./build_all_examples.sh
```

Example of compiling it manually with clang:
```bash
clang examples/l_strings.c -o examples/build/l_strings.c
```
