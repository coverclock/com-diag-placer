/* vi: set ts=4 expandtab shiftwidth=4: */

/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA.
 * Licensed under the terms in LICENSE.txt.
 *
 * The Steps Callback feature generates a step callback function from
 * a schema. Note that the column index is ZERO based.
 *
 * N.B. THIS HEADER FILE CAN BE INCLUDED MORE THAN ONCE.
 */

#include <string.h>
#include <sqlite3.h>
#include "placer.h"

/**
 * @def PLACER_SCHEMA(_STRUCTURE_)
 * This macro generates the front matter for a placer_steps() callback
 * function that loads the data from a single selected row in a table
 * into a structure for the schema. The structure is pointed to by an
 * element in a null-terminated array of structure pointers through which
 * successive calls to the callback function indexes. Each successive row
 * is loaded into the next structure until either the selection of rows is
 * exhausted or the null pointer at the end of the structure pointer array
 * is reached.
 * @a _STRUCTURE_ is the schema name.
 */
#define PLACER_SCHEMA(_STRUCTURE_) \
int placer_struct_##_STRUCTURE_##_steps_callback(sqlite3_stmt * sp, void * vp) { \
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

/**
 * @def PLACER_BLOB(_NAME_, _ITEMS_)
 * This macro generates the code for a field of type BLOB.
 * @a _NAME_ parameter is the field name.
 * @a _ITEMS_ parameters is the number of placer_BLOB_t elements in the field.
 */
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

/**
 * @def PLACER_FLOAT(_ITEM_)
 * This macro generates the code for a field of type FLOAT.
 * @a _NAME_ parameter is the field name.
 */
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

/**
 * @def PLACER_INTEGER(_NAME_)
 * This macro generates the code for a field of type INTEGER.
 * @a _NAME_ parameter is the field name.
 */
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

/**
 * @def PLACER_INTEGER64(_NAME_)
 * This macro generates the code for a field of type INTEGER64.
 * @a _NAME_ parameter is the field name.
 */
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

/**
 * @def PLACER_TEXT(_NAME_, _ITEMS_)
 * This macro generates the code for a field of type TEXT.
 * @a _NAME_ parameter is the field name.
 * @a _ITEMS_ parameters is the number of placer_TEXT_t elements in the field.
 */
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
            if (bytes > ((_ITEMS_) * sizeof(placer_TEXT_t))) { break; } \
            strncpy(pp->_NAME_, text, bytes); \
            if (bytes < ((_ITEMS_) * sizeof(placer_TEXT_t))) { pp->_NAME_[bytes / sizeof(placer_TEXT_t)] = '\0'; } \
            ii += 1; \
        }

/**
 * @def PLACER_TEXT16(_NAME_, _ITEMS_)
 * This macro generates the code for a field of type TEXT16.
 * @a _NAME_ parameter is the field name.
 * @a _ITEMS_ parameters is the number of placer_TEXT16_t elements in the field.
 */
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
            if (bytes > ((_ITEMS_) * sizeof(placer_TEXT16_t))) { break; } \
            placer_TEXT16_copy(pp->_NAME_, text16, bytes / sizeof(placer_TEXT16_t)); \
            if (bytes < ((_ITEMS_) * sizeof(placer_TEXT16_t))) { pp->_NAME_[bytes / sizeof(placer_TEXT16_t)] = '\0'; } \
            ii += 1; \
        }

/**
 * @def PLACER_FIELD(_CONSTRAINTS_)
 * This macro generates the ending for any field that is not the last field.
 * @a _CONSTRAINTS_ is any additional contraints on the field.
 */
#define PLACER_FIELD(_CONSTRAINTS_)

/**
 * @def PLACER_FINAL(_CONSTRAINTS_)
 * This macro generates the ending for any field that is the last field.
 * @a _CONSTRAINTS_ is any additional contraints on the field.
 */
#define PLACER_FINAL(_CONSTRAINTS_)

/**
 * @def PLACER_END(_CONSTRAINTS_)
 * This macro generates the end matter for a placer_steps() callback
 * function that loads the data from a single selected row in a table
 * into a structure for the schema.
 * @a _CONSTRAINTS_ is any additional contraints on the schema.
 */
#define PLACER_END(_CONSTRAINTS_) \
        (*ip) += 1; \
        rc = SQLITE_OK; \
    } while (0); \
    if (rc != SQLITE_OK) { \
        placer_error(rc); \
    } \
    return rc; \
}
