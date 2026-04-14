#if !defined(NOLIBC_C)
#define NOLIBC_C

#include <sys/syscall.h>
#include <unistd.h>

#if defined(__x86_64__)

__asm__(".global _start\n"
        "_start:\n"
        "   movl  (%rsp), %edi\n"   // argc -> %edi
        "   lea   8(%rsp), %rsi\n"  // argv -> %rsi
        "   call  main\n"           // call main(argc, argv)
        "   movl  %eax, %edi\n"     // exit code -> %edi
        "   movl  $60, %eax\n"      // SYS_exit
        "   syscall\n"              // exit(%eax)
);

static inline long __attribute__((always_inline))
syscall0(long n) {
    long ret = 0;
    asm volatile("syscall"
                 : "=a"(ret)
                 : "a"(n)
                 : "rcx", "r11", "memory");
    return ret;
}

static inline long __attribute__((always_inline))
syscall1(long n, long a1) {
    long ret = 0;
    asm volatile("syscall"
                 : "=a"(ret)
                 : "a"(n), "D"(a1)
                 : "rcx", "r11", "memory");
    return ret;
}

static inline long __attribute__((always_inline))
syscall2(long n, long a1, long a2) {
    long ret = 0;
    asm volatile("syscall"
                 : "=a"(ret)
                 : "a"(n), "D"(a1), "S"(a2)
                 : "rcx", "r11", "memory");
    return ret;
}

static inline long __attribute__((always_inline))
syscall3(long n, long a1, void *a2, long a3) {
    long ret;
    asm volatile("syscall"
                 : "=a"(ret)
                 : "a"(n), "D"(a1), "S"(a2), "d"(a3)
                 : "rcx", "r11", "memory");
    return ret;
}

static inline long __attribute__((always_inline))
syscall4(long n, long a1, long a2, long a3, long a4) {
    long ret = 0;
    register long r10 __asm__("r10") = a4;

    asm volatile("syscall"
                 : "=a"(ret)
                 : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10)
                 : "rcx", "r11", "memory");
    return ret;
}

static inline long __attribute__((always_inline))
syscall5(long n, long a1, long a2, long a3, long a4, long a5) {
    long ret = 0;
    register long r10 __asm__("r10") = a4;
    register long r8 __asm__("r8") = a5;

    asm volatile("syscall"
                 : "=a"(ret)
                 : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8)
                 : "rcx", "r11", "memory");
    return ret;
}

static inline long __attribute__((always_inline))
syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6) {
    long ret = 0;
    register long r10 __asm__("r10") = a4;
    register long r8 __asm__("r8") = a5;
    register long r9 __asm__("r9") = a6;

    asm volatile("syscall"
                 : "=a"(ret)
                 : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8), "r"(r9)
                 : "rcx", "r11", "memory");
    return ret;
}

#else
#error "Only x86-64 linux is supported"
#endif

#if TESTING_nolibc
int
main(void) {
    return 0;
}
#endif

#endif
