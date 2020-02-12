/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "sqlite3.h"
#include "com/diag/placer/placer.h"
#include "com/diag/placer/placer_schema.h"

/*******************************************************************************
 *
 ******************************************************************************/

int placer_schema_BLOB_import(placer_schema_BLOB_t * dest, size_t items, const char * src)
{
    int rc = SQLITE_OK;

    (void)memcpy(dest, (placer_schema_BLOB_t *)src, items);

    return rc;
}

int placer_schema_FLOAT_import(placer_schema_FLOAT_t * dest, const char * src)
{
    int rc = SQLITE_ERROR;
    char * end = (char *)0;

    *dest = strtod(src, &end);
    if ((end != (char *)0) && (*end == '\0')) {
        rc = SQLITE_OK;
    }

    return rc;
}

int placer_schema_INTEGER_import(placer_schema_INTEGER_t * dest, const char * src)
{
    int rc = SQLITE_ERROR;
    char * end = (char *)0;

    *dest = strtol(src, &end, 10);
    if ((end != (char *)0) && (*end == '\0')) {
        rc = SQLITE_OK;
    }

    return rc;
}

int placer_schema_INTEGER64_import(placer_schema_INTEGER64_t * dest, const char * src)
{
    int rc = SQLITE_ERROR;
    char * end = (char *)0;

    *dest = strtoll(src, &end, 10);
    if ((end != (char *)0) && (*end == '\0')) {
        rc = SQLITE_OK;
    }

    return rc;
}

int placer_schema_TEXT_import(placer_schema_TEXT_t * dest, size_t items, const char * src)
{
    int rc = SQLITE_ERROR;

    (void)strncpy(dest, (placer_schema_TEXT_t *)src, items);
    if (dest[items - 1] == '\0') {
        rc = SQLITE_OK;
    } else {
        dest[items - 1] = '\0';
    }

    return rc;
}

int placer_schema_TEXT16_import(placer_schema_TEXT16_t * dest, size_t items, const char * src)
{
    int rc = SQLITE_ERROR;

    (void)wcsncpy(dest, (placer_schema_TEXT16_t *)src, items);
    if (dest[items - 1] == L'\0') {
        rc = SQLITE_OK;
    } else {
        dest[items - 1] = L'\0';
    }

    return rc;
}
