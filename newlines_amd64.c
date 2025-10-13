// cc nolibc.c -o nolibc_cc.exe
// -nostdlib -static -g2 -O3 -Wall -Wextra -fno-stack-protector

#include <sys/syscall.h>
#include <unistd.h>

#if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__clang__)
  #define UNLIKELY(x) __builtin_expect(x, 0)
#else
  #define UNLIKELY(x) (x)
#endif

__asm__(
    ".global _start\n"
    "_start:\n"
    "   movl  (%rsp), %edi\n"   // argc -> %edi
    "   lea   8(%rsp), %rsi\n"  // argv -> %rsi
    "   call  main\n"           // call main(argc, argv)
    "   movl  %eax, %edi\n"     // exit code -> %edi
    "   movl  $60, %eax\n"      // SYS_exit
    "   syscall\n"              // exit(%eax)
);

static inline long __attribute__((always_inline)) 
syscall3(long n, long a1, void *a2, long a3) {
    long ret;
    asm volatile("syscall"
                 : "=a"(ret)
                 : "a"(n), "D"(a1), "S"(a2), "d"(a3)
                 : "rcx", "r11", "memory");
    return ret;
}

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
static char newlines[] = {
    NEWLINES100
    NEWLINES100
};

int
main(int argc, char **argv) {
    int n;
    if (UNLIKELY(argc < 2))
        return 1;

    n = atoi(argv[1]);
    if (UNLIKELY(n <= 0))
        return 1;
    if (UNLIKELY(n > (int)(sizeof(newlines) - 1)))
        n = (int)(sizeof(newlines) - 1);

    syscall3(SYS_write, STDOUT_FILENO, newlines, n);

    return 0;
}
