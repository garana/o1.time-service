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
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "o1-time-reader.h"
#include "../../o1.shm/src/o1shm.h"

static
struct o1_time_record*
get_record() {
    int fd = -1;
    static struct o1_time_record* time_record = NULL;
    char* error_message = NULL;

    if (!time_record)
        time_record = shm_alloc(&fd, O1_TIME_SHM_NAME, sizeof(struct o1_time_record), O_RDONLY, &error_message);

    if (
        ((void*)time_record == SHM_ALLOC_OPEN_ERROR) &&
        (errno == ENOENT)
    )
        return NULL;

    if (error_message) {
        fprintf(stderr, "fatal: %s", error_message);
        exit(1);
    }

    return time_record;
}

o1_time_val o1_time_get_resolution() {
    struct o1_time_record* time_record = get_record();
    return time_record ?
        time_record->resolution :
        0;
}

o1_time_val o1_time_get_period() {
    struct o1_time_record* time_record = get_record();
    return time_record ?
           time_record->period :
           0;
}

o1_time_val o1_read_time() {
    struct o1_time_record* time_record = get_record();
    return time_record ?
           time_record->last_sample :
           0;
}
