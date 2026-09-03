#!/bin/sh -e

# shellcheck disable=SC2086

dir=$(dirname "$(readlink -f "$0")")
cd "$dir" || exit

newlines_test_exclude_pattern='(^|/)(newlines_amd64|nolibc)\.c$'
newlines_old_test_exclude_pattern=${TEST_EXCLUDE_PATTERN-}
newlines_had_test_exclude_pattern=${TEST_EXCLUDE_PATTERN+x}
if [ -n "${TEST_EXCLUDE_PATTERN:-}" ]; then
    TEST_EXCLUDE_PATTERN="$newlines_test_exclude_pattern|$TEST_EXCLUDE_PATTERN"
else
    TEST_EXCLUDE_PATTERN=$newlines_test_exclude_pattern
fi

# shellcheck source=./cbase/common.sh
. "./cbase/common.sh"

if [ "$newlines_had_test_exclude_pattern" = x ]; then
    TEST_EXCLUDE_PATTERN=$newlines_old_test_exclude_pattern
else
    unset TEST_EXCLUDE_PATTERN
fi

program=$(common_get_program "$0")
script=$(basename "$0")
common_build_parse_args "$@"

case "$mode" in
build|check|debug|debug-fast|fast_feedback|install|test|uninstall)
    ;;
*)
    common_build_unknown_mode
    ;;
esac

common_build_print_invocation "$script"

PREFIX="${PREFIX:-/usr/local}"
DESTDIR="${DESTDIR:-/}"

main="newlines.c"
main_amd64="newlines_amd64.c"
exe="bin/newlines_amd64"
mkdir -p "$(dirname "$exe")"

CPPFLAGS="$CPPFLAGS -I$dir/cbase"
CPPFLAGS="$CPPFLAGS -Dconst= -DNOLIBC=1"
CFLAGS="$CFLAGS -fpermissive"
CFLAGS_AMD64="$CFLAGS_AMD64 -nostdlib -static -fno-stack-protector"
CFLAGS_AMD64="$CFLAGS_AMD64 -fno-builtin"
LDFLAGS="$LDFLAGS -lm"

OS=$(uname -a)
GNUSOURCE=
if echo "$OS" | grep -q "Linux"; then
    if echo "$OS" | grep -q "GNU"; then
        GNUSOURCE="-D_GNU_SOURCE"
    fi
fi

CC=$(common_get_compiler "$mode")

case "$mode" in
"debug")
    CFLAGS_AMD64="$CFLAGS_AMD64 -g3 -O0"
    CPPFLAGS="$CPPFLAGS $GNUSOURCE -DDEBUGGING=1"
    exe="bin/newlines_amd64_debug"
    ;;
"debug-fast")
    CFLAGS_AMD64="$CFLAGS_AMD64 $GNUSOURCE -g2 -O3"
    CFLAGS_AMD64="$CFLAGS_AMD64 -fsanitize=undefined"
    CFLAGS_AMD64="$CFLAGS_AMD64 -fsanitize-undefined-trap-on-error"
    CPPFLAGS="$CPPFLAGS -DDEBUGGING=1"
    exe="bin/newlines_amd64_debug_fast"
    ;;
"test")
    CFLAGS_AMD64="$CFLAGS_AMD64 -g3 -O0"
    CPPFLAGS="$CPPFLAGS $GNUSOURCE -DDEBUGGING=1"
    ;;
"build")
    CFLAGS_AMD64="$CFLAGS_AMD64 $GNUSOURCE -g2 -O3"
    ;;
"fast_feedback")
    CC=clang
    CFLAGS_AMD64="$CFLAGS_AMD64 $GNUSOURCE -Werror"
    ;;
"install"|"uninstall")
    ;;
*)
    common_build_unknown_mode
    ;;
esac

with_toy_cc () {
    compiler="$1"
    compiler_macro=$(echo "$compiler" | tr '[:lower:]' '[:upper:]')
    compiler_macro="__${compiler_macro}__"
    shift
    args="$*"
    trace_on
    while ! problem=$($compiler "-D${compiler_macro}" $args 2>&1); do
        trace_off
        problem=$(echo "$problem" | head -n 1 | tr -d "'")

        sleep 0.4
        if echo "$problem" | grep -Eq "unknown (argument|option)"; then
            arg=$(echo "$problem" | awk '{print $NF}')
            printf "
Removing argument %s...
" "$arg"
            args=$(common_option_remove "$args" "$arg")
        else
            printf "

Error compiling with %s:

%s

" "$compiler" "$problem"
            return 1
        fi
        printf "
"
        trace_on
    done
    return 0
}
build_program () {
    common_build_tags
    trace_on
    $CC $CPPFLAGS $CFLAGS $CFLAGS_AMD64 "$main_amd64" -o "$exe" $LDFLAGS
    trace_off
}

testing () {
    find . -iname "*.c" | sort | while read -r src; do
        trace_off
        name=$(basename "$src")

        if [ -n "${2:-}" ] && [ "$name" != "$2" ]; then
            continue
        fi
        if [ "$name" = "$main" ] || [ "$name" = "$main_amd64" ]; then
            continue
        fi
        if echo "$src" | grep -q "stc/"; then
            continue
        fi
        if echo "$src" | grep -q "cbase/"; then
            continue
        fi
        name=$(echo "$name" | sed 's/\.c//')
        test_exe="/tmp/${name}_test"

        printf "\nTesting ${RED}%s${RES} ...\n" "$src"

        flags=$(awk '/\/\/ flags:/ { $1=$2=""; print $0 }' "$src")
        cmdline="$CC $CPPFLAGS $CFLAGS"
        cmdline="$cmdline -Wno-unused-variable -DTESTING_$name=1 -DTESTING=1 $LDFLAGS"
        cmdline="$cmdline $flags -o $test_exe $src"

        if [ "$CC" = "chibicc" ]; then
            cmdline_no_cc=$(common_option_remove "$cmdline" "$CC")
            trace_on
            if with_toy_cc "$CC" "$cmdline_no_cc"; then
                "$test_exe"
            else
                exit 1
            fi
        else
            trace_on
            if $cmdline; then
                if ! "$test_exe"; then
                    gdb --quiet -ex run -ex backtrace -ex quit "$test_exe"
                    exit 1
                fi
            else
                exit 1
            fi
        fi
        trace_off
    done
}

case "$mode" in
"fast_feedback")
    build_program
    "$exe" 2
    ;;
"test")
    testing "$@"
    ;;
"check")
    CC=gcc CFLAGS="-fanalyzer -fdiagnostics-color=never" "$0" build
    CFLAGS="--analyze -Xanalyzer -analyzer-output=text"
    CFLAGS="$CFLAGS -Xanalyzer -analyzer-werror"
    CFLAGS="$CFLAGS -Xanalyzer -analyzer-opt-analyze-headers"
    CFLAGS="$CFLAGS -Wno-unused-command-line-argument"
    CFLAGS="$CFLAGS -fno-color-diagnostics"
    CC=clang CFLAGS="$CFLAGS" "$0" build
    echo "static analysis finished."
    exit
    ;;
"uninstall")
    trace_on
    rm -f "${DESTDIR}${PREFIX}/bin/newlines_amd64"
    trace_off
    ;;
"install")
    if [ ! -f "$exe" ]; then
        "$0" build
    fi
    trace_on
    install -Dm755 "$exe" "${DESTDIR}${PREFIX}/bin/newlines_amd64"
    trace_off
    ;;
build|debug|debug-fast)
    build_program
    "$exe" 2
    ;;
esac
