#!/bin/sh

# shellcheck disable=SC2086
error () {
    >&2 printf "$@"
    return
}

if [ -n "$BASH_VERSION" ]; then
    # shellcheck disable=SC3044
    shopt -s expand_aliases
fi

alias trace_on='set -x'
alias trace_off='{ set +x; } 2>/dev/null'

CC="${CC:-cc}"
main="newlines.c"

dir="$(readlink -f "$(dirname "$0")")"
cbase="cbase"
CPPFLAGS="$CPPFLAGS -I "$dir/$cbase""

CFLAGS_AMD64="$CFLAGS_AMD64 -nostdlib -static -fno-stack-protector"
CFLAGS_AMD64="$CFLAGS_AMD64 -g2 -O3 -Wall -Wextra"
CPPFLAGS="$CPPFLAGS -Dconst="
CFLAGS="$CFLAGS -fpermissive"

set -x
find . -iname "*.[ch]" -print0 | xargs -0 ctags --kinds-C=+l 2> /dev/null || true
vtags.sed tags > .tags.vim 2> /dev/null || true
$CC $CPPFLAGS $CFLAGS $CFLAGS_AMD64 newlines_amd64.c -o newlines_amd64 || exit 1
# $CC $CPPFLAGS $CFLAGS       newlines.c       -o newlines || exit 1

./newlines_amd64 2
# ./newlines 2

case "$1" in
"test")
    find . -iname "*.c" | sort | while read -r src; do
        trace_off
        name=$(basename "$src")

        if [ -n "$2" ] && [ "$name" != "$2" ]; then
            continue
        fi
        if [ "$name" = "$main" ]; then
            continue
        fi
        if echo "$src" | grep -q "stc/"; then
            continue
        fi
        name=$(echo "$name" | sed 's/\.c//')
        test_exe="/tmp/${name}_test"

        printf "\nTesting ${RED}${src}${RES} ...\n"

        flags="$(awk '/\/\/ flags:/ { $1=$2=""; print $0 }' "$src")"
        echo "flags=$flags"
        if [ "$name" = "windows_functions" ]; then
            if ! zig version; then
                continue
            fi
            cmdline="zig cc $CPPFLAGS $CFLAGS"
            cmdline=$(option_remove "$cmdline" "-D_GNU_SOURCE")
            cmdline="$cmdline -target x86_64-windows-gnu"
            cmdline="$cmdline -Wno-unused-variable -DTESTING_$name=1 -DTESTING=1"
            cmdline="$cmdline $flags -o $test_exe $src"
        else
            cmdline="$CC $CPPFLAGS $CFLAGS"
            cmdline="$cmdline -Wno-unused-variable -DTESTING_$name=1 -DTESTING=1 $LDFLAGS"
            cmdline="$cmdline $flags -o $test_exe $src"
        fi

        if [ "$CC" = "chibicc" ]; then
            cmdline_no_cc=$(option_remove "$cmdline" "$CC")
            trace_on
            if with_chibicc "$cmdline_no_cc"; then
                /tmp/${name}_test
            else
                exit 1
            fi
        else
            trace_on
            if $cmdline; then
                if ! $test_exe; then
                    gdb --quiet \
                        -ex run -ex backtrace -ex quit \
                        $test_exe 2>&1 | xsel -b
                    exit 1
                fi
            else
                exit 1
            fi
        fi
        trace_off
    done
    exit
    ;;
"check")
    CC=gcc CFLAGS="-fanalyzer" ./build.sh
    scan-build --view -analyze-headers --status-bugs ./build.sh
    exit
    ;;
esac

set +x
