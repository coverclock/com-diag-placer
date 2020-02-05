/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include "sqlite3.h"
#include "com/diag/diminuto/diminuto_types.h"
#include "com/diag/placer/placer.h"

static FILE * Debug = (FILE *)0;

FILE * placer_debug(FILE * now)
{
    FILE * was;

    was = Debug;
    Debug = now;

    return was;
}

int placer_callback_generic(void * vp, int ncols, char ** value, char ** keyword)
{
    placer_callback_generic_t * pp = (placer_callback_generic_t *)0;
    int ii = 0;

    if (vp != (void *)0) {

        pp = (placer_callback_generic_t *)vp;

        ++(pp->count);

        if (pp->fp == (FILE *)0) {
            /* Do nothing. */
        } else if (ncols <= 0) {
            /* Do nothing. */
        } else {
            for (ii = 0; ii < ncols; ++ii) {
                fprintf(pp->fp, "%s[%d]=\"%s\" ", keyword[ii], ii, value[ii]);
            }
            fputc('\n', pp->fp);
        }

    }

    return SQLITE_OK;
}

size_t placer_expand(char * to, const char * from, size_t tsize, size_t fsize) {
    size_t size = 0;

    while ((fsize > 0) && (tsize > 1) && (*from != '\0')) {
        *(to++) = *from;
        if (*from != '\'') {
            /* Do nothing. */
        } else if (tsize > 2) {
            *(to++) = '\'';
            --tsize;
            ++size;
        } else {
            break;
        }
        ++from;
        --fsize;
        --tsize;
        ++size;
    }
    if (tsize > 0) {
        *to = '\0';
        ++size;
    }

    return size;
}

char * placer_expand_alloc(const char * from)
{
    char * to = (char *)0;
    size_t fsize = 0;
    size_t tsize = 0;

    /*
     * I could scan the from string and determine exactly how many
     * single quotes there are. But I decided to optimize CPU usage
     * instead of memory usage. The memory can be freed quite quickly
     * if the application is building a SQL string using snprintf(3).
     */

    fsize = strlen(from);
    tsize = (fsize * 2) + 1;
    to = malloc(tsize);
    if (to != (char *)0) {
        placer_expand(to, from, tsize, fsize);
    }

    return to;
}

char * placer_format_alloc(size_t size, const char * format, ...)
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

            if (Debug != (FILE *)0) {
                fprintf(Debug, "%s@%d: size=%zu ss=%zu f0=%zu f1=%zu f2=%zu\n", __FILE__, __LINE__, size, ss, f0, f1, f2);
            }

            buffer = malloc(ss);
            if (buffer == (char *)0) {
                perror("malloc");
                break;
            }

            va_start(ap, format);
            length = vsnprintf(buffer, ss, format, ap);
            va_end(ap);

            if (length < ss) {
                break; 
            }

            free(buffer);
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
