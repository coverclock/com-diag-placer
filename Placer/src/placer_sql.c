/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in LICENSE.txt<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "sqlite3.h"
#include "com/diag/diminuto/diminuto_types.h"
#include "com/diag/diminuto/diminuto_fibonacci.h"
#include "com/diag/placer/placer.h"
#include "placer.h" /* Private API. */

size_t placer_sql_vformat(char * buffer, size_t size, const char * format, va_list ap)
{
    size_t ss = 0;

    if (buffer == (char *)0) {
        /*  Do nothing. */
    } else if (size == 0) {
        /* Do nothing. */
    } else {
        ss = vsnprintf(buffer, size, format, ap);
    }

    return ss;
}

size_t placer_sql_format(char * buffer, size_t size, const char * format, ...)
{
    va_list ap;
    size_t ss = 0;

    va_start(ap, format);
    ss = placer_sql_vformat(buffer, size, format, ap);
    va_end(ap);

    return ss;
}

char * placer_sql_vformata(size_t size, const char * format, va_list op)
{
    char * buffer = (char *)0;
    int length = 0;
    va_list ap;
    diminuto_fibonacci_state_t state = { 0, };
    diminuto_fibonacci_value_t ff = 0;
    size_t ss = 0;

    do {

        if (size == 0) {
            errno = EINVAL;
            perror("size");
            break;
        }

        ss = size;

        diminuto_fibonacci_init_classic(&state);
        (void)diminuto_fibonacci_next(&state);
        ff = diminuto_fibonacci_next(&state);

        while (!0) {

            if (placer_Debug != (FILE *)0) {
                fprintf(placer_Debug, "%s@%d: size=%zu ss=%zu ff=%u\n", __FILE__, __LINE__, size, ss, ff);
            }

            buffer = (char *)sqlite3_malloc(ss);
            if (buffer == (char *)0) {
                perror("sqlite3_malloc");
                break;
            }

            va_copy(ap, op);
            length = placer_sql_vformat(buffer, ss, format, ap);

            if (length < ss) {
                break; 
            }

            sqlite3_free(buffer);
            buffer = (char *)0;

            /*
             * Scale the buffer size by the Fibonacci sequence. No, really.
             */

            ff = diminuto_fibonacci_next(&state);

            if (size > ((~(size_t)0) / ff)) {
                errno = E2BIG;
                perror("size");
                break;
            }

            ss = size * ff;

        }

    } while (0);

    return buffer;
}

char * placer_sql_formata(size_t size, const char * format, ...)
{
    char * buffer = (char *)0;
    va_list ap;

    va_start(ap, format);
    buffer = placer_sql_vformata(size, format, ap);
    va_end(ap);

    return buffer;
}
