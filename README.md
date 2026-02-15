# CUTILS

A collection of STB-style header only libraries for general purpose programming in C. If you're
interested in using it, there are other options that have more features, and are better tested, I
just wanted to make my own in order to have full control over how they work, and for learning
purposes.

## Libraries

These are the only libraries so far, but I will be adding more as I see them fit.

- types.h: A collection of common typedef's that I didn't want to keep rewriting.
- macros.h: A few macros that might be of use, pretty similar to types.h in motive.
- l_strings.h: Length based strings with utilities like a string builder
- mem_arena.h: A dead simple memory arena allocator implementation

## TODO

- Add a general allocator type, and integrate it in all the libraries so you can choose how they
    allocate memory
    - This should really have been first priority when making this, but it went over my head



## Building the examples

I have set up bash scripts to build the examples, but they are very rigid and simple. They use
gcc so as long as you have that in the path they should work, but if they don't it shouldn't be
hard to compile them manually.

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
clang examples/l_strings.c -o examples/build/l_strings.c -I include
```
