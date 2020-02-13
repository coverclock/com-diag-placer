/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_PLACER_SCHEMA_
#define _H_COM_DIAG_PLACER_SCHEMA_

/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 */

#include <stdint.h>
#include <wchar.h>
#include "sqlite3.h"

/*******************************************************************************
 *
 ******************************************************************************/

typedef uint8_t placer_BLOB_t;

typedef double placer_FLOAT_t;

typedef int32_t placer_INTEGER_t;

typedef int64_t placer_INTEGER64_t;

typedef unsigned char placer_TEXT_t;

typedef wchar_t placer_TEXT16_t;

/*******************************************************************************
 *
 ******************************************************************************/

extern int placer_BLOB_import(placer_BLOB_t * dest, size_t items, const char * src);

extern int placer_FLOAT_import(placer_FLOAT_t * dest, const char * src);

extern int placer_INTEGER_import(placer_INTEGER_t * dest, const char * src);

extern int placer_INTEGER64_import(placer_INTEGER64_t * dest, const char * src);

extern int placer_TEXT_import(placer_TEXT_t * dest, size_t items, const char * src);

extern int placer_TEXT16_import(placer_TEXT16_t * dest, size_t items, const char * src);

/*******************************************************************************
 *
 ******************************************************************************/

#endif
