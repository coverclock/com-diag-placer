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

#endif
