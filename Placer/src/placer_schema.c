/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 */

#include "sqlite3.h"
#include "com/diag/placer/placer.h"
#include "com/diag/placer/placer_schema.h"

/*******************************************************************************
 *
 ******************************************************************************/

void placer_schema_BLOB_import(placer_schema_BLOB_t * dest, char * src)
{
    if (*dest != (placer_schema_BLOB_t)0) {
        free(*dest);
        *dest = (placer_schema_BLOB_t)0;
    }
}

void placer_schema_FLOAT_import(placer_schema_FLOAT_t * dest, char * src)
{
}

void placer_schema_INTEGER_import(placer_schema_INTEGER_t * dest, char * src)
{
}

void placer_schema_INTEGER64_import(placer_schema_INTEGER64_t * dest, char * src)
{
}

void placer_schema_TEXT_import(placer_schema_TEXT_t * dest, char * src)
{
    if (*dest != (placer_schema_TEXT_t)0) {
        free(*dest);
        *dest = (placer_schema_TEXT_t)0;
    }
}

void placer_schema_TEXT16_import(placer_schema_TEXT16_t * dest, char * src)
{
    if (*dest != (placer_schema_TEXT16_t)0) {
        free(*dest);
        *dest = (placer_schema_TEXT16_t)0;
    }
}
