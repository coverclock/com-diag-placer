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
int placer_##_STRUCTURE_##_callback(void * vp, int ncols, char ** value, char ** keyword) { \
    int rc = SQLITE_ERROR; \
    struct _STRUCTURE_ ** pp = (struct _STRUCTURE_ **)0; \
    int ii = 0; \
    pp = (struct _STRUCTURE_ **)vp; \
    do { \
        if (*pp == (struct _STRUCTURE_ *)0) { break; }

#define PLACER_BLOB(_NAME_, _ITEMS_) \
        if (ii >= ncols) { break; } \
        if (strcmp(keyword[ii], #_NAME_) != 0) { break; } \
        if (placer_BLOB_import((*pp)->_NAME_, _ITEMS_, value[ii++]) != SQLITE_OK) { break; }

#define PLACER_FLOAT(_NAME_) \
        if (ii >= ncols) { break; } \
        if (placer_FLOAT_import(&((*pp)->_NAME_), value[ii++]) != SQLITE_OK) { break; }

#define PLACER_INTEGER(_NAME_) \
        if (ii >= ncols) { break; } \
        if (strcmp(keyword[ii], #_NAME_) != 0) { break; } \
        if (placer_INTEGER_import(&((*pp)->_NAME_), value[ii++]) != SQLITE_OK) { break; }

#define PLACER_INTEGER64(_NAME_) \
        if (ii >= ncols) { break; } \
        if (strcmp(keyword[ii], #_NAME_) != 0) { break; } \
        if (placer_INTEGER64_import(&((*pp)->_NAME_), value[ii++]) != SQLITE_OK) { break; }

#define PLACER_TEXT(_NAME_, _ITEMS_) \
        if (ii >= ncols) { break; } \
        if (strcmp(keyword[ii], #_NAME_) != 0) { break; } \
        if (placer_TEXT_import((*pp)->_NAME_, _ITEMS_, value[ii++]) != SQLITE_OK) { break; }

#define PLACER_TEXT16(_NAME_, _ITEMS_) \
        if (ii >= ncols) { break; } \
        if (strcmp(keyword[ii], #_NAME_) != 0) { break; } \
        if (placer_TEXT16_import((*pp)->_NAME_, _ITEMS_, value[ii++]) != SQLITE_OK) { break; }

#define PLACER_FIELD(_CONSTRAINTS_)

#define PLACER_FINAL(_CONSTRAINTS_)

#define PLACER_END(_CONSTRAINTS_) \
        (*pp) += 1; \
        rc = SQLITE_OK; \
    } while (0); \
    return rc; \
}
