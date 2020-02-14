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
    static const char SEPERATOR = '|';
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
            if (pp->count == 1) {
                for (ii = 0; ii < ncols; ++ii) {
                    if (ii > 0) {
                        fputc(SEPERATOR, pp->fp);
                    }
                    fputs(keyword[ii], pp->fp);
                }
                fputc('\n', pp->fp);
            }
            for (ii = 0; ii < ncols; ++ii) {
                if (ii > 0) {
                    fputc(SEPERATOR, pp->fp);
                }
                fputs(value[ii], pp->fp);
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

    if (Debug != (FILE *)0) {
        fprintf(Debug, "%s@%d: size=%zu\n", __FILE__, __LINE__, size);
    }

    while ((size > 1) && (*from != '\0')) {
        if (*from != '\'') {
            *(to++) = *(from++);
            size -= 1;
            length += 1;
        } else if (size > 2) {
            *(to++) = *from;
            *(to++) = *(from++);
            size -= 2;
            length += 2;
        } else {
            break;
        }
    }

    if (size > 0) {
        *to = '\0';
        length += 1;
    }

    if (Debug != (FILE *)0) {
        fprintf(Debug, "%s@%d: length=%zu\n", __FILE__, __LINE__, length);
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

    if (Debug != (FILE *)0) {
        fprintf(Debug, "%s@%d: count=%zu\n", __FILE__, __LINE__, count);
    }

    size = strlen(from) + count + 1;
    to = (char *)sqlite3_malloc(size);
    if (to == (char *)0) {
        perror("sqlite3_malloc");
    } else  {
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

/*******************************************************************************
 *
 ******************************************************************************/

int placer_db_exec(sqlite3 * db, const char * sql, placer_callback_t * cp, void * vp)
{
    int rc = SQLITE_ERROR;
    char * sqlmessage = (char *)0;

    rc = sqlite3_exec(db, sql, cp, vp, &sqlmessage);
    if (rc != SQLITE_OK) {
        placer_message(sqlmessage);
        placer_error(rc);
    }

    return rc;
}

sqlite3_stmt * placer_db_prepare(sqlite3 * db, const char * sql)
{
    sqlite3_stmt * sp = (sqlite3_stmt *)0;
    int rc = SQLITE_ERROR;

    rc = sqlite3_prepare_v2(db, sql, -1, &sp, (const char **)0);
    if (rc != SQLITE_OK) {
        placer_error(rc);
        if (sp != (sqlite3_stmt *)0) {
            rc = sqlite3_finalize(sp);
            if (rc != SQLITE_OK) {
                placer_error(rc);
            }
            sp = (sqlite3_stmt *)0;
        }
    }

    return sp;
}

int placer_db_steps(sqlite3_stmt * sp, placer_step_t * cp, void * vp)
{
    int rc = SQLITE_ERROR;
    int tc = SQLITE_ERROR;
    int ii = 0;

    while (!0) {

        if (Debug != (FILE *)0) {
            fprintf(Debug, "%s@%d: step=%d\n", __FILE__, __LINE__, ii);
        }

        rc = sqlite3_step(sp);

        if (Debug != (FILE *)0) {
            fprintf(Debug, "%s@%d: step=%d rc=%d=\"%s\"\n", __FILE__, __LINE__, ii, rc, sqlite3_errstr(rc));
        }

        if (rc == SQLITE_OK) {
            rc = SQLITE_ERROR;
            placer_error(rc);
            break;
        }

        if (rc == SQLITE_DONE) {
            rc = SQLITE_OK;
            break;
        }

        if (rc != SQLITE_ROW) {
            placer_error(rc);
            break;
        }

        if (cp != (placer_step_t *)0) {
            rc = (*cp)(sp, vp);
            if (rc != SQLITE_OK) {
                placer_error(rc);
                break;
            }
        }

    } 

    tc = sqlite3_finalize(sp);
    if (tc != SQLITE_OK) {
        placer_error(tc);
        rc = tc;
    }

    return rc;
}

/*******************************************************************************
 *
 ******************************************************************************/

int placer_BLOB_import(placer_BLOB_t * dest, size_t items, const char * src)
{
    int rc = SQLITE_OK;

    (void)memcpy(dest, (placer_BLOB_t *)src, items);

    return rc;
}

int placer_FLOAT_import(placer_FLOAT_t * dest, const char * src)
{
    int rc = SQLITE_ERROR;
    char * end = (char *)0;

    *dest = strtod(src, &end);
    if ((end != (char *)0) && (*end == '\0')) {
        rc = SQLITE_OK;
    }

    return rc;
}

int placer_INTEGER_import(placer_INTEGER_t * dest, const char * src)
{
    int rc = SQLITE_ERROR;
    char * end = (char *)0;

    *dest = strtol(src, &end, 10);
    if ((end != (char *)0) && (*end == '\0')) {
        rc = SQLITE_OK;
    }

    return rc;
}

int placer_INTEGER64_import(placer_INTEGER64_t * dest, const char * src)
{
    int rc = SQLITE_ERROR;
    char * end = (char *)0;

    *dest = strtoll(src, &end, 10);
    if ((end != (char *)0) && (*end == '\0')) {
        rc = SQLITE_OK;
    }

    return rc;
}

int placer_TEXT_import(placer_TEXT_t * dest, size_t items, const char * src)
{
    int rc = SQLITE_ERROR;

    (void)strncpy(dest, (placer_TEXT_t *)src, items);
    if (dest[items - 1] == '\0') {
        rc = SQLITE_OK;
    } else {
        dest[items - 1] = '\0';
    }

    return rc;
}

int placer_TEXT16_import(placer_TEXT16_t * dest, size_t items, const char * src)
{
    int rc = SQLITE_ERROR;

    (void)wcsncpy(dest, (placer_TEXT16_t *)src, items);
    if (dest[items - 1] == L'\0') {
        rc = SQLITE_OK;
    } else {
        dest[items - 1] = L'\0';
    }

    return rc;
}

/*******************************************************************************
 *
 ******************************************************************************/
