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

#define COM_DIAG_PLACER_SCHEMA_BEGIN(_STRUCTURE_) \
    struct _STRUCTURE_ {

#define COM_DIAG_PLACER_SCHEMA_BLOB(_NAME_, _ITEMS_, _CONSTRAINTS_) \
	placer_schema_BLOB_t _NAME_ [ _ITEMS_ ];

#define COM_DIAG_PLACER_SCHEMA_FLOAT(_NAME_, _CONSTRAINTS_) \
	placer_schema_FLOAT_t _NAME_;

#define COM_DIAG_PLACER_SCHEMA_INTEGER(_NAME_, _CONSTRAINTS_) \
	placer_schema_INTEGER_t _NAME_;

#define COM_DIAG_PLACER_SCHEMA_INTEGER64(_NAME_, _CONSTRAINTS_) \
	placer_schema_INTEGER64_t _NAME_;

#define COM_DIAG_PLACER_SCHEMA_TEXT(_NAME_, _ITEMS_, _CONSTRAINTS_) \
	placer_schema_TEXT_t _NAME_ [ _ITEMS_ ];

#define COM_DIAG_PLACER_SCHEMA_TEXT16(_NAME_, _ITEMS_, _CONSTRAINTS_) \
	placer_schema_TEXT16_t _NAME_ [ _ITEMS_ ];

#define PLACER_FIELD(_CONSTRAINTS_)

#define PLACER_FINAL(_CONSTRAINTS_)

#define COM_DIAG_PLACER_SCHEMA_END(_CONSTRAINTS_) \
    };
