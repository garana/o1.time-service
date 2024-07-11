/**
 * BSD 3-Clause License
 *
 * Copyright (c) 2024, Gonzalo Arana
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#include <fcntl.h>
#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include "o1-time-feeder.h"
#include "o1shm.h"

/**
 * if status != 0, is treated as an error, the message is printed in stderr,
 * and exit(1) is called in that case.
 * @param status value to evaluate as success (0) or failure.
 * @param format printf(3) format string.
 * @param ...
 */
static void o1_must_succeed(int status, const char* format, ...) {
    if (status == 0) return;

    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    exit(1);
}

_Noreturn void o1_time_periodic_update(unsigned long period) {

    int fd = -1;
    char* message;

    struct o1_time_record* time_record = shm_alloc(
        &fd,
        O1_TIME_SHM_NAME,
        sizeof(struct o1_time_record),
        O_CREAT | O_RDWR,
        &message
    );

    if (message) {
        fprintf(stderr, "%s\n", message);
        exit(1);
    }

    struct timespec tmp;
    o1_must_succeed(clock_getres(CLOCK_MONOTONIC_RAW, &tmp), "clock_getres failed: %s", strerror(errno));
    time_record->resolution = o1_ts2o1tv(&tmp);

    time_record->period = period;

    o1_time_val expected = 0;
    o1_time_val first = 0;
    o1_time_val loop_count = 0;

    while (true) {
        o1_must_succeed(clock_gettime(CLOCK_MONOTONIC_RAW, &tmp), "clock_getres failed: %s", strerror(errno));
        o1_time_val current = o1_ts2o1tv(&tmp);
        time_record->last_sample = current;

        if (!loop_count++)
            first = current;

        o1_time_val elapsed = current - first;
        o1_time_val drift = elapsed - expected;
        o1_time_val sleep_time = drift > period ? 0 : period - drift;

#if defined(O1_TIME_DEBUG)
        fprintf(stdout, "first=%lu ", first);
        fprintf(stdout, "current=%lu ", current);
        fprintf(stdout, "elapsed=%lu ", elapsed);
        fprintf(stdout, "expected=%lu ", expected);
        fprintf(stdout, "drift=%lu ", drift);
        fprintf(stdout, "sleep_time=%lu\n", sleep_time);
#endif

        if (sleep_time)
            usleep(sleep_time);

        expected += period;
    }

}
