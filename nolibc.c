#if !defined(NOLIBC_C)
#define NOLIBC_C

#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

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
syscall3(long n, long a1, long a2, long a3) {
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

void
exit(int exit_code) {
    syscall1(SYS_exit, exit_code);
    while (1) {}
}

long
read(int file_descriptor, void *buffer, size_t count) {
    return syscall3(SYS_read, file_descriptor, (long)buffer, count);
}

ssize_t
write(int file_descriptor, const void *buffer, size_t count) {
    return syscall3(SYS_write, file_descriptor, (long)buffer, count);
}

static inline long __attribute__((always_inline))
open(char *path, long flags, long mode) {
    return syscall3(SYS_open, (long)path, flags, mode);
}

int
close(int file_descriptor) {
    return syscall1(SYS_close, file_descriptor);
}

long
lseek(int file_descriptor, long offset, int whence) {
    return syscall3(SYS_lseek, file_descriptor, offset, whence);
}

static inline long
mmap(void *address, long length, long protection, long flags, long file_descriptor, long offset) {
    return syscall6(SYS_mmap, (long)address, length, protection, flags, file_descriptor, offset);
}

static inline long __attribute__((always_inline))
munmap(void *address, long length) {
    return syscall2(SYS_munmap, (long)address, length);
}

static inline long __attribute__((always_inline))
mprotect(void *address, long length, long protection) {
    return syscall3(SYS_mprotect, (long)address, length, protection);
}

int __attribute__((always_inline))
brk(void *address) {
    return syscall1(SYS_brk, (long)address);
}

#else
#error "Only x86-64 linux is supported"
#endif

#if TESTING_nolibc
int
main(void) {
    {
        long process_id = syscall0(SYS_getpid);
        long kill_return_value;

        if (process_id < 0) {
            return 1;
        }

        kill_return_value = syscall2(SYS_kill, process_id, 0);
        if (kill_return_value < 0) {
            return 1;
        }
    }

    {
        long close_return_value = syscall1(SYS_close, -1);
        if (close_return_value >= 0) {
            return 1;
        }
    }

    {
        char read_buffer[1];
        long read_return_value = syscall3(SYS_read, -1, read_buffer, 1);
        
        if (read_return_value >= 0) {
            return 1;
        }
    }

    {
        char write_buffer[1];
        long pwrite_return_value = syscall4(SYS_pwrite64, -1, write_buffer, 1, 0);
        
        if (pwrite_return_value >= 0) {
            return 1;
        }
    }

    {
        long prctl_return_value = syscall5(SYS_prctl, 3, 0, 0, 0, 0);
        
        if (prctl_return_value < 0) {
            return 1;
        }
    }

    {
        long mmap_return_value = syscall6(SYS_mmap, 0, 4096, 3, 0x22, -1, 0);
        
        if (mmap_return_value < 0) {
            return 1;
        }

        syscall2(SYS_munmap, mmap_return_value, 4096);
    }

    {
        char write_buffer[1];
        long write_return_value;

        write_buffer[0] = '\n';
        write_return_value = write64(-1, write_buffer, 1);
        
        if (write_return_value >= 0) {
            return 1;
        }
    }

    return 0;
}
#endif /* TESTING_nolibc */

#endif /* NOLIBC_C */
