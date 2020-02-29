/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_PLACER_PRIVATE_
#define _H_COM_DIAG_PLACER_PRIVATE_

/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 * This defines the private API for Placer.
 */

#include <stdio.h>
#include "com/diag/placer/placer.h"

/*******************************************************************************
 * CORE
 ******************************************************************************/

/**
 * This declares the global debug flag.
 */
extern FILE * placer_Debug;

/**
 * This is the global separator character.
 */
extern char placer_Separator;

/*******************************************************************************
 * EXEC
 ******************************************************************************/

/**
 * Import a BLOB object from a characcter pointer provided by SQLite3 to the exec
 * callback. This is called by the schema exec macros.
 * @param dest points to the BLOB field in the schema strructure.
 * @param src points to the data provided by SQLite3.
 * @param items it the number of bytes in the BLOB field.
 */
extern int placer_exec_BLOB_import(placer_BLOB_t * dest, const char * src, size_t items);

/**
 * Import a FLOAT from a characcter pointer provided by SQLite3 to the exec
 * callback. This is called by the schema exec macros.
 * @param dest points to the FLOAT field in the schema strructure.
 * @param src points to the data provided by SQLite3.
 */
extern int placer_exec_FLOAT_import(placer_FLOAT_t * dest, const char * src);

/**
 * Import a INTEGER from a characcter pointer provided by SQLite3 to the exec
 * callback. This is called by the schema exec macros.
 * @param dest points to the INTEGER field in the schema strructure.
 * @param src points to the data provided by SQLite3.
 */
extern int placer_exec_INTEGER_import(placer_INTEGER_t * dest, const char * src);

/**
 * Import a INTERGER64 from a characcter pointer provided by SQLite3 to the exec
 * callback. This is called by the schema exec macros.
 * @param dest points to the INTEGER64 field in the schema strructure.
 * @param src points to the data provided by SQLite3.
 */
extern int placer_exec_INTEGER64_import(placer_INTEGER64_t * dest, const char * src);

/**
 * Import a TEXT string from a characcter pointer provided by SQLite3 to the exec
 * callback. This is called by the schema exec macros. The destination buffer
 * is NOT guaranteed to be NUL terminated if the source data completely fills
 * it.
 * @param dest points to the TEXT field in the schema strructure.
 * @param src points to the data provided by SQLite3.
 * @param items it the number of UTF-8 charactgers in the TEXT field.
 */
extern int placer_exec_TEXT_import(placer_TEXT_t * dest, const char * src, size_t items);

/**
 * Import a TEXT16 string from a characcter pointer provided by SQLite3 to the
 * exec callback. This is called by the schema exec macros. The destination
 * buffer is NOT guaranteed to be NUL terminated if the source data completely
 * fills it.
 * @param dest points to the TEXT16 field in the schema strructure.
 * @param src points to the data provided by SQLite3.
 * @param items it the number of UTF-16 characters (NOT BYTES) in the TEXT16 field.
 */
extern int placer_exec_TEXT16_import(placer_TEXT16_t * dest, const char * src, size_t items);

#endif
