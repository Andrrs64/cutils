#!/usr/bin/env bash

if [[ -z "$1" ]]; then
    echo "ERROR: Expected example name as first argument" >&2
    exit 1
fi

gcc -I include -o examples/build/$1 examples/$1.c ${@:2}
