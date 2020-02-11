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

#if defined(BLOB)
#undef BLOB
#endif

#if defined(FLOAT)
#undef FLOAT
#endif

#if defined(INTEGER)
#undef INTEGER
#endif

#if defined(INTEGER64)
#undef INTEGER64
#endif

#if defined(TEXT)
#undef TEXT
#endif

#if defined(TEXT16)
#undef TEXT16
#endif

/*******************************************************************************
 *
 ******************************************************************************/

#define BLOB        "BLOB"

#define FLOAT       "FLOAT"

#define INTEGER     "INTEGER"

#define INTEGER64   "INTEGER"

#define TEXT        "TEXT"

#define TEXT16      "TEXT"

/*******************************************************************************
 *
 ******************************************************************************/

typedef void * placer_schema_BLOB_t;

typedef double placer_schema_FLOAT_t;

typedef int32_t placer_schema_INTEGER_t;

typedef int64_t placer_schema_INTEGER64_t;

typedef char * placer_schema_TEXT_t;

typedef wchar_t * placer_schema_TEXT16_t;

/*******************************************************************************
 *
 ******************************************************************************/

extern void placer_schema_BLOB_import(placer_schema_BLOB_t * dest, char * src);

extern void placer_schema_FLOAT_import(placer_schema_FLOAT_t * dest, char * src);

extern void placer_schema_INTEGER_import(placer_schema_INTEGER_t * dest, char * src);

extern void placer_schema_INTEGER64_import(placer_schema_INTEGER64_t * dest, char * src);

extern void placer_schema_TEXT_import(placer_schema_TEXT_t * dest, char * src);

extern void placer_schema_TEXT16_import(placer_schema_TEXT16_t * dest, char * src);

/*******************************************************************************
 *
 ******************************************************************************/

#endif
