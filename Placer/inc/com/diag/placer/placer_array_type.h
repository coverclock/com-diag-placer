/* vi: set ts=4 expandtab shiftwidth=4: */

/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 * These X macros generate a list of field types.
 * N.B. THIS HEADER FILE CAN BE INCLUDED MORE THAN ONCE.
 * Note that there are no SQLITE_INTEGER64 or SQLITE_TEXT16
 * fundamental types.
 */

#include "placer.h"

#define PLACER_SCHEMA(_STRUCTURE_) \
    static const int PLACER_ARRAY_struct_##_STRUCTURE_##_TYPE[] = {

#define PLACER_BLOB(_NAME_, _ITEMS_) \
	( SQLITE_BLOB )

#define PLACER_FLOAT(_NAME_) \
	( SQLITE_FLOAT )

#define PLACER_INTEGER(_NAME_) \
	( SQLITE_INTEGER )

#define PLACER_INTEGER64(_NAME_) \
	( SQLITE_INTEGER )

#define PLACER_TEXT(_NAME_, _ITEMS_) \
	( SQLITE_TEXT )

#define PLACER_TEXT16(_NAME_, _ITEMS_) \
	( SQLITE_TEXT )

#define PLACER_FIELD(_CONSTRAINTS_) \
        ,

#define PLACER_FINAL(_CONSTRAINTS_)

#define PLACER_END(_CONSTRAINTS_) \
    };

