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

int placer_schema_BLOB_import(placer_BLOB_t * dest, const char * src, size_t items)
{
    int rc = SQLITE_OK;

    (void)memcpy(dest, (placer_BLOB_t *)src, items);

    return rc;
}

int placer_schema_FLOAT_import(placer_FLOAT_t * dest, const char * src)
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

int placer_schema_INTEGER_import(placer_INTEGER_t * dest, const char * src)
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

int placer_schema_INTEGER64_import(placer_INTEGER64_t * dest, const char * src)
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

int placer_schema_TEXT_import(placer_TEXT_t * dest, const char * src, size_t items)
{
    int rc = SQLITE_ERROR;

    if (items < 1) {
        rc = SQLITE_OK;
    } else {
        dest[items - 1] = '\0';
        (void)strncpy(dest, (placer_TEXT_t *)src, items);
        if (dest[items - 1] == '\0') {
            rc = SQLITE_OK;
        } else {
            dest[items - 1] = '\0';
            placer_error(rc);
        }
    }

    return rc;
}

int placer_schema_TEXT16_import(placer_TEXT16_t * dest, const char * src, size_t items)
{
    int rc = SQLITE_ERROR;
    placer_TEXT16_t * dp = (placer_TEXT16_t *)0;
    const char * sp = (const char *)0;
    size_t ii = 0;

    dp = dest;
    sp = src;
    ii = items;

    while ((ii--) > 0) {
        *dp = *sp; /* Conversion. */
        if (*sp == '\0') {
            break;
        }
        dp += 1;
        sp += 1;
    }

    if (items < 1) {
        rc = SQLITE_OK;
    } else if (*dp == 0) {
        rc = SQLITE_OK;
    } else {
        dest[items - 1] = 0;
        placer_error(rc);
    }

    return rc;
}
