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
 * The prototype for this function matches that of an SQLite3 callback
 * function, so that it can be used directly as a callback, or just called
 * from within a callback. Casting the void pointer to a triple indirect
 * variable makes even my brain hurt. The void pointer points to a variable
 * whose value is a pointer to an array that contains pointers to the
 * structure. So the first dereference gets to the variable that points to
 * the array; incrementing that variable indexs through the array. The
 * second dereference accesses a pointer to a structure. The list of pointers
 * to structures in the awway is terminated by a null structure pointer.
 * This allows this callback to be used in an operation that selects rows
 * from a table table, import the  columns from each row, and populate
 * several structures. The generated function is paranoid: it not only
 * checks for the array not having enough entries, but for the number of
 * of columns not matching, for the names of the columns not matching,
 * and for the data type imports failing.
 */

#include "placer.h"

#define PLACER_SCHEMA(_STRUCTURE_) \
int placer_struct_##_STRUCTURE_##_callback(void * vp, int ncols, char ** value, char ** keyword) { \
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
            if ((rc = placer_BLOB_import(pp->_NAME_, (_ITEMS_), value[ii])) != SQLITE_OK) { break; } \
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
            if ((rc = placer_TEXT_import(pp->_NAME_, (_ITEMS_), value[ii])) != SQLITE_OK) { break; } \
            ii += 1; \
        }

#define PLACER_TEXT16(_NAME_, _ITEMS_) \
        { \
            if (ii >= ncols) { rc = SQLITE_ERROR; break; } \
            if (strcmp(keyword[ii], #_NAME_) != 0) { rc = SQLITE_ERROR; break; } \
            if ((rc = placer_TEXT16_import(pp->_NAME_, (_ITEMS_), value[ii])) != SQLITE_OK) { break; } \
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
