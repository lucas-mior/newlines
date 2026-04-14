#if !defined(NOLIBC_C)
#define NOLIBC_C

#define const

#include <sys/syscall.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#if defined(__INCLUDE_LEVEL__) && (__INCLUDE_LEVEL__ == 0)
#define TESTING_nolibc 1
#elif !defined(TESTING_nolibc)
#define TESTING_nolibc 0
#endif

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

static long syscall0(long n);
static long syscall1(long n, long a1);
static long syscall2(long n, long a1, long a2);
static long syscall3(long n, long a1, long a2, long a3);
static long syscall4(long n, long a1, long a2, long a3, long a4);
static long syscall5(long n, long a1, long a2, long a3, long a4, long a5);
static long syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6);

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
write(int file_descriptor, void *buffer, size_t count) {
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

void *
mmap(void *address, size_t length, int protection, int flags, int file_descriptor, long offset) {
    return (void *)syscall6(SYS_mmap, (long)address, length, protection, flags, file_descriptor, offset);
}

int
munmap(void *address, size_t length) {
    return syscall2(SYS_munmap, (long)address, (long)length);
}

int
mprotect(void *address, size_t length, int protection) {
    return syscall3(SYS_mprotect, (long)address, length, protection);
}

int __attribute__((always_inline))
brk(void *address) {
    return syscall1(SYS_brk, (long)address);
}

int
chdir(char *path) {
    return syscall1(SYS_chdir, (long)path);
}

int
mkdir(char *pathname, mode_t mode) {
    return syscall2(SYS_mkdir, (long)pathname, mode);
}

int
rmdir(char *pathname) {
    return syscall1(SYS_rmdir, (long)pathname);
}

int
unlink(char *pathname) {
    return syscall1(SYS_unlink, (long)pathname);
}

int
dup(int oldfd) {
    return syscall1(SYS_dup, oldfd);
}

int
dup2(int oldfd, int newfd) {
    return syscall2(SYS_dup2, oldfd, newfd);
}

int
dup3(int oldfd, int newfd, int flags) {
    return syscall3(SYS_dup3, oldfd, newfd, flags);
}

int
fork(void) {
    return syscall0(SYS_fork);
}

int
vfork(void) {
    return syscall0(SYS_vfork);
}

int
wait4(int pid, int *wstatus, int options, void *rusage) {
    return syscall4(SYS_wait4, pid, (long)wstatus, options, (long)rusage);
}

int
waitpid(int pid, int *wstatus, int options) {
    return syscall4(SYS_wait4, pid, (long)wstatus, options, 0);
}

int
wait(int *wstatus) {
    return syscall4(SYS_wait4, -1, (long)wstatus, 0, 0);
}

int
waitid(int which, int pid, void *infop, int options, void *ru) {
    return syscall5(SYS_waitid, which, pid, (long)infop, options, (long)ru);
}

int
getcwd2(char *buf, size_t size) {
    return syscall2(SYS_getcwd, (long)buf, size);
}

int
rename(char *oldpath, char *newpath) {
    return syscall2(SYS_rename, (long)oldpath, (long)newpath);
}

int
access(char *pathname, int mode) {
    return syscall2(SYS_access, (long)pathname, mode);
}

int
chmod(char *pathname, mode_t mode) {
    return syscall2(SYS_chmod, (long)pathname, mode);
}

int
fstat(int fd, struct stat *statbuf) {
    return syscall2(SYS_fstat, fd, (long)statbuf);
}

int
pipe(int pipefd[2]) {
    return syscall1(SYS_pipe, (long)pipefd);
}

int
execve(char *pathname, char **argv, char **envp) {
    return syscall3(SYS_execve, (long)pathname, (long)argv, (long)envp);
}

int
stat(char *pathname, struct stat *statbuf) {
    return syscall2(SYS_stat, (long)pathname, (long)statbuf);
}

int
stat64(char *pathname, struct stat *statbuf) {
    return syscall2(SYS_stat, (long)pathname, (long)statbuf);
}

int
lstat(char *pathname, struct stat *statbuf) {
    return syscall2(SYS_lstat, (long)pathname, (long)statbuf);
}

int
lstat64(char *pathname, struct stat *statbuf) {
    return syscall2(SYS_lstat, (long)pathname, (long)statbuf);
}

int
fstat64(int fd, struct stat *statbuf) {
    return syscall2(SYS_fstat, fd, (long)statbuf);
}

int
statfs(char *path, void *buf) {
    return syscall2(SYS_statfs, (long)path, (long)buf);
}

int
statfs64(char *path, void *buf) {
    return syscall2(SYS_statfs, (long)path, (long)buf);
}

int
fstatfs(int fd, void *buf) {
    return syscall2(SYS_fstatfs, fd, (long)buf);
}

int
fstatfs64(int fd, void *buf) {
    return syscall2(SYS_fstatfs, fd, (long)buf);
}

int
newfstatat(int dirfd, char *pathname, struct stat *statbuf, int flags) {
    return syscall4(SYS_newfstatat, dirfd, (long)pathname, (long)statbuf, flags);
}

int
fstatat64(int dirfd, char *pathname, struct stat *statbuf, int flags) {
    return syscall4(SYS_newfstatat, dirfd, (long)pathname, (long)statbuf, flags);
}

int
statx(int dirfd, char *pathname, int flags, unsigned int mask, void *statxbuf) {
    return syscall5(SYS_statx, dirfd, (long)pathname, flags, mask, (long)statxbuf);
}

uint
alarm(unsigned int seconds) {
    return syscall1(SYS_alarm, seconds);
}

int
chroot(char *path) {
    return syscall1(SYS_chroot, (long)path);
}

int
fchdir(int fd) {
    return syscall1(SYS_fchdir, fd);
}

int
fdatasync(int fd) {
    return syscall1(SYS_fdatasync, fd);
}

int
fsync(int fd) {
    return syscall1(SYS_fsync, fd);
}

int
getpid(void) {
    return syscall0(SYS_getpid);
}

int
getppid(void) {
    return syscall0(SYS_getppid);
}

uint
getuid(void) {
    return syscall0(SYS_getuid);
}

uint
geteuid(void) {
    return syscall0(SYS_geteuid);
}

uint
getgid(void) {
    return syscall0(SYS_getgid);
}

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

// TODO: syscalls
// _llseek
// _newselect
// accept                      
// accept4
// acct
// add_key
// adjtimex
// arch_prctl                  
// bind                       
// bpf
// capget
// capset
// chown                      
// chown32
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
// epoll_create
// epoll_create1
// epoll_ctl
// epoll_pwait
// epoll_pwait2
// epoll_wait
// eventfd
// eventfd2
// execveat
// exit_group
// faccessat
// faccessat2
// fadvise64
// fadvise64_64
// fallocate
// fanotify_init
// fanotify_mark
// fchmod
// fchmodat
// fchown
// fchown32
// fchownat
// fcntl
// fcntl64
// fgetxattr
// finit_module
// flistxattr
// flock
// fremovexattr
// fsconfig
// fsetxattr
// fsmount
// fsopen
// fspick
// ftruncate
// ftruncate64
// futex
// futimesat
// get_mempolicy
// get_robust_list
// get_thread_area
// getcpu
// getdents
// getdents64
// getegid32
// geteuid32
// getgid32
// getgroups
// getgroups32
// getitimer
// getpeername            
// getpgid
// getpgrp
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
// unlinkat
// unshare
// uselib
// ustat
// userfaultfd
// utime
// utimensat
// utimes
// vhangup
// vm86
// vmsplice
// writev

#else
#error "Only x86-64 linux is supported"
#endif

// flags: -fpermissive -nostdlib -static -fno-stack-protector -g2 -O3 -Wall -Wextra
#if TESTING_nolibc
int
main(void) {
    {
        long process_id = syscall0(SYS_getpid);
        long kill_return_value = 0;

        if (process_id < 0) {
            return 1;
        }

        kill_return_value = syscall2(SYS_kill, process_id, 0);
        if (kill_return_value < 0) {
            return 1;
        }
    }

    {
        long brk_return_value = brk((void *)0);

        if (brk_return_value < 0) {
            return 1;
        }
    }

    {
        char *dir_name = "dummy_test_dir";
        long mkdir_return_value = mkdir(dir_name, 0777);

        if (mkdir_return_value < 0) {
            return 1;
        }
    }

    {
        char *dir_name = "dummy_test_dir";
        long chdir_return_value = chdir(dir_name);

        if (chdir_return_value < 0) {
            return 1;
        }
    }

    {
        char *file_name = "dummy_file.txt";
        long fd = open(file_name, 66, 0666);
        char write_buffer[5] = {'t', 'e', 's', 't', '\n'};
        long write_success = 0;
        long lseek_return_value = 0;
        char read_buffer[5] = {0};
        long read_success = 0;
        void *mmap_return_value = 0;
        char *mapped_memory = 0;
        long munmap_return_value = 0;
        long close_return_value = 0;
        long unlink_return_value = 0;

        if (fd < 0) {
            return 1;
        }

        write_success = write(fd, write_buffer, 5);
        if (write_success < 0) {
            return 1;
        }

        lseek_return_value = lseek(fd, 0, 0);
        if (lseek_return_value < 0) {
            return 1;
        }

        read_success = read(fd, read_buffer, 5);
        if (read_success < 0) {
            return 1;
        }

        mmap_return_value = mmap((void *)0, 4096, 1, 1, fd, 0);
        if (mmap_return_value == MAP_FAILED) {
            return 1;
        }

        mapped_memory = (char *)mmap_return_value;
        if (mapped_memory[0] != 't') {
            return 1;
        }

        mprotect(mapped_memory, 4096, PROT_READ|PROT_WRITE);

        munmap_return_value = munmap((void *)mmap_return_value, 4096);
        if (munmap_return_value < 0) {
            return 1;
        }

        close_return_value = close(fd);
        if (close_return_value < 0) {
            return 1;
        }

        unlink_return_value = unlink(file_name);
        if (unlink_return_value < 0) {
            return 1;
        }
    }

    {
        char *parent_dir = "..";
        long chdir_return_value = chdir(parent_dir);

        if (chdir_return_value < 0) {
            return 1;
        }
    }

    {
        char *dir_name = "dummy_test_dir";
        long rmdir_return_value = rmdir(dir_name);

        if (rmdir_return_value < 0) {
            return 1;
        }
    }

    {
        long dup_return_value = dup(0);

        if (dup_return_value < 0) {
            return 1;
        }

        close(dup_return_value);
    }

    {
        long dup2_return_value = dup2(0, 100);

        if (dup2_return_value < 0) {
            return 1;
        }

        close(dup2_return_value);
    }

    {
        long dup3_return_value = dup3(0, 101, 0);

        if (dup3_return_value < 0) {
            return 1;
        }

        close(dup3_return_value);
    }

    {
        long fork_return_value = fork();

        if (fork_return_value < 0) {
            return 1;
        }

        if (fork_return_value == 0) {
            exit(0);
        }
    }

    {
        long vfork_return_value = vfork();

        if (vfork_return_value < 0) {
            return 1;
        }

        if (vfork_return_value == 0) {
            exit(0);
        }
    }

    {
        long child_pid = fork();
        int wstatus = 0;
        long wait_return_value = 0;

        if (child_pid < 0) {
            return 1;
        }

        if (child_pid == 0) {
            exit(0);
        }

        wait_return_value = wait(&wstatus);
        if (wait_return_value < 0) {
            return 1;
        }
    }

    {
        long child_pid = fork();
        int wstatus = 0;
        long waitpid_return_value = 0;

        if (child_pid < 0) {
            return 1;
        }

        if (child_pid == 0) {
            exit(0);
        }

        waitpid_return_value = waitpid((int)child_pid, &wstatus, 0);
        if (waitpid_return_value < 0) {
            return 1;
        }
    }

    {
        long child_pid = fork();
        char siginfo_buffer[128];
        long waitid_return_value = 0;

        if (child_pid < 0) {
            return 1;
        }

        if (child_pid == 0) {
            exit(0);
        }

        waitid_return_value = waitid(1, (int)child_pid, siginfo_buffer, 4, (void *)0);
        if (waitid_return_value < 0) {
            return 1;
        }
    }

    {
        char cwd_buffer[4096];
        long getcwd_return_value = getcwd2(cwd_buffer, 4096);

        if (getcwd_return_value < 0) {
            return 1;
        }
    }

    {
        char *old_file_name = "test_rename_old.txt";
        char *new_file_name = "test_rename_new.txt";
        long fd = open(old_file_name, 66, 0666);
        long rename_return_value = 0;
        long unlink_return_value = 0;

        if (fd < 0) {
            return 1;
        }

        close(fd);

        rename_return_value = rename(old_file_name, new_file_name);
        if (rename_return_value < 0) {
            unlink(old_file_name);
            return 1;
        }

        unlink_return_value = unlink(new_file_name);
        if (unlink_return_value < 0) {
            return 1;
        }
    }

    {
        char *dir_name = ".";
        long access_return_value = access(dir_name, 0);

        if (access_return_value < 0) {
            return 1;
        }
    }

    {
        char *file_name = "dummy_chmod_file.txt";
        long fd = open(file_name, 66, 0666);
        long chmod_return_value = 0;
        long unlink_return_value = 0;

        if (fd < 0) {
            return 1;
        }

        close(fd);

        chmod_return_value = chmod(file_name, 0777);
        if (chmod_return_value < 0) {
            unlink(file_name);
            return 1;
        }

        unlink_return_value = unlink(file_name);
        if (unlink_return_value < 0) {
            return 1;
        }
    }

    {
        struct stat stat_buffer = {0};
        long fstat_return_value = fstat(0, &stat_buffer);

        if (fstat_return_value < 0) {
            return 1;
        }
    }

    {
        int pipe_fds[2] = {-1, -1};
        long pipe_return_value = pipe(pipe_fds);

        if (pipe_return_value < 0) {
            return 1;
        }

        close(pipe_fds[0]);
        close(pipe_fds[1]);
    }

    {
        char *invalid_path = "/invalid_exec_path_12345";
        char *argv[1] = {0};
        char *envp[1] = {0};
        long execve_return_value = execve(invalid_path, argv, envp);

        if (execve_return_value >= 0) {
            return 1;
        }
    }

    {
        struct stat stat_buffer = {0};
        char *dir_name = ".";
        long stat_return_value = stat(dir_name, &stat_buffer);

        if (stat_return_value < 0) {
            return 1;
        }
    }

    {
        struct stat stat_buffer = {0};
        char *dir_name = ".";
        long stat64_return_value = stat64(dir_name, &stat_buffer);

        if (stat64_return_value < 0) {
            return 1;
        }
    }

    {
        struct stat stat_buffer = {0};
        char *dir_name = ".";
        long lstat_return_value = lstat(dir_name, &stat_buffer);

        if (lstat_return_value < 0) {
            return 1;
        }
    }

    {
        struct stat stat_buffer = {0};
        char *dir_name = ".";
        long lstat64_return_value = lstat64(dir_name, &stat_buffer);

        if (lstat64_return_value < 0) {
            return 1;
        }
    }

    {
        struct stat stat_buffer = {0};
        long fstat64_return_value = fstat64(0, &stat_buffer);

        if (fstat64_return_value < 0) {
            return 1;
        }
    }

    {
        char statfs_buffer[120] = {0};
        char *dir_name = ".";
        long statfs_return_value = statfs(dir_name, statfs_buffer);

        if (statfs_return_value < 0) {
            return 1;
        }
    }

    {
        char statfs_buffer[120] = {0};
        char *dir_name = ".";
        long statfs64_return_value = statfs64(dir_name, statfs_buffer);

        if (statfs64_return_value < 0) {
            return 1;
        }
    }

    {
        char statfs_buffer[120] = {0};
        long fstatfs_return_value = fstatfs(0, statfs_buffer);

        if (fstatfs_return_value < 0) {
            return 1;
        }
    }

    {
        char statfs_buffer[120] = {0};
        long fstatfs64_return_value = fstatfs64(0, statfs_buffer);

        if (fstatfs64_return_value < 0) {
            return 1;
        }
    }

    {
        struct stat stat_buffer = {0};
        char *dir_name = ".";
        long newfstatat_return_value = newfstatat(-100, dir_name, &stat_buffer, 0);

        if (newfstatat_return_value < 0) {
            return 1;
        }
    }

    {
        struct stat stat_buffer = {0};
        char *dir_name = ".";
        long fstatat64_return_value = fstatat64(-100, dir_name, &stat_buffer, 0);

        if (fstatat64_return_value < 0) {
            return 1;
        }
    }

    {
        char statx_buffer[256] = {0};
        char *dir_name = ".";
        long statx_return_value = statx(-100, dir_name, 0, 0x7ff, statx_buffer);

        if (statx_return_value < 0) {
            return 1;
        }
    }

    return 0;
}
#endif /* TESTING_nolibc */

#endif /* NOLIBC_C */
