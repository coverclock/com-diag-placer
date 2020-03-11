/* vi: set ts=4 expandtab shiftwidth=4: */

/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA.
 * Licensed under the terms in LICENSE.txt.
 *
 * The Exec Callback feature generates an exec callback function from
 * a schema. Note that the keyword and value index is ZERO based.
 *
 * N.B. THIS HEADER FILE CAN BE INCLUDED MORE THAN ONCE.
 */

#include "placer.h"

/**
 * @def PLACER_SCHEMA(_STRUCTURE_)
 * This macro generates the front matter for a placer_exec() callback
 * function that imports the fields of the schema into the table when
 * used with an INSERT or REPLACE operation. The function is externally
 * visible by default.
 * @a _STRUCTURE_ is the schema name.
 */
#define PLACER_SCHEMA(_STRUCTURE_) \
int placer_exec_struct_##_STRUCTURE_##_callback(void * vp, int ncols, char ** value, char ** keyword) { \
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

/**
 * @def PLACER_BLOB(_NAME_, _ITEMS_)
 * This macro generates the code for a field of type BLOB.
 * @a _NAME_ parameter is the field name.
 * @a _ITEMS_ parameters is the number of placer_BLOB_t elements in the field.
 */
#define PLACER_BLOB(_NAME_, _ITEMS_) \
        { \
            if (ii >= ncols) { rc = SQLITE_ERROR; break; } \
            if (strcmp(keyword[ii], #_NAME_) != 0) { rc = SQLITE_ERROR; break; } \
            if ((rc = placer_exec_BLOB_import(pp->_NAME_, value[ii], (_ITEMS_))) != SQLITE_OK) { break; } \
            ii += 1; \
        }

/**
 * @def PLACER_FLOAT(_ITEM_)
 * This macro generates the code for a field of type FLOAT.
 * @a _NAME_ parameter is the field name.
 */
#define PLACER_FLOAT(_NAME_) \
        { \
            if (ii >= ncols) { rc = SQLITE_ERROR; break; } \
            if (strcmp(keyword[ii], #_NAME_) != 0) { rc = SQLITE_ERROR; break; } \
            if ((rc = placer_exec_FLOAT_import(&(pp->_NAME_), value[ii])) != SQLITE_OK) { break; } \
            ii += 1; \
        }

/**
 * @def PLACER_INTEGER(_NAME_)
 * This macro generates the code for a field of type INTEGER.
 * @a _NAME_ parameter is the field name.
 */
#define PLACER_INTEGER(_NAME_) \
        { \
            if (ii >= ncols) { rc = SQLITE_ERROR; break; } \
            if (strcmp(keyword[ii], #_NAME_) != 0) { rc = SQLITE_ERROR; break; } \
            if ((rc = placer_exec_INTEGER_import(&(pp->_NAME_), value[ii])) != SQLITE_OK) { break; } \
            ii += 1; \
        }

/**
 * @def PLACER_INTEGER64(_NAME_)
 * This macro generates the code for a field of type INTEGER64.
 * @a _NAME_ parameter is the field name.
 */
#define PLACER_INTEGER64(_NAME_) \
        { \
            if (ii >= ncols) { rc = SQLITE_ERROR; break; } \
            if (strcmp(keyword[ii], #_NAME_) != 0) { rc = SQLITE_ERROR; break; } \
            if ((rc = placer_exec_INTEGER64_import(&(pp->_NAME_), value[ii])) != SQLITE_OK) { break; } \
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
            if (ii >= ncols) { rc = SQLITE_ERROR; break; } \
            if (strcmp(keyword[ii], #_NAME_) != 0) { rc = SQLITE_ERROR; break; } \
            if ((rc = placer_exec_TEXT_import(pp->_NAME_, value[ii], (_ITEMS_))) != SQLITE_OK) { break; } \
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
            if (ii >= ncols) { rc = SQLITE_ERROR; break; } \
            if (strcmp(keyword[ii], #_NAME_) != 0) { rc = SQLITE_ERROR; break; } \
            if ((rc = placer_exec_TEXT16_import(pp->_NAME_, value[ii], (_ITEMS_))) != SQLITE_OK) { break; } \
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
 * This macro generates the emd matter for a placer_exec() callback
 * function that imports the fields of the schema into the table when
 * used with an INSERT or REPLACE operation.
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
