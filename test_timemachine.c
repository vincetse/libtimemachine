#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>
#include <check.h>
#include "timemachine.h"

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

START_TEST (test_time)
{
    {
        time_t a = 1;
        time_t b = 100;
        fail_unless(time(0) == timemachine_time(0));
        fail_unless(time(&a) == timemachine_time(&b));
        fail_unless(a == b);
    }
    {
        int delta = 0;
        fail_unless(set_delta(delta) == 0);
        time_t a = 1;
        time_t b = 100;
        fail_unless(time(0) == timemachine_time(0) + delta);
        fail_unless(time(&a) == timemachine_time(&b) + delta);
        fail_unless(a == b + delta);
        cleanup();
    }
    {
        int delta = -1;
        fail_unless(set_delta(delta) == 0);
        time_t a = 1;
        time_t b = 100;
        fail_unless(time(0) == timemachine_time(0) + delta);
        fail_unless(time(&a) == timemachine_time(&b) + delta);
        fail_unless(a == b + delta);
        cleanup();
    }
    {
        int delta = INT_MIN;
        set_delta(delta);
        time_t a = 1;
        time_t b = 100;
        fail_unless(time(0) == timemachine_time(0) + delta);
        fail_unless(time(&a) == timemachine_time(&b) + delta);
        fail_unless(a == b + delta);
        cleanup();
    }
    {
        int delta = 1;
        fail_unless(set_delta(delta) == 0);
        time_t a = 1;
        time_t b = 100;
        fail_unless(time(0) == timemachine_time(0) + delta);
        fail_unless(time(&a) == timemachine_time(&b) + delta);
        fail_unless(a == b + delta);
        cleanup();
    }
    {
        int delta = INT_MAX;
        set_delta(delta);
        time_t a = 1;
        time_t b = 100;
        fail_unless(time(0) == timemachine_time(0) + delta);
        fail_unless(time(&a) == timemachine_time(&b) + delta);
        fail_unless(a == b + delta);
        cleanup();
    }
}
END_TEST

#define _timeval(x) \
    struct timeval x; x.tv_sec = x.tv_usec = 0;

START_TEST (test_gettimeofday)
{
    {
        _timeval(a);
        _timeval(b);
        fail_unless(a.tv_sec == 0);
        fail_unless(b.tv_sec == 0);
        fail_unless(gettimeofday(&a, 0) == 0);
        fail_unless(timemachine_gettimeofday(&b, 0) == 0);
        fail_unless(a.tv_sec == b.tv_sec);
        fail_if(a.tv_sec == 0);
        fail_if(b.tv_sec == 0);
    }
    {
        _timeval(a);
        _timeval(b);
        time_t delta = 0;
        set_delta(delta);
        fail_unless(gettimeofday(&a, 0) == 0);
        fail_unless(timemachine_gettimeofday(&b, 0) == 0);
        fail_unless(a.tv_sec == b.tv_sec);
        fail_if(a.tv_sec == 0);
        fail_if(b.tv_sec == 0);
        cleanup();
    }
    {
        _timeval(a);
        _timeval(b);
        time_t delta = -1;
        set_delta(delta);
        fail_unless(gettimeofday(&a, 0) == 0);
        fail_unless(timemachine_gettimeofday(&b, 0) == 0);
        fail_unless(a.tv_sec == b.tv_sec + delta);
        fail_if(a.tv_sec == 0);
        fail_if(b.tv_sec == 0);
        cleanup();
    }
    {
        _timeval(a);
        _timeval(b);
        time_t delta = INT_MIN;
        set_delta(delta);
        fail_unless(gettimeofday(&a, 0) == 0);
        fail_unless(timemachine_gettimeofday(&b, 0) == 0);
        fail_unless(a.tv_sec == b.tv_sec + delta);
        fail_if(a.tv_sec == 0);
        fail_if(b.tv_sec == 0);
        cleanup();
    }
    {
        _timeval(a);
        _timeval(b);
        time_t delta = 1;
        set_delta(delta);
        fail_unless(gettimeofday(&a, 0) == 0);
        fail_unless(timemachine_gettimeofday(&b, 0) == 0);
        fail_unless(a.tv_sec == b.tv_sec + delta);
        fail_if(a.tv_sec == 0);
        fail_if(b.tv_sec == 0);
        cleanup();
    }
    {
        _timeval(a);
        _timeval(b);
        time_t delta = INT_MAX;
        set_delta(delta);
        fail_unless(gettimeofday(&a, 0) == 0);
        fail_unless(timemachine_gettimeofday(&b, 0) == 0);
        fail_unless(a.tv_sec == b.tv_sec + delta);
        fail_if(a.tv_sec == 0);
        fail_if(b.tv_sec == 0);
        cleanup();
    }
}
END_TEST

#define _timespec(x) \
    struct timespec x; x.tv_sec = x.tv_nsec = 0;

START_TEST (test_clock_gettime)
{
    {
        _timespec(a);
        _timespec(b);
        fail_unless(a.tv_sec == 0);
        fail_unless(b.tv_sec == 0);
        fail_unless(clock_gettime(CLOCK_REALTIME, &a) == 0);
        fail_unless(timemachine_clock_gettime(CLOCK_REALTIME, &b) == 0);
        fail_unless(a.tv_sec == b.tv_sec);
        fail_if(a.tv_sec == 0);
        fail_if(b.tv_sec == 0);
    }
    {
        _timespec(a);
        _timespec(b);
        time_t delta = 0;
        set_delta(delta);
        fail_unless(a.tv_sec == 0);
        fail_unless(b.tv_sec == 0);
        fail_unless(clock_gettime(CLOCK_REALTIME, &a) == 0);
        fail_unless(timemachine_clock_gettime(CLOCK_REALTIME, &b) == 0);
        fail_unless(a.tv_sec == b.tv_sec);
        fail_if(a.tv_sec == 0);
        fail_if(b.tv_sec == 0);
        cleanup();
    }
    {
        _timespec(a);
        _timespec(b);
        time_t delta = -1;
        set_delta(delta);
        fail_unless(a.tv_sec == 0);
        fail_unless(b.tv_sec == 0);
        fail_unless(clock_gettime(CLOCK_REALTIME, &a) == 0);
        fail_unless(timemachine_clock_gettime(CLOCK_REALTIME, &b) == 0);
        fail_unless(a.tv_sec == b.tv_sec + delta);
        fail_if(a.tv_sec == 0);
        fail_if(b.tv_sec == 0);
        cleanup();
    }
    {
        _timespec(a);
        _timespec(b);
        time_t delta = INT_MIN;
        set_delta(delta);
        fail_unless(a.tv_sec == 0);
        fail_unless(b.tv_sec == 0);
        fail_unless(clock_gettime(CLOCK_REALTIME, &a) == 0);
        fail_unless(timemachine_clock_gettime(CLOCK_REALTIME, &b) == 0);
        fail_unless(a.tv_sec == b.tv_sec + delta);
        fail_if(a.tv_sec == 0);
        fail_if(b.tv_sec == 0);
        cleanup();
    }
    {
        _timespec(a);
        _timespec(b);
        time_t delta = 1;
        set_delta(delta);
        fail_unless(a.tv_sec == 0);
        fail_unless(b.tv_sec == 0);
        fail_unless(clock_gettime(CLOCK_REALTIME, &a) == 0);
        fail_unless(timemachine_clock_gettime(CLOCK_REALTIME, &b) == 0);
        fail_unless(a.tv_sec == b.tv_sec + delta);
        fail_if(a.tv_sec == 0);
        fail_if(b.tv_sec == 0);
        cleanup();
    }
    {
        _timespec(a);
        _timespec(b);
        time_t delta = INT_MAX;
        set_delta(delta);
        fail_unless(a.tv_sec == 0);
        fail_unless(b.tv_sec == 0);
        fail_unless(clock_gettime(CLOCK_REALTIME, &a) == 0);
        fail_unless(timemachine_clock_gettime(CLOCK_REALTIME, &b) == 0);
        fail_unless(a.tv_sec == b.tv_sec + delta);
        fail_if(a.tv_sec == 0);
        fail_if(b.tv_sec == 0);
        cleanup();
    }
}
END_TEST

Suite* test_suite(void)
{
    Suite* s = suite_create("libtimemachine");
    TCase* tc_core = tcase_create("core");
    tcase_add_test(tc_core, test_time);
    tcase_add_test(tc_core, test_gettimeofday);
    tcase_add_test(tc_core, test_clock_gettime);
    suite_add_tcase(s, tc_core);
    return s;
}

int main(void)
{
    int nfailed = 0;
    Suite* s = test_suite();
    SRunner* sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    nfailed = srunner_ntests_failed (sr);
    srunner_free(sr);
    return (nfailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
