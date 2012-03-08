#ifndef __TIMEMACHINE_H__
#define __TIMEMACHINE_H__
// \brief this header only needs to be included if you want the ability to call
// the real functions themselves which are not overridden.
#include <sys/time.h>
#include <time.h>

// \brief alias to the real time function
time_t timemachine_time(time_t* t);

// \brief alias to the real gettimeofday function
int timemachine_gettimeofday(struct timeval *tv, struct timezone *tz);

// \brief alias to the real clock_gettime function
int timemachine_clock_gettime(clockid_t clk_id, struct timespec *tp);

#endif // __TIMEMACHINE_H__
