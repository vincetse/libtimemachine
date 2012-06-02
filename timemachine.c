#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <inttypes.h>
#include <dlfcn.h>
#include <stdlib.h>

// function pointers
static time_t (*time_fn)(time_t*) = 0;
static int (*gettimeofday_fn)(struct timeval*, struct timezone*) = 0;

#ifndef __APPLE__
static int (*clock_gettime_fn)(clockid_t, struct timespec*) = 0;
#endif

// configuration
#define ENV_CONF_FILE "LIBTIMEMACHINE_CONF"
#define DEFAULT_CONF_FILE "/tmp/libtimemachine.conf"

// pointers to the libraries
static void* libc = 0;
#ifndef __APPLE__
#define LIBC "libc.so.6"
#else
#define LIBC "libc.dylib"
#endif

#ifndef __APPLE__
static void* librt = 0;
#define LIBRT "librt.so.1"
#endif

#define BUFFER_SIZE 128

//////////////////////////////////////////////////////////////////////////////
// utils
//////////////////////////////////////////////////////////////////////////////
static char* get_conf_file()
{
    char* ev = getenv(ENV_CONF_FILE);
    char* conf_file = DEFAULT_CONF_FILE;
    if (ev) {
        conf_file = ev;
    }
    return conf_file;
}

static time_t get_delta()
{
    int delta = 0;
    char buffer[BUFFER_SIZE] = { 0 };
    char* conf_file = get_conf_file();
    int fd = open(conf_file, O_RDONLY);
    if (fd != -1) {
        ssize_t nread = read(fd, buffer, (BUFFER_SIZE - 1));
        if (nread > 0) {
            sscanf(buffer, "%d", &delta);
        }
        close(fd);
    }
    return (time_t)delta;
}

#define setup_func_pointer(lib_ptr, lib_name, func_ptr, func_name) \
    if (func_ptr == 0) { \
        if (lib_ptr == 0) { \
            lib_ptr = dlopen(lib_name, RTLD_LAZY); \
        } \
        if (lib_ptr != 0) { \
            dlerror(); \
            *(void**)(&func_ptr) = dlsym(lib_ptr, func_name); \
        } \
    }

//////////////////////////////////////////////////////////////////////////////
time_t timemachine_time(time_t* t)
//////////////////////////////////////////////////////////////////////////////
{
    time_t now = 0;
    setup_func_pointer(libc, LIBC, time_fn, "time");
    if (time_fn) {
        now = (*time_fn)(t);
    }
    return now;
}

//////////////////////////////////////////////////////////////////////////////
time_t time(time_t* t)
//////////////////////////////////////////////////////////////////////////////
{
    time_t delta = get_delta();
    time_t now = timemachine_time(t) + delta;
    if (t) {
        (*t) = now;
    }
    return now;
}

//////////////////////////////////////////////////////////////////////////////
int timemachine_gettimeofday(struct timeval *tv, struct timezone *tz)
//////////////////////////////////////////////////////////////////////////////
{
    int retv = -1;
    setup_func_pointer(libc, LIBC, gettimeofday_fn, "gettimeofday");
    if (gettimeofday_fn) {
        retv = (*gettimeofday_fn)(tv, tz);
    }
    return retv;
}

//////////////////////////////////////////////////////////////////////////////
#ifdef __APPLE__
int gettimeofday(struct timeval *tv, void *tz)
#else
int gettimeofday(struct timeval *tv, struct timezone *tz)
#endif
//////////////////////////////////////////////////////////////////////////////
{
    time_t delta = get_delta();
    int retv = timemachine_gettimeofday(tv, tz);
    if (retv == 0 && tv) {
        tv->tv_sec += delta;
    }
    return retv;
}

#ifndef __APPLE__
//////////////////////////////////////////////////////////////////////////////
int timemachine_clock_gettime(clockid_t clk_id, struct timespec *tp)
//////////////////////////////////////////////////////////////////////////////
{
    int retv = -1;
    setup_func_pointer(librt, LIBRT, clock_gettime_fn, "clock_gettime");
    if (clock_gettime_fn) {
        retv = (*clock_gettime_fn)(clk_id, tp);
    }
    return retv;
}

//////////////////////////////////////////////////////////////////////////////
int clock_gettime(clockid_t clk_id, struct timespec *tp)
//////////////////////////////////////////////////////////////////////////////
{
    time_t delta = get_delta();
    int retv = timemachine_clock_gettime(clk_id, tp);
    if (retv == 0 && tp) {
        tp->tv_sec += delta;
    }
    return retv;
}

#endif
