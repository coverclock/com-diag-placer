/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include "sqlite3.h"
#include "com/diag/diminuto/diminuto_types.h"
#include "placer.h"
#include "com/diag/placer/placer.h"

char * placer_sql_vformata(size_t size, const char * format, va_list op)
{
    char * buffer = (char *)0;
    int length = 0;
    va_list ap;
    size_t f0 = 1;
    size_t f1 = 1;
    size_t f2 = 0;
    size_t ss = 0;

    do {

        if (size == 0) {
            errno = EINVAL;
            perror("size");
            break;
        }

        ss = size;

        while (!0) {

            if (placer_Debug != (FILE *)0) {
                fprintf(placer_Debug, "%s@%d: size=%zu ss=%zu f0=%zu f1=%zu f2=%zu\n", __FILE__, __LINE__, size, ss, f0, f1, f2);
            }

            buffer = (char *)sqlite3_malloc(ss);
            if (buffer == (char *)0) {
                perror("sqlite3_malloc");
                break;
            }

            va_copy(ap, op);
            length = vsnprintf(buffer, ss, format, ap);

            if (length < ss) {
                break; 
            }

            sqlite3_free(buffer);
            buffer = (char *)0;

            /*
             * Scale the buffer size by the Fibonacci sequence. No, really.
             */

            f2 = f0;
            f0 = f1;

            if (f1 > ((~(size_t)0) - f2)) {
                errno = E2BIG;
                perror("size");
                break;
            }

            f1 += f2;

            if (size > ((~(size_t)0) / f1)) {
                errno = E2BIG;
                perror("size");
                break;
            }

            ss = size * f1;

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
