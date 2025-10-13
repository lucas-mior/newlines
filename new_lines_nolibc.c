// gcc -nostdlib -static -O2 -o newlines newlines.c

#include <sys/syscall.h>
#include <unistd.h>

__asm__(
    ".global _start\n"
    "_start:\n"
    "   movl  (%rsp), %edi\n"     // argc -> %edi
    "   lea   8(%rsp), %rsi\n"    // argv -> %rsi
    "   call  main\n"             // call main(argc, argv)
    "   movl  %eax, %edi\n"       // exit code -> %edi
    "   movl  $60, %eax\n"        // SYS_exit
    "   syscall\n"                // exit(%eax)
);

static long syscall3(long n, long a1, void *a2, long a3) {
    long ret;
    asm volatile("syscall"
                 : "=a"(ret)
                 : "a"(n), "D"(a1), "S"(a2), "d"(a3)
                 : "rcx", "r11", "memory");
    return ret;
}

static int str_to_int(const char *s) {
    int v = 0;
    while (*s >= '0' && *s <= '9') {
        v = v * 10 + (*s - '0');
        s++;
    }
    return v;
}

int main(int argc, char **argv) {
    if (argc < 2)
        return 1;

    int n = str_to_int(argv[1]);
    if (n <= 0)
        return 1;

    static const char nl = '\n';
    for (int i = 0; i < n; i++)
        syscall3(SYS_write, STDOUT_FILENO, (void *)&nl, 1);

    return 0;
}
