#include <sys/time.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char** argv)
{
    time_t now = time(0);
    printf("time: %s", ctime(&now));

    struct timeval tv = {0, 0};
    gettimeofday(&tv, 0);
    printf("gettimeofday: %s", ctime(&tv.tv_sec));

#ifndef __APPLE__
    struct timespec ts = {0, 0};
    clock_gettime(CLOCK_REALTIME, &ts);
    printf("clock_gettime: %s", ctime(&ts.tv_sec));
#endif

    return 0;
}
