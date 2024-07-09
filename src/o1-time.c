
#include "o1-time.h"

uint64_t o1_ts2o1tv(struct timespec* ts) {
    return ts->tv_sec * 1000000 +
        ts->tv_nsec / 1000;
}
