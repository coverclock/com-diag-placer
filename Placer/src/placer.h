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

extern FILE * placer_Debug;

/*******************************************************************************
 * EXEC
 ******************************************************************************/

extern int placer_exec_BLOB_import(placer_BLOB_t * dest, const char * src, size_t items);

extern int placer_exec_FLOAT_import(placer_FLOAT_t * dest, const char * src);

extern int placer_exec_INTEGER_import(placer_INTEGER_t * dest, const char * src);

extern int placer_exec_INTEGER64_import(placer_INTEGER64_t * dest, const char * src);

extern int placer_exec_TEXT_import(placer_TEXT_t * dest, const char * src, size_t items);

extern int placer_exec_TEXT16_import(placer_TEXT16_t * dest, const char * src, size_t items);

#endif
