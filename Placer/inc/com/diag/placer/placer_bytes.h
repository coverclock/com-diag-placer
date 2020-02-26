/* vi: set ts=4 expandtab shiftwidth=4: */

/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 * These X macros generate a list of field lengths in bytes.
 * N.B. THIS HEADER FILE CAN BE INCLUDED MORE THAN ONCE.
 */

#include "placer.h"

#define PLACER_SCHEMA(_STRUCTURE_) \
    static const size_t PLACER_ARRAY_struct_##_STRUCTURE_##_BYTES[] = {

#define PLACER_BLOB(_NAME_, _ITEMS_) \
        (sizeof(placer_BLOB_t) * (_ITEMS_))

#define PLACER_FLOAT(_NAME_) \
        sizeof(placer_FLOAT_t)

#define PLACER_INTEGER(_NAME_) \
        sizeof(placer_INTEGER_t)

#define PLACER_INTEGER64(_NAME_) \
        sizeof(placer_INTEGER64_t)

#define PLACER_TEXT(_NAME_, _ITEMS_) \
        (sizeof(placer_TEXT_t) * (_ITEMS_))

#define PLACER_TEXT16(_NAME_, _ITEMS_) \
        (sizeof(placer_TEXT16_t) * (_ITEMS_))

#define PLACER_FIELD(_CONSTRAINTS_) \
        ,

#define PLACER_FINAL(_CONSTRAINTS_)

#define PLACER_END(_CONSTRAINTS_) \
    };
