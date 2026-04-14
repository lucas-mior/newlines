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

int
chdir(const char *path) {
    return syscall1(SYS_chdir, (long)path);
}

int
mkdir(char *pathname, long mode) {
    return syscall2(SYS_mkdir, (long)pathname, mode);
}

int
rmdir(const char *pathname) {
    return syscall1(SYS_rmdir, (long)pathname);
}

// TODO: syscalls
// _llseek
// _newselect
// accept                      
// accept4
// access
// acct
// add_key
// adjtimex
// alarm
// arch_prctl                  
// bind                       
// bpf
// capget
// capset
// chmod
// chown                      
// chown32
// chroot
// clock_adjtime
// clock_getres
// clock_gettime
// clock_nanosleep
// clock_settime
// clone
// clone3
// close_range
// connect                    
// copy_file_range
// creat
// delete_module
// dup
// dup2
// dup3
// epoll_create
// epoll_create1
// epoll_ctl
// epoll_pwait
// epoll_pwait2
// epoll_wait
// eventfd
// eventfd2
// execve
// execveat
// exit_group
// faccessat
// faccessat2
// fadvise64
// fadvise64_64
// fallocate
// fanotify_init
// fanotify_mark
// fchdir
// fchmod
// fchmodat
// fchown
// fchown32
// fchownat
// fcntl
// fcntl64
// fdatasync
// fgetxattr
// finit_module
// flistxattr
// flock
// fork
// fremovexattr
// fsconfig
// fsetxattr
// fsmount
// fsopen
// fspick
// fstat
// fstat64
// fstatat64
// fstatfs
// fstatfs64
// fsync
// ftruncate
// ftruncate64
// futex
// futimesat
// get_mempolicy
// get_robust_list
// get_thread_area
// getcpu
// getcwd
// getdents
// getdents64
// getegid
// getegid32
// geteuid
// geteuid32
// getgid
// getgid32
// getgroups
// getgroups32
// getitimer
// getpeername            
// getpgid
// getpgrp
// getpid
// getppid
// getpriority
// getrandom
// getresgid
// getresgid32
// getresuid
// getresuid32
// getrlimit
// getrusage
// getsid
// getsockname                     
// getsockopt                     
// gettid
// gettimeofday
// getuid
// getuid32
// getxattr
// init_module
// inotify_add_watch
// inotify_init
// inotify_init1
// inotify_rm_watch
// io_cancel
// io_destroy
// io_getevents
// io_pgetevents
// io_setup
// io_submit
// io_uring_enter
// io_uring_register
// io_uring_setup
// ioctl
// ioperm
// iopl
// ioprio_get
// ioprio_set
// ipc
// kcmp
// kexec_file_load
// kexec_load
// keyctl
// kill
// landlock_add_rule
// landlock_create_ruleset
// landlock_restrict_self
// lchown                             
// lchown32
// lgetxattr
// link
// linkat
// listen                            
// listxattr
// llistxattr
// lookup_dcookie
// lremovexattr
// lsetxattr
// lstat
// lstat64
// madvise
// mbind
// membarrier
// memfd_create
// memfd_secret
// migrate_pages
// mincore
// mkdirat
// mknod
// mknodat
// mlock
// mlock2
// mlockall
// mmap2
// modify_ldt
// mount
// move_mount
// move_pages
// mq_getsetattr
// mq_notify
// mq_open
// mq_timedreceive
// mq_timedsend
// mq_unlink
// mremap
// msgctl                             
// msgget                            
// msgrcv                           
// msgsnd                          
// msync
// munlock
// munlockall
// name_to_handle_at
// nanosleep
// newfstatat                     
// nice
// oldfstat
// oldlstat
// oldolduname
// oldstat
// oldumount                     
// olduname
// open_by_handle_at
// open_tree
// openat
// openat2
// pause
// perf_event_open                 
// personality
// pidfd_getfd
// pidfd_send_signal
// pidfd_open
// pipe
// pipe2
// pivot_root
// pkey_alloc
// pkey_free
// pkey_mprotect
// poll
// ppoll
// prctl
// pread64                         
// preadv
// preadv2
// prlimit64
// process_madvise
// process_vm_readv
// process_vm_writev
// pselect6
// ptrace
// pwrite64                        
// pwritev
// pwritev2
// quotactl
// quotactl_fd
// readahead
// readdir
// readlink
// readlinkat
// readv
// reboot
// recv                             
// recvfrom                        
// recvmsg                        
// recvmmsg
// removexattr
// rename
// renameat
// renameat2
// request_key
// restart_syscall
// rseq
// rt_sigaction
// rt_sigpending
// rt_sigprocmask
// rt_sigqueueinfo
// rt_sigreturn
// rt_sigsuspend
// rt_sigtimedwait
// rt_tgsigqueueinfo
// sched_get_priority_max
// sched_get_priority_min
// sched_getaffinity
// sched_getattr
// sched_getparam
// sched_getscheduler
// sched_rr_get_interval
// sched_set_affinity         
// sched_setaffinity
// sched_setattr
// sched_setparam
// sched_setscheduler
// sched_yield
// seccomp
// select
// semctl                    
// semget                   
// semop                   
// semtimedop
// send                    
// sendfile
// sendfile64
// sendmmsg
// sendmsg                
// sendto                
// set_mempolicy
// set_robust_list
// set_thread_area
// set_tid_address
// setdomainname
// setfsgid
// setfsgid32
// setfsuid
// setfsuid32
// setgid
// setgid32
// setgroups
// setgroups32
// sethostname
// setitimer
// setns
// setpgid
// setpgrp                     
// setpriority
// setregid
// setregid32
// setresgid
// setresgid32
// setresuid
// setresuid32
// setreuid
// setreuid32
// setrlimit
// setsid
// setsockopt                 
// settimeofday
// setuid
// setuid32
// setxattr
// sgetmask
// shmat                     
// shmctl                   
// shmdt                   
// shmget                 
// shutdown              
// sigaction
// sigaltstack
// signal
// signalfd
// signalfd4
// sigpending
// sigprocmask
// sigreturn
// sigsuspend
// socket                
// socketcall
// socketpair          
// splice
// ssetmask
// stat
// stat64
// statfs
// statfs64
// statx
// stime
// swapoff
// swapon
// symlink
// symlinkat
// sync
// sync_file_range
// sync_file_range2
// syncfs
// sysfs
// sysinfo
// syslog
// tee
// tgkill
// time
// timer_create
// timer_delete
// timer_getoverrun
// timer_gettime
// timer_settime
// timerfd_create
// timerfd_gettime
// timerfd_settime
// times
// tkill
// truncate
// truncate64
// ugetrlimit
// umask
// umount
// umount2
// uname
// unlink
// unlinkat
// unshare
// uselib
// ustat
// userfaultfd
// utime
// utimensat
// utimes
// vfork
// vhangup
// vm86
// vmsplice
// wait4
// waitid
// waitpid
// writev

#else
#error "Only x86-64 linux is supported"
#endif

// flags: -nostdlib -static -fno-stack-protector -g2 -O3 -Wall -Wextra
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
        long read_return_value = read(-1, read_buffer, 1);
        
        if (read_return_value >= 0) {
            return 1;
        }
    }

    {
        long mmap_return_value = mmap(0, 4096, 3, 0x22, -1, 0);
        
        if (mmap_return_value < 0) {
            return 1;
        }

        syscall2(SYS_munmap, mmap_return_value, 4096);
    }

    {
        char write_buffer[1];
        long write_return_value;

        write_buffer[0] = '\n';
        write_return_value = write(-1, write_buffer, 1);
        
        if (write_return_value >= 0) {
            return 1;
        }
    }

    {
        char *invalid_path = "/invalid_nonexistent_path_12345";
        long chdir_return_value = chdir(invalid_path);
        
        if (chdir_return_value >= 0) {
            return 1;
        }
    }

    {
        char *invalid_path = "";
        long mkdir_return_value = mkdir(invalid_path, 0777);
        
        if (mkdir_return_value >= 0) {
            return 1;
        }
    }

    {
        char *invalid_path = "";
        long rmdir_return_value = rmdir(invalid_path);
        
        if (rmdir_return_value >= 0) {
            return 1;
        }
    }

    return 0;
}
#endif /* TESTING_nolibc */

#endif /* NOLIBC_C */
