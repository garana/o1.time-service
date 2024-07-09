/**
 * BSD 3-Clause License
 *
 * Copyright (c) 2021, Gonzalo Arana
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli-common.h"

const char* help =
    "Usage: o1-time-source -h|period\n"
    "  -h shows this message\n"
    "  period is in micro seconds, how often we retrieve the current time.\n"
    "\n";

void parse_options(
    int argc, const char** argv,
    unsigned long* period
) {
    if (argc != 2) {
        fputs(help, stderr);
        exit(1);
    }

    if (!strcmp(argv[1], "-h")) {
        fputs(help, stdout);
        exit(0);
    }

    if (!*argv[1]) {
        fprintf(stderr, "Empty period.\n");
        fputs(help, stderr);
        exit(1);
    }

    char* end;
    *period = strtoul(argv[1], &end, 10);

    if (*end) {
        fprintf(stderr, "Invalid period: %s", argv[1]);
        fputs(help, stderr);
        exit(1);
    }

}
