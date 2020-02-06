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

/*******************************************************************************
 *
 ******************************************************************************/

static FILE * Debug = (FILE *)0;

FILE * placer_debug(FILE * now)
{
    FILE * was;

    was = Debug;
    Debug = now;

    return was;
}

/*******************************************************************************
 *
 ******************************************************************************/

void placer_message(char * message)
{
    if (message != (char *)0) {
        fputs("SQLite: \"", stderr);
        fputs(message, stderr);
        fputs("\"\n", stderr);
        sqlite3_free(message);
    }
}

void placer_error(int error)
{
    if (error != SQLITE_OK) {
        fprintf(stderr, "SQLite: [%d] \"", error);
        fputs(sqlite3_errstr(error), stderr);
        fputs("\"\n", stderr);
    }
}

/*******************************************************************************
 *
 ******************************************************************************/

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
                fprintf(pp->fp, "%s=\"%s\" ", keyword[ii], value[ii]);
            }
            fputc('\n', pp->fp);
        }

    }

    return SQLITE_OK;
}

/*******************************************************************************
 *
 ******************************************************************************/

size_t placer_str_expand(char * to, const char * from, size_t size) {
    size_t length = 0;

    while ((size > 1) && (*from != '\0')) {
        if (*from != '\'') {
            *(to++) = *(from++);
        } else if (size > 2) {
            *(to++) = *from;
            *(to++) = *(from++);
            --size;
            ++length;
        } else {
            break;
        }
        --size;
        ++length;
    }

    if (size > 0) {
        *to = '\0';
        ++length;
    }

    return length;
}

char * placer_str_expanda(const char * from)
{
    char * to = (char *)0;
    const char * ff = (const char *)0;
    size_t count = 0;
    size_t size = 0;

    for (ff = from; *ff != '\0'; ++ff) {
        if (*ff == '\'') {
            ++count;
        }
    }

    size = strlen(from) + count + 1;
    to = malloc(size);
    if (to != (char *)0) {
        placer_str_expand(to, from, size);
    }

    return to;
}

/*******************************************************************************
 *
 ******************************************************************************/

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

            if (Debug != (FILE *)0) {
                fprintf(Debug, "%s@%d: size=%zu ss=%zu f0=%zu f1=%zu f2=%zu\n", __FILE__, __LINE__, size, ss, f0, f1, f2);
            }

            buffer = malloc(ss);
            if (buffer == (char *)0) {
                perror("malloc");
                break;
            }

            va_copy(ap, op);
            length = vsnprintf(buffer, ss, format, ap);

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

char * placer_sql_formata(size_t size, const char * format, ...)
{
    char * buffer = (char *)0;
    va_list ap;

    va_start(ap, format);
    buffer = placer_sql_vformata(size, format, ap);
    va_end(ap);

    return buffer;
}

/*******************************************************************************
 *
 ******************************************************************************/

int placer_db_exec(sqlite3 * db, const char * sql, int (*cp)(void *, int, char **, char **), void * vp)
{
    int rc = 0;
    char * sqlmessage = (char *)0;

    rc = sqlite3_exec(db, sql, cp, vp, &sqlmessage);
    if (rc != SQLITE_OK) {
        placer_message(sqlmessage);
        placer_error(rc);
    }

    return rc;
}
