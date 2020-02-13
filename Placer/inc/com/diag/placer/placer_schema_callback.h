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

/*
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

#define PLACER_SCHEMA(_STRUCTURE_) \
int placer_##_STRUCTURE_##_callback(void * vp, int ncols, char ** value, char ** keyword) { \
    int rc = SQLITE_ERROR; \
    struct _STRUCTURE_ *** pp = (struct _STRUCTURE_ ***)0; \
    int ii = 0; \
    pp = (struct _STRUCTURE_ ***)vp; \
    do { \
        if (**pp == (struct _STRUCTURE_ *)0) { break; }

#define PLACER_BLOB(_NAME_, _ITEMS_) \
        if (ii >= ncols) { break; } \
        if (strcmp(keyword[ii], #_NAME_) != 0) { break; } \
        if (placer_BLOB_import((**pp)->_NAME_, _ITEMS_, value[ii++]) != SQLITE_OK) { break; }

#define PLACER_FLOAT(_NAME_) \
        if (ii >= ncols) { break; } \
        if (strcmp(keyword[ii], #_NAME_) != 0) { break; } \
        if (placer_FLOAT_import(&((**pp)->_NAME_), value[ii++]) != SQLITE_OK) { break; }

#define PLACER_INTEGER(_NAME_) \
        if (ii >= ncols) { break; } \
        if (strcmp(keyword[ii], #_NAME_) != 0) { break; } \
        if (placer_INTEGER_import(&((**pp)->_NAME_), value[ii++]) != SQLITE_OK) { break; }

#define PLACER_INTEGER64(_NAME_) \
        if (ii >= ncols) { break; } \
        if (strcmp(keyword[ii], #_NAME_) != 0) { break; } \
        if (placer_INTEGER64_import(&((**pp)->_NAME_), value[ii++]) != SQLITE_OK) { break; }

#define PLACER_TEXT(_NAME_, _ITEMS_) \
        if (ii >= ncols) { break; } \
        if (strcmp(keyword[ii], #_NAME_) != 0) { break; } \
        if (placer_TEXT_import((**pp)->_NAME_, _ITEMS_, value[ii++]) != SQLITE_OK) { break; }

#define PLACER_TEXT16(_NAME_, _ITEMS_) \
        if (ii >= ncols) { break; } \
        if (strcmp(keyword[ii], #_NAME_) != 0) { break; } \
        if (placer_TEXT16_import((**pp)->_NAME_, _ITEMS_, value[ii++]) != SQLITE_OK) { break; }

#define PLACER_FIELD(_CONSTRAINTS_)

#define PLACER_FINAL(_CONSTRAINTS_)

#define PLACER_END(_CONSTRAINTS_) \
        (*pp) += 1; \
        rc = SQLITE_OK; \
    } while (0); \
    return rc; \
}
