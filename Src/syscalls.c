/**
 ******************************************************************************
  * File Name          : syscalls.c
  * Description        : Support files for GNU libc. Files in the system
  *                      namespace go here. Files in the C namespace (ie those
  *                      that do not start with an underscore) go in .c.
  ******************************************************************************
  */

#ifdef __cplusplus
 extern "C" {
#endif

#include "debug_terminal.h"

/* Support files for GNU libc.  Files in the system namespace go here.
   Files in the C namespace (ie those that do not start with an
   underscore) go in .c.  */

#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <errno.h>
#include <reent.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_STACK_SIZE 0x2000

extern int __io_putchar(int ch) __attribute__((weak));
extern int __io_getchar(void) __attribute__((weak));

// defined in heap_useNewlib.c
#if 0
caddr_t _sbrk(int incr)
{
    extern char end asm("end");
    static char *heap_end;
    char *prev_heap_end,*min_stack_ptr;

    if (heap_end == 0)
        heap_end = &end;

    prev_heap_end = heap_end;

    /* Use the NVIC offset register to locate the main stack pointer. */
    min_stack_ptr = (char*)(*(unsigned int *)*(unsigned int *)0xE000ED08);
    /* Locate the STACK bottom address */
    min_stack_ptr -= MAX_STACK_SIZE;

    if (heap_end + incr > min_stack_ptr)
    {
        errno = ENOMEM;
        return (caddr_t) -1;
    }

    heap_end += incr;

    return (caddr_t) prev_heap_end;
}
#endif

/*
 * _gettimeofday primitive (Stub function)
 * */

int _gettimeofday(struct timeval *tp, struct timezone *tzp) {
    /* Return fixed data for the timezone.  */
    if (tzp) {
        tzp->tz_minuteswest = 0;
        tzp->tz_dsttime = 0;
    }

    return 0;
}

void initialise_monitor_handles() {
}

int _getpid(void) {
    return 1;
}

int _kill(int pid, int sig) {
    errno = EINVAL;
    return -1;
}

void _exit(int status) {
    _kill(status, -1);
    while (1) {}
}

int _write(int file, char *ptr, int len) {
//    if (__get_IPSR() != 0) {
//        // 处于中断模式
//        return 0;
//    }
    switch (file) {
        case STDOUT_FILENO:
        case STDERR_FILENO:
            return debug_socket_write(ptr, len);
    }
}

int _close(int file) {
    return -1;
}

int _fstat(int file, struct stat *st) {
    // memset(st, 0, sizeof(*st));
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file) {
    return 1;
}

int _lseek(int file, int ptr, int dir) {
    return 0;
}

int _read(int file, char *ptr, int len) {
//    if (__get_IPSR() != 0) {
//        // 处于中断模式
//        return 0;
//    }
    switch (file) {
    case STDIN_FILENO:
        return debug_socket_read(ptr, len);
    }
}

int _open(char *path, int flags, ...) {
    /* Pretend like we always fail */
    return -1;
}

int _wait(int *status) {
    errno = ECHILD;
    return -1;
}

int _unlink(char *name) {
    errno = ENOENT;
    return -1;
}

int _times(struct tms *buf) {
    return -1;
}

int _stat(char *file, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

int _link(char *old, char *new) {
    errno = EMLINK;
    return -1;
}

int _fork(void) {
    errno = EAGAIN;
    return -1;
}

int _execve(char *name, char **argv, char **env) {
    errno = ENOMEM;
    return -1;
}

#ifdef __cplusplus
}
#endif


