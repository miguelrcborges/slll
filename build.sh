#!/bin/bash

INCLUDES="-Ithirdparty"
CC="clang"

clang $INCLUDES src/slll.c -o slll.exe
