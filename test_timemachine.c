#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>
#include "timemachine.h"
#include "minunit.h"

#define CONF_FILE "test_timemachine.conf"
#define ENV_CONF_FILE "LIBTIMEMACHINE_CONF"

int set_delta(time_t delta)
{
    int retv = setenv(ENV_CONF_FILE, CONF_FILE, 1);
    if (retv != 0) {
        return -1;
    }
    FILE* file = fopen(CONF_FILE, "w");
    if (file == NULL) {
        return -2;
    }
    fprintf(file, "%d", (int)delta); // counting on time_t being the same size as int
    fclose(file);
    return 0;
}

void cleanup()
{
    unlink(CONF_FILE);
}

static char* test_time()
{
    {
        time_t a = 1;
        time_t b = 100;
        mu_assert_int_equals(time(0), timemachine_time(0));
        mu_assert_int_equals(time(&a), timemachine_time(&b));
        mu_assert_int_equals(a, b);
    }
    {
        int delta = 0;
        mu_assert_int_equals(set_delta(delta), 0);
        time_t a = 1;
        time_t b = 100;
        mu_assert_int_equals(time(0), timemachine_time(0) + delta);
        mu_assert_int_equals(time(&a), timemachine_time(&b) + delta);
        mu_assert_int_equals(a, b + delta);
        cleanup();
    }
    {
        int delta = -1;
        mu_assert_int_equals(set_delta(delta), 0);
        time_t a = 1;
        time_t b = 100;
        mu_assert_int_equals(time(0), timemachine_time(0) + delta);
        mu_assert_int_equals(time(&a), timemachine_time(&b) + delta);
        mu_assert_int_equals(a, b + delta);
        cleanup();
    }
    {
        int delta = INT_MIN;
        set_delta(delta);
        time_t a = 1;
        time_t b = 100;
        mu_assert_int_equals(time(0), timemachine_time(0) + delta);
        mu_assert_int_equals(time(&a), timemachine_time(&b) + delta);
        mu_assert_int_equals(a, b + delta);
        cleanup();
    }
    {
        int delta = 1;
        mu_assert_int_equals(set_delta(delta), 0);
        time_t a = 1;
        time_t b = 100;
        mu_assert_int_equals(time(0), timemachine_time(0) + delta);
        mu_assert_int_equals(time(&a), timemachine_time(&b) + delta);
        mu_assert_int_equals(a, b + delta);
        cleanup();
    }
    {
        int delta = INT_MAX;
        set_delta(delta);
        time_t a = 1;
        time_t b = 100;
        mu_assert_int_equals(time(0), timemachine_time(0) + delta);
        mu_assert_int_equals(time(&a), timemachine_time(&b) + delta);
        mu_assert_int_equals(a, b + delta);
        cleanup();
    }

    return 0;
}

#define _timeval(x) \
    struct timeval x; x.tv_sec = x.tv_usec = 0;

static char* test_gettimeofday()
{
    {
        _timeval(a);
        _timeval(b);
        mu_assert_int_equals(a.tv_sec, 0);
        mu_assert_int_equals(b.tv_sec, 0);
        mu_assert_int_equals(gettimeofday(&a, 0), 0);
        mu_assert_int_equals(timemachine_gettimeofday(&b, 0), 0);
        mu_assert_int_equals(a.tv_sec, b.tv_sec);
        mu_assert(a.tv_sec != 0);
        mu_assert(b.tv_sec != 0);
    }
    {
        _timeval(a);
        _timeval(b);
        time_t delta = 0;
        set_delta(delta);
        mu_assert_int_equals(gettimeofday(&a, 0), 0);
        mu_assert_int_equals(timemachine_gettimeofday(&b, 0), 0);
        mu_assert_int_equals(a.tv_sec, b.tv_sec);
        mu_assert(a.tv_sec != 0);
        mu_assert(b.tv_sec != 0);
        cleanup();
    }
    {
        _timeval(a);
        _timeval(b);
        time_t delta = -1;
        set_delta(delta);
        mu_assert_int_equals(gettimeofday(&a, 0), 0);
        mu_assert_int_equals(timemachine_gettimeofday(&b, 0), 0);
        mu_assert_int_equals(a.tv_sec, b.tv_sec + delta);
        mu_assert(a.tv_sec != 0);
        mu_assert(b.tv_sec != 0);
        cleanup();
    }
    {
        _timeval(a);
        _timeval(b);
        time_t delta = INT_MIN;
        set_delta(delta);
        mu_assert_int_equals(gettimeofday(&a, 0), 0);
        mu_assert_int_equals(timemachine_gettimeofday(&b, 0), 0);
        mu_assert_int_equals(a.tv_sec, b.tv_sec + delta);
        mu_assert(a.tv_sec != 0);
        mu_assert(b.tv_sec != 0);
        cleanup();
    }
    {
        _timeval(a);
        _timeval(b);
        time_t delta = 1;
        set_delta(delta);
        mu_assert_int_equals(gettimeofday(&a, 0), 0);
        mu_assert_int_equals(timemachine_gettimeofday(&b, 0), 0);
        mu_assert_int_equals(a.tv_sec, b.tv_sec + delta);
        mu_assert(a.tv_sec != 0);
        mu_assert(b.tv_sec != 0);
        cleanup();
    }
    {
        _timeval(a);
        _timeval(b);
        time_t delta = INT_MAX;
        set_delta(delta);
        mu_assert_int_equals(gettimeofday(&a, 0), 0);
        mu_assert_int_equals(timemachine_gettimeofday(&b, 0), 0);
        mu_assert_int_equals(a.tv_sec, b.tv_sec + delta);
        mu_assert(a.tv_sec != 0);
        mu_assert(b.tv_sec != 0);
        cleanup();
    }

    return 0;
}

#ifndef __APPLE__
#define _timespec(x) \
    struct timespec x; x.tv_sec = x.tv_nsec = 0;

static char* test_clock_gettime()
{
    {
        _timespec(a);
        _timespec(b);
        mu_assert_int_equals(a.tv_sec, 0);
        mu_assert_int_equals(b.tv_sec, 0);
        mu_assert_int_equals(clock_gettime(CLOCK_REALTIME, &a), 0);
        mu_assert_int_equals(timemachine_clock_gettime(CLOCK_REALTIME, &b), 0);
        mu_assert_int_equals(a.tv_sec, b.tv_sec);
        mu_assert(a.tv_sec != 0);
        mu_assert(b.tv_sec != 0);
    }
    {
        _timespec(a);
        _timespec(b);
        time_t delta = 0;
        set_delta(delta);
        mu_assert_int_equals(a.tv_sec, 0);
        mu_assert_int_equals(b.tv_sec, 0);
        mu_assert_int_equals(clock_gettime(CLOCK_REALTIME, &a), 0);
        mu_assert_int_equals(timemachine_clock_gettime(CLOCK_REALTIME, &b), 0);
        mu_assert_int_equals(a.tv_sec, b.tv_sec);
        mu_assert(a.tv_sec != 0);
        mu_assert(b.tv_sec != 0);
        cleanup();
    }
    {
        _timespec(a);
        _timespec(b);
        time_t delta = -1;
        set_delta(delta);
        mu_assert_int_equals(a.tv_sec, 0);
        mu_assert_int_equals(b.tv_sec, 0);
        mu_assert_int_equals(clock_gettime(CLOCK_REALTIME, &a), 0);
        mu_assert_int_equals(timemachine_clock_gettime(CLOCK_REALTIME, &b), 0);
        mu_assert_int_equals(a.tv_sec, b.tv_sec + delta);
        mu_assert(a.tv_sec != 0);
        mu_assert(b.tv_sec != 0);
        cleanup();
    }
    {
        _timespec(a);
        _timespec(b);
        time_t delta = INT_MIN;
        set_delta(delta);
        mu_assert_int_equals(a.tv_sec, 0);
        mu_assert_int_equals(b.tv_sec, 0);
        mu_assert_int_equals(clock_gettime(CLOCK_REALTIME, &a), 0);
        mu_assert_int_equals(timemachine_clock_gettime(CLOCK_REALTIME, &b), 0);
        mu_assert_int_equals(a.tv_sec, b.tv_sec + delta);
        mu_assert(a.tv_sec != 0);
        mu_assert(b.tv_sec != 0);
        cleanup();
    }
    {
        _timespec(a);
        _timespec(b);
        time_t delta = 1;
        set_delta(delta);
        mu_assert_int_equals(a.tv_sec, 0);
        mu_assert_int_equals(b.tv_sec, 0);
        mu_assert_int_equals(clock_gettime(CLOCK_REALTIME, &a), 0);
        mu_assert_int_equals(timemachine_clock_gettime(CLOCK_REALTIME, &b), 0);
        mu_assert_int_equals(a.tv_sec, b.tv_sec + delta);
        mu_assert(a.tv_sec != 0);
        mu_assert(b.tv_sec != 0);
        cleanup();
    }
    {
        _timespec(a);
        _timespec(b);
        time_t delta = INT_MAX;
        set_delta(delta);
        mu_assert_int_equals(a.tv_sec, 0);
        mu_assert_int_equals(b.tv_sec, 0);
        mu_assert_int_equals(clock_gettime(CLOCK_REALTIME, &a), 0);
        mu_assert_int_equals(timemachine_clock_gettime(CLOCK_REALTIME, &b), 0);
        mu_assert_int_equals(a.tv_sec, b.tv_sec + delta);
        mu_assert(a.tv_sec != 0);
        mu_assert(b.tv_sec != 0);
        cleanup();
    }

    return 0;
}
#endif

static char* all_tests()
{
    mu_run_test(test_time);
    mu_run_test(test_gettimeofday);
#ifndef __APPLE__
    mu_run_test(test_clock_gettime);
#endif
    return 0;
}

UNITTESTS
