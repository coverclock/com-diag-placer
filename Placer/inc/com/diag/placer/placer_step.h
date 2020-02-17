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
 * Note that the column index is ZERO based.
 */

#include <string.h>
#include <sqlite3.h>
#include "placer.h"

#define PLACER_SCHEMA(_STRUCTURE_) \
int placer_struct_##_STRUCTURE_##_step(sqlite3_stmt * sp, void * vp) { \
    int rc = SQLITE_ERROR; \
    struct _STRUCTURE_ *** ip = (struct _STRUCTURE_ ***)0; \
    struct _STRUCTURE_ ** ap = (struct _STRUCTURE_ **)0; \
    struct _STRUCTURE_ * pp = (struct _STRUCTURE_ *)0; \
    int count = 0; \
    int type = 0; \
    int bytes = 0; \
    int ii = 0; \
    const char * name = (const char *)0; \
    ip = (struct _STRUCTURE_ ***)vp; \
    ap = *ip; \
    pp = *ap; \
    do { \
        if (pp == (struct _STRUCTURE_ *)0) { break; } \
        count = sqlite3_column_count(sp); \

#define PLACER_BLOB(_NAME_, _ITEMS_) \
        { \
            const placer_BLOB_t * blob = (const placer_BLOB_t *)0; \
            if (ii >= count) { break; } \
            type = sqlite3_column_type(sp, ii); \
            if (type != SQLITE_BLOB) {  break; } \
            name = sqlite3_column_name(sp, ii); \
            if (strcmp(name, #_NAME_) != 0) { break; } \
            blob = sqlite3_column_blob(sp, ii); \
            bytes = sqlite3_column_bytes(sp, ii); \
            if (bytes > ((_ITEMS_) * sizeof(placer_BLOB_t))) { break; } \
            memcpy(pp->_NAME_, blob, bytes); \
            ii += 1; \
        }

#define PLACER_FLOAT(_NAME_) \
        { \
            if (ii >= count) { break; } \
            type = sqlite3_column_type(sp, ii); \
            if (type != SQLITE_FLOAT) { break; } \
            name = sqlite3_column_name(sp, ii); \
            if (strcmp(name, #_NAME_) != 0) { break; } \
            pp->_NAME_ = sqlite3_column_double(sp, ii); \
            ii += 1; \
        }

#define PLACER_INTEGER(_NAME_) \
        { \
            if (ii >= count) { break; } \
            type = sqlite3_column_type(sp, ii); \
            if (type != SQLITE_INTEGER) { break; } \
            name = sqlite3_column_name(sp, ii); \
            if (strcmp(name, #_NAME_) != 0) { break; } \
            pp->_NAME_ = sqlite3_column_int(sp, ii); \
            ii += 1; \
        }

#define PLACER_INTEGER64(_NAME_) \
        { \
            if (ii >= count) { break; } \
            type = sqlite3_column_type(sp, ii); \
            if (type != SQLITE_INTEGER) { break; } \
            name = sqlite3_column_name(sp, ii); \
            if (strcmp(name, #_NAME_) != 0) { break; } \
            pp->_NAME_ = sqlite3_column_int64(sp, ii); \
            ii += 1; \
        }

#define PLACER_TEXT(_NAME_, _ITEMS_) \
        { \
            const placer_TEXT_t * text = (const placer_TEXT_t *)0; \
            if (ii >= count) { break; } \
            type = sqlite3_column_type(sp, ii); \
            if (type != SQLITE_TEXT) { break; } \
            name = sqlite3_column_name(sp, ii); \
            if (strcmp(name, #_NAME_) != 0) { break; } \
            text = sqlite3_column_text(sp, ii); \
            bytes = sqlite3_column_bytes(sp, ii); \
            if (bytes > (((_ITEMS_) - 1) * sizeof(placer_TEXT_t))) { break; } \
            strncpy(pp->_NAME_, text, bytes); \
            pp->_NAME_[_ITEMS_ - 1] = '\0'; \
            ii += 1; \
        }

#define PLACER_TEXT16(_NAME_, _ITEMS_) \
        { \
            const placer_TEXT16_t * text16 = (const placer_TEXT16_t *)0; \
            if (ii >= count) { break; } \
            type = sqlite3_column_type(sp, ii); \
            if (type != SQLITE_TEXT) { break; } \
            name = sqlite3_column_name(sp, ii); \
            if (strcmp(name, #_NAME_) != 0) { break; } \
            text16 = sqlite3_column_text16(sp, ii); \
            bytes = sqlite3_column_bytes16(sp, ii); \
            if (bytes > (((_ITEMS_) - 1) * sizeof(placer_TEXT16_t))) { break; } \
            placer_TEXT16_copy(pp->_NAME_, text16, bytes / sizeof(placer_TEXT16_t)); \
            pp->_NAME_[_ITEMS_ - 1] = 0; \
            ii += 1; \
        }

#define PLACER_FIELD(_CONSTRAINTS_)

#define PLACER_FINAL(_CONSTRAINTS_)

#define PLACER_END(_CONSTRAINTS_) \
        (*ip) += 1; \
        rc = SQLITE_OK; \
    } while (0); \
    if (rc != SQLITE_OK) { \
        placer_error(rc); \
    } \
    return rc; \
}
