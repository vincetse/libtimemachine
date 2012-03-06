What is `libtimemachine`?
-----------------------

`libtimemachine` is a library used to shift time as reported to a program running on Linux by some time-related functions when it is inconvenient or impossible to change the system clock.  The following time-related functions are currently supported.

1. `time(2)`
2. `gettimeofday(2)`
3. `clock_gettime(2)`


Configuration
-------------

`libtimemachine` reads its configuration file from its default location, `/tmp/libtimemachine.conf`, every time the supported functions are called.  The location of the configuration file can be overwritten by setting the environment variable, `LIBTIMEMACHINE_CONF`, to the location of the desired configuration file.

The configuration file contains a signed integer value that is interpreted as the number of seconds to shift the current time.  For instance, putting a value of -86400 in the configuration file will have the effect of shifting time back by 86400 seconds within the program.  The following is a sample configuration.

    vt100x@dev01:~/src
    $ cat /tmp/libtimemachine.conf
    -86400

Usage
-----

This library can either be linked into an executable or dynamically-loaded with the `LD_PRELOAD` environment variable.  The following is an example of using the latter method.

    $ cat /tmp/libtimemachine.conf
    -86400
    
    vt100x@dev01:~/src/libtime
    $ ./show_time; LD_PRELOAD=$(pwd)/libtimemachine.so ./show_time
    time: Fri Nov  4 02:00:22 2011
    gettimeofday: Fri Nov  4 02:00:22 2011
    clock_gettime: Fri Nov  4 02:00:22 2011
    time: Thu Nov  3 02:00:22 2011
    gettimeofday: Thu Nov  3 02:00:22 2011
    clock_gettime: Thu Nov  3 02:00:22 2011

Note the date/time show_time test program with the `LD_PRELOAD` environment variables set reports a time 86400 seconds before what the same `show_time` program reports without the environments variables set?


FAQ
---

1. Why does it not work for my program?

This library overrides only the aforementioned time-related functions, so your program using `libtimemachine` need to be using these functions to use this library.  More functions will be supported as they are requested.

2. How do I request more functions to be supported?

Submit a patch, or send me a request!

3. How does it work?

`libtimemachine` simply overrides the supported functions from their native libraries with its own versions of the functions so that it has an opportunity to do some arithmetic on the values returned by the real functions.  The real functions are called by address after their addresses are determined by looking up the symbol table.
