/* vi: set ts=4 expandtab shiftwidth=4: */

/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 * These X macros generate a callback function for a schema.
 * N.B. THIS HEADER FILE CAN BE INCLUDED MORE THAN ONCE.
 * Note that the keyword and value index is ZERO based.
 */

#include "placer.h"

#define PLACER_SCHEMA(_STRUCTURE_) \
int placer_struct_##_STRUCTURE_##_exec_callback(void * vp, int ncols, char ** value, char ** keyword) { \
    int rc = SQLITE_ERROR; \
    struct _STRUCTURE_ *** ip = (struct _STRUCTURE_ ***)0; \
    struct _STRUCTURE_ ** ap = (struct _STRUCTURE_ **)0; \
    struct _STRUCTURE_ * pp = (struct _STRUCTURE_ *)0; \
    int ii = 0; \
    ip = (struct _STRUCTURE_ ***)vp; \
    ap = *ip; \
    pp = *ap; \
    do { \
        if (pp == (struct _STRUCTURE_ *)0) { break; }

#define PLACER_BLOB(_NAME_, _ITEMS_) \
        { \
            if (ii >= ncols) { rc = SQLITE_ERROR; break; } \
            if (strcmp(keyword[ii], #_NAME_) != 0) { rc = SQLITE_ERROR; break; } \
            if ((rc = placer_BLOB_import(pp->_NAME_, value[ii], (_ITEMS_))) != SQLITE_OK) { break; } \
            ii += 1; \
        }

#define PLACER_FLOAT(_NAME_) \
        { \
            if (ii >= ncols) { rc = SQLITE_ERROR; break; } \
            if (strcmp(keyword[ii], #_NAME_) != 0) { rc = SQLITE_ERROR; break; } \
            if ((rc = placer_FLOAT_import(&(pp->_NAME_), value[ii])) != SQLITE_OK) { break; } \
            ii += 1; \
        }

#define PLACER_INTEGER(_NAME_) \
        { \
            if (ii >= ncols) { rc = SQLITE_ERROR; break; } \
            if (strcmp(keyword[ii], #_NAME_) != 0) { rc = SQLITE_ERROR; break; } \
            if ((rc = placer_INTEGER_import(&(pp->_NAME_), value[ii])) != SQLITE_OK) { break; } \
            ii += 1; \
        }

#define PLACER_INTEGER64(_NAME_) \
        { \
            if (ii >= ncols) { rc = SQLITE_ERROR; break; } \
            if (strcmp(keyword[ii], #_NAME_) != 0) { rc = SQLITE_ERROR; break; } \
            if ((rc = placer_INTEGER64_import(&(pp->_NAME_), value[ii])) != SQLITE_OK) { break; } \
            ii += 1; \
        }

#define PLACER_TEXT(_NAME_, _ITEMS_) \
        { \
            if (ii >= ncols) { rc = SQLITE_ERROR; break; } \
            if (strcmp(keyword[ii], #_NAME_) != 0) { rc = SQLITE_ERROR; break; } \
            if ((rc = placer_TEXT_import(pp->_NAME_, value[ii], (_ITEMS_))) != SQLITE_OK) { break; } \
            ii += 1; \
        }

#define PLACER_TEXT16(_NAME_, _ITEMS_) \
        { \
            if (ii >= ncols) { rc = SQLITE_ERROR; break; } \
            if (strcmp(keyword[ii], #_NAME_) != 0) { rc = SQLITE_ERROR; break; } \
            if ((rc = placer_TEXT16_import(pp->_NAME_, value[ii], (_ITEMS_))) != SQLITE_OK) { break; } \
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
