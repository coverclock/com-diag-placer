/* vi: set ts=4 expandtab shiftwidth=4: */

/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 * These X macros generate a step function for a schema.
 * N.B. THIS HEADER FILE CAN BE INCLUDED MORE THAN ONCE.
 */

#include <string.h>
#include <wchar.h>
#include <sqlite3.h>
#include "placer.h"

#define PLACER_SCHEMA(_STRUCTURE_) \
int placer_struct_##_STRUCTURE_##_step(sqlite3_stmt * sp, void * vp) { \
    int rc = SQLITE_ERROR; \
    struct _STRUCTURE_ *** ip = (struct _STRUCTURE_ ***)0; \
    struct _STRUCTURE_ ** ap = (struct _STRUCTURE_ **)0; \
    struct _STRUCTURE_ * pp = (struct _STRUCTURE_ *)0; \
    int nn = 0; \
    int tt = 0; \
    int ii = 0; \
    const char * name = (const char *)0; \
    ip = (struct _STRUCTURE_ ***)vp; \
    ap = *ip; \
    pp = *ap; \
    do { \
        if (pp == (struct _STRUCTURE_ *)0) { break; } \
        nn = sqlite3_column_count(sp); \

#define PLACER_BLOB(_NAME_, _ITEMS_) \
        { \
            if (ii >= nn) { break; } \
            tt = sqlite3_column_type(sp, ii); \
            if (tt != SQLITE_BLOB) { break; } \
            name = sqlite3_column_name(sp, ii); \
            if (strcmp(name, #_NAME_) != 0) { break; } \
            memcpy(pp->_NAME_, sqlite3_column_blob(sp, ii), _ITEMS_); \
            ii += 1; \
        }

#define PLACER_FLOAT(_NAME_) \
        { \
            if (ii >= nn) { break; } \
            tt = sqlite3_column_type(sp, ii); \
            if (tt != SQLITE_FLOAT) { break; } \
            name = sqlite3_column_name(sp, ii); \
            if (strcmp(name, #_NAME_) != 0) { break; } \
            pp->_NAME_ = sqlite3_column_double(sp, ii); \
            ii += 1; \
        }

#define PLACER_INTEGER(_NAME_) \
        { \
            if (ii >= nn) { break; } \
            tt = sqlite3_column_type(sp, ii); \
            if (tt != SQLITE_INTEGER) { break; } \
            name = sqlite3_column_name(sp, ii); \
            if (strcmp(name, #_NAME_) != 0) { break; } \
            pp->_NAME_ = sqlite3_column_int(sp, ii); \
            ii += 1; \
        }

#define PLACER_INTEGER64(_NAME_) \
        { \
            if (ii >= nn) { break; } \
            tt = sqlite3_column_type(sp, ii); \
            if (tt != SQLITE_INTEGER) { break; } \
            name = sqlite3_column_name(sp, ii); \
            if (strcmp(name, #_NAME_) != 0) { break; } \
            pp->_NAME_ = sqlite3_column_int64(sp, ii); \
            ii += 1; \
        }

#define PLACER_TEXT(_NAME_, _ITEMS_) \
        { \
            if (ii >= nn) { break; } \
            tt = sqlite3_column_type(sp, ii); \
            if (tt != SQLITE_TEXT) { break; } \
            name = sqlite3_column_name(sp, ii); \
            if (strcmp(name, #_NAME_) != 0) { break; } \
            strncpy(pp->_NAME_, sqlite3_column_text(sp, ii), _ITEMS_); \
            pp->_NAME_[_ITEMS_ - 1] = '\0'; \
            ii += 1; \
        }

#define PLACER_TEXT16(_NAME_, _ITEMS_) \
        { \
            if (ii >= nn) { break; } \
            tt = sqlite3_column_type(sp, ii); \
            if (tt != SQLITE_TEXT) { break; } \
            name = sqlite3_column_name(sp, ii); \
            if (strcmp(name, #_NAME_) != 0) { break; } \
            wcsncpy(pp->_NAME_, sqlite3_column_text16(sp, ii), _ITEMS_); \
            pp->_NAME_[_ITEMS_ - 1] = L'\0'; \
            ii += 1; \
        }

#define PLACER_FIELD(_CONSTRAINTS_)

#define PLACER_FINAL(_CONSTRAINTS_)

#define PLACER_END(_CONSTRAINTS_) \
        (*ip) += 1; \
        rc = SQLITE_OK; \
    } while (0); \
    return rc; \
}
