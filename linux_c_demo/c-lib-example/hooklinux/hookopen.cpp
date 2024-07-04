#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>

int open(const char *path, int flags, ...)
{
    printf("flags %d\n");
    typedef int (*orig_open_func_type)(const char *, int, ...);
    orig_open_func_type orig_open;
    orig_open = (orig_open_func_type)dlsym(RTLD_NEXT, "open");

    printf("opening file: %s\n", path);

    if (flags & O_CREAT)
    {
        va_list args;
        va_start(args, flags);
        
        mode_t mode = va_arg(args, mode_t);
        va_end(args);
        printf("with mode: %o\n", mode);
    }
    return orig_open(path, flags);
}