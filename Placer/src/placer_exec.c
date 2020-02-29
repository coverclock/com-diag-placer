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
#include "placer.h" /* Private API. */

int placer_exec_BLOB_import(placer_BLOB_t * dest, const char * src, size_t items)
{
    int rc = SQLITE_OK;

    (void)memcpy(dest, (placer_BLOB_t *)src, items);

    return rc;
}

int placer_exec_FLOAT_import(placer_FLOAT_t * dest, const char * src)
{
    int rc = SQLITE_ERROR;
    char * end = (char *)0;

    *dest = strtod(src, &end);
    if ((end != (char *)0) && (*end == '\0')) {
        rc = SQLITE_OK;
    } else {
        placer_error(rc);
    }

    return rc;
}

int placer_exec_INTEGER_import(placer_INTEGER_t * dest, const char * src)
{
    int rc = SQLITE_ERROR;
    char * end = (char *)0;

    *dest = strtol(src, &end, 10);
    if ((end != (char *)0) && (*end == '\0')) {
        rc = SQLITE_OK;
    } else {
        placer_error(rc);
    }

    return rc;
}

int placer_exec_INTEGER64_import(placer_INTEGER64_t * dest, const char * src)
{
    int rc = SQLITE_ERROR;
    char * end = (char *)0;

    *dest = strtoll(src, &end, 10);
    if ((end != (char *)0) && (*end == '\0')) {
        rc = SQLITE_OK;
    } else {
        placer_error(rc);
    }

    return rc;
}

int placer_exec_TEXT_import(placer_TEXT_t * dest, const char * src, size_t items)
{
    int rc = SQLITE_OK;

    (void)strncpy(dest, (placer_TEXT_t *)src, items);

    return rc;
}

int placer_exec_TEXT16_import(placer_TEXT16_t * dest, const char * src, size_t items)
{
    int rc = SQLITE_OK;
    placer_TEXT16_t * dp = (placer_TEXT16_t *)0;
    const char * sp = (const char *)0;
    size_t ii = 0;

    for (dp = dest, sp = src, ii = items; ii > 0; dp++, sp++, ii--) {
        *dp = *sp; /* Conversion. */
        if (*sp == '\0') {
            break;
        }
    }

    return rc;
}

int placer_exec_generic_callback(void * vp, int ncols, char ** value, char ** keyword)
{
    placer_generic_callback_t * pp = (placer_generic_callback_t *)0;
    int ii = 0;

    if (vp != (void *)0) {

        pp = (placer_generic_callback_t *)vp;

        ++(pp->count);

        if (pp->fp == (FILE *)0) {
            /* Do nothing. */
        } else if (ncols <= 0) {
            /* Do nothing. */
        } else {
            if (pp->count == 1) {
                for (ii = 0; ii < ncols; ++ii) {
                    if (ii > 0) {
                        fputc(placer_Separator, pp->fp);
                    }
                    fputs(keyword[ii], pp->fp);
                }
                fputc('\n', pp->fp);
            }
            for (ii = 0; ii < ncols; ++ii) {
                if (ii > 0) {
                    fputc(placer_Separator, pp->fp);
                }
                fputs(value[ii], pp->fp);
            }
            fputc('\n', pp->fp);
        }

    }

    return SQLITE_OK;
}

int placer_exec(sqlite3 * db, const char * sql, placer_exec_callback_t * cp, void * vp)
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
