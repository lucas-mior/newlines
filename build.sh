#!/bin/sh

CC="${CC:-cc}"

CFLAGS_AMD64="$CFLAGS_AMD64 -nostdlib -static -fno-stack-protector"
CFLAGS_AMD64="$CFLAGS_AMD64 -g2 -O3 -Wall -Wextra"

set -x
ctags --kinds-C=+l *.h *.c 2> /dev/null || true
vtags.sed tags > .tags.vim 2> /dev/null || true
$CC $CFLAGS_AMD64 newlines_amd64.c -o newlines_amd64
$CC $CFLAGS       newlines.c       -o newlines

./newlines_amd64 2
./newlines 2
set +x
