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

#define PLACER_BEGIN(_STRUCTURE_) \
    {

#define PLACER_BLOB(_NAME_, _ITEMS_) \
    { (placer_schema_BLOB_t)0, }

#define PLACER_FLOAT(_NAME_) \
    (placer_schema_FLOAT_t)0

#define PLACER_INTEGER(_NAME_) \
    (placer_schema_INTEGER_t)0

#define PLACER_INTEGER64(_NAME_) \
    (placer_schema_INTEGER64_t)0

#define PLACER_TEXT(_NAME_, _ITEMS_) \
    { (placer_schema_TEXT_t)0, }

#define PLACER_TEXT16(_NAME_, _ITEMS_) \
    { (placer_schema_TEXT16_t)0, }

#define PLACER_FIELD(_CONSTRAINTS_) \
    ,

#define PLACER_FINAL(_CONSTRAINTS_)

#define PLACER_END(_CONSTRAINTS_) \
    };
