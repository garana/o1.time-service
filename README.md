# o1.time-service

There are essentially to retrieve time/date:
1. using syscalls (gettimeofday(2)),
2. using library calls (clock_gettime(3)), which rely on syscalls on their own
3. using assembly instructions (`rdtsc` on x86/x86_64, `cntvct` on arm64).

#1 and #2 above require a syscall, which add some overhead (negligible, most likely).
#3 the exact implementation depend on different CPU models and vendors.

This library provides:
a. A time feeder ("producer"), which calls `clock_gettime(CLOCK_MONOTONIC)` periodically
   (frequency is configurable) and stores its output in shared memory (it also stores
   `clock_getres(CLOCK_MONOTONIC)` in the same shared memory area).  This is expected
   to run either in a separate process or thread.
2. A time reader ("consumer") which reads from this shared memory area.

This allows the time to be read by only reading from memory.

## Notes

`clock_getres(2)` and `clock_gettime(2)` they both use a `struct timespec` which
has 2 fields:
```
struct timespec {
    time_t   tv_sec;        /* seconds */
    long     tv_nsec;       /* nanoseconds */
};
```

Reading and writing multi field value can cause unexpected results.

To prevent this, each values (time, and timer resolution) is stored as an unsigned 64 bit 
integer, representing the sample in microseconds (would wrap 584942 years after 1970).
