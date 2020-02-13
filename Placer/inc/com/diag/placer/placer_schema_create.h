/* vi: set ts=4 expandtab shiftwidth=4: */

/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 * N.B. THIS HEADER FILE CAN BE INCLUDED MORE THAN ONCE.
 */

#include "placer_schema_undef.h"

#define PLACER_SCHEMA(_STRUCTURE_) \
    "CREATE TABLE " #_STRUCTURE_ " ("

#define PLACER_BLOB(_NAME_, _ITEMS_) \
	#_NAME_ " BLOB "

#define PLACER_FLOAT(_NAME_) \
	#_NAME_ " FLOAT "

#define PLACER_INTEGER(_NAME_) \
	#_NAME_ " INTEGER "

#define PLACER_INTEGER64(_NAME_) \
	#_NAME_ " INTEGER "

#define PLACER_TEXT(_NAME_, _ITEMS_) \
	#_NAME_ " TEXT "

#define PLACER_TEXT16(_NAME_, _ITEMS_) \
	#_NAME_ " TEXT "

#define PLACER_FIELD(_CONSTRAINTS_) \
    #_CONSTRAINTS_ ", "

#define PLACER_FINAL(_CONSTRAINTS_) \
    #_CONSTRAINTS_ " "

#define PLACER_END(_CONSTRAINTS_) \
    #_CONSTRAINTS_ ");";
