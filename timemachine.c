#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <inttypes.h>
#include <dlfcn.h>

static time_t (*time_fn)(time_t*) = 0;
static int (*gettimeofday_fn)(struct timeval*, struct timezone*) = 0;
static int (*clock_gettime_fn)(clockid_t, struct timespec*) = 0;
static char* delta_file = "/tmp/libtime.conf";
static void* libc = 0;
static void* librt = 0;

#define BUFFER_SIZE 128
#define LIBC "libc.so.6"
#define LIBRT "librt.so.1"

static time_t get_delta()
{
    time_t delta = 0;
    char buffer[BUFFER_SIZE] = { 0 };
    int fd = open(delta_file, O_RDONLY);    
    if (fd != -1) {
        ssize_t nread = read(fd, buffer, (BUFFER_SIZE - 1));
        if (nread > 0) {
            sscanf(buffer, "%"SCNi64, &delta);
        }
        close(fd);
    }
    return delta;
}

time_t time(time_t* t)
{
    time_t delta = get_delta();
    time_t now = 0;
    if (time_fn == 0) {
        if (!libc) {
            libc = dlopen(LIBC, RTLD_LAZY);
        }
        if (libc) {
            dlerror();
            *(void**)(&time_fn) = dlsym(libc, "time");
        }
    }
    if (time_fn) {
        now = (*time_fn)(t);
        now += delta;
    }
    return now;
}

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    time_t delta = get_delta();
    int retv = -1;
    if (gettimeofday_fn == 0) {
        if (!libc) {
            libc = dlopen(LIBC, RTLD_LAZY);
        }
        if (libc) {
            dlerror();
            *(void**)(&gettimeofday_fn) = dlsym(libc, "gettimeofday");
        }
    }
    if (gettimeofday_fn) {
        retv = (*gettimeofday_fn)(tv, tz);
        if (retv == 0 && tv) {
            tv->tv_sec += delta;
        }
    }
    return retv;
}

int clock_gettime(clockid_t clk_id, struct timespec *tp)
{
    time_t delta = get_delta();
    int retv = -1;
    if (clock_gettime_fn == 0) {
        if (!librt) {
            librt = dlopen(LIBRT, RTLD_LAZY);
        }
        if (librt) {
            dlerror();
            *(void**)(&clock_gettime_fn) = dlsym(librt, "clock_gettime");
        }
    }
    if (clock_gettime_fn) {
        retv = (*clock_gettime_fn)(clk_id, tp);
        if (retv == 0 && tp) {
            tp->tv_sec += delta;
        }
    }
    return retv;
}
