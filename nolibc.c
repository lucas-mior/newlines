#if !defined(NOLIBC_C)
#define NOLIBC_C

#include <sys/syscall.h>
#include <unistd.h>

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
syscall3(long n, long a1, void *a2, long a3) {
    long ret;
    asm volatile("syscall"
                 : "=a"(ret)
                 : "a"(n), "D"(a1), "S"(a2), "d"(a3)
                 : "rcx", "r11", "memory");
    return ret;
}

#endif
