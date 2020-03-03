/* vi: set ts=4 expandtab shiftwidth=4: */

/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 * These X macros generate a bind function for a schema.
 * N.B. THIS HEADER FILE CAN BE INCLUDED MORE THAN ONCE.
 * Note that the parameter index is ONE based.
 */

#include <string.h>
#include <sqlite3.h>
#include "placer.h"

/**
 * @def PLACER_SCHEMA(_STRUCTURE_)
 * This macro generates the front matter for a function which binds each of
 * the fields in the schema to a SQLite3 statement object which has been
 * prepared with indexed fields. This statement object can have been prepared
 * from an SQL statement that is the result of another set of schema macros, or
 * from SQL statement generated by the application.
 * @a _STRUCTURE_ is the schema name.
 */
#define PLACER_SCHEMA(_STRUCTURE_) \
int placer_stmt_struct_##_STRUCTURE_##_bind(sqlite3_stmt * sp, const struct _STRUCTURE_ * pp) { \
    int rc = SQLITE_ERROR; \
    int ii = 0; \
    int ll = 0; \
    do { \
        ll = sqlite3_bind_parameter_count(sp);

/**
 * @def PLACER_BLOB(_NAME_, _ITEMS_)
 * This macro generates the code for a field of type BLOB.
 * @a _NAME_ parameter is the field name.
 * @a _ITEMS_ parameters is the number of placer_BLOB_t elements in the field.
 */
#define PLACER_BLOB(_NAME_, _ITEMS_) \
        { \
            if (ii >= ll) { rc = SQLITE_ERROR; break; } \
            ii += 1; \
            if ((rc = sqlite3_bind_blob(sp, ii, pp->_NAME_, (_ITEMS_) * sizeof(placer_BLOB_t), (placer_bind_callback_t *)0)) != SQLITE_OK) { break; } \
        }
    
/**
 * @def PLACER_FLOAT(_ITEM_)
 * This macro generates the code for a field of type FLOAT.
 * @a _NAME_ parameter is the field name.
 */
#define PLACER_FLOAT(_NAME_) \
        { \
            if (ii >= ll) { rc = SQLITE_ERROR; break; } \
            ii += 1; \
            if ((rc = sqlite3_bind_double(sp, ii, pp->_NAME_)) != SQLITE_OK) { break; } \
        }

/**
 * @def PLACER_INTEGER(_NAME_)
 * This macro generates the code for a field of type INTEGER.
 * @a _NAME_ parameter is the field name.
 */
#define PLACER_INTEGER(_NAME_) \
        { \
            if (ii >= ll) { rc = SQLITE_ERROR; break; } \
            ii += 1; \
            if ((rc = sqlite3_bind_int(sp, ii, pp->_NAME_)) != SQLITE_OK) { break; } \
        }

/**
 * @def PLACER_INTEGER64(_NAME_)
 * This macro generates the code for a field of type INTEGER64.
 * @a _NAME_ parameter is the field name.
 */
#define PLACER_INTEGER64(_NAME_) \
        { \
            if (ii >= ll) { rc = SQLITE_ERROR; break; } \
            ii += 1; \
            if ((rc = sqlite3_bind_int64(sp, ii, pp->_NAME_)) != SQLITE_OK) { break; } \
        }

/**
 * @def PLACER_TEXT(_NAME_, _ITEMS_)
 * This macro generates the code for a field of type TEXT.
 * @a _NAME_ parameter is the field name.
 * @a _ITEMS_ parameters is the number of placer_TEXT_t elements in the field.
 */
#define PLACER_TEXT(_NAME_, _ITEMS_) \
        { \
            if (ii >= ll) { rc = SQLITE_ERROR; break; } \
            ii += 1; \
            if ((rc = sqlite3_bind_text(sp, ii, pp->_NAME_, strnlen(pp->_NAME_, (_ITEMS_)) * sizeof(placer_TEXT_t), (placer_bind_callback_t *)0)) != SQLITE_OK) { break; } \
        }

/**
 * @def PLACER_TEXT16(_NAME_, _ITEMS_)
 * This macro generates the code for a field of type TEXT16.
 * @a _NAME_ parameter is the field name.
 * @a _ITEMS_ parameters is the number of placer_TEXT16_t elements in the field.
 */
#define PLACER_TEXT16(_NAME_, _ITEMS_) \
        { \
            if (ii >= ll) { rc = SQLITE_ERROR; break; } \
            ii += 1; \
            if ((rc = sqlite3_bind_text16(sp, ii, pp->_NAME_, placer_TEXT16_length(pp->_NAME_, (_ITEMS_)) * sizeof(placer_TEXT16_t), (placer_bind_callback_t *)0)) != SQLITE_OK) { break; } \
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
 * This macro generates the end matter for a function which binds each of
 * the fields in the schema to a SQLite3 statement object which has been
 * prepared with indexed fields.
 * @a _CONSTRAINTS_ is any additional contraints on the schema.
 */
#define PLACER_END(_CONSTRAINTS_) \
        rc = SQLITE_OK; \
    } while (0); \
    if (rc != SQLITE_OK) { \
        placer_error(rc); \
    } \
    return rc; \
}
