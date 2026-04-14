#include <unistd.h>
#include "nolibc.c"

#if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__clang__)
#define UNLIKELY(x) __builtin_expect(x, 0)
#else
#define UNLIKELY(x) (x)
#endif

static inline int __attribute__((always_inline)) atoi(const char *s);

int __attribute__((always_inline))
atoi(const char *s) {
    int v = 0;
    while (*s >= '0' && *s <= '9') {
        v = v*10 + (*s - '0');
        s++;
    }
    return v;
}

void *
memset(void *dest, int c, size_t n) {
    char *p = dest;
    for (size_t i = 0; i < n; i += 1) {
        p[i] = (char)c;
    }
    return dest;
}

#define NEWLINES5 "\n\n\n\n\n"
#define NEWLINES25 NEWLINES5 NEWLINES5 NEWLINES5 NEWLINES5 NEWLINES5
#define NEWLINES100 NEWLINES25 NEWLINES25 NEWLINES25 NEWLINES25
static char newlines[] = {NEWLINES100 NEWLINES100};

int
main(int argc, char **argv) {
    int n;
    if (UNLIKELY(argc < 2)) {
        return 1;
    }

    n = atoi(argv[1]);
    if (UNLIKELY(n <= 0)) {
        return 1;
    }
    if (UNLIKELY(n > (int)(sizeof(newlines) - 1))) {
        n = (int)(sizeof(newlines) - 1);
    }

    syscall3(SYS_write, STDOUT_FILENO, (long)newlines, n);

    return 0;
}
