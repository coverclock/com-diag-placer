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

#define COM_DIAG_PLACER_SCHEMA_FIELD(_NAME_, _TYPE_, _CONSTRAINTS_) \
	placer_schema_##_TYPE_##_t _NAME_;

#define COM_DIAG_PLACER_SCHEMA_FIELD_LAST(_NAME_, _TYPE_, _CONSTRAINTS_) \
	placer_schema_##_TYPE_##_t _NAME_;

#define COM_DIAG_PLACER_SCHEMA_END(_CONSTRAINTS_) \
    };
