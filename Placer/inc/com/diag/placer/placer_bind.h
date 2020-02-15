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

#include <sqlite3.h>
#include "placer.h"

#define PLACER_SCHEMA(_STRUCTURE_) \
int placer_struct_##_STRUCTURE_##_bind(sqlite3_stmt * sp, const struct _STRUCTURE_ * pp) { \
    int rc = SQLITE_ERROR; \
    int ii = 0; \
    int ll = 0; \
    do { \
        ll = sqlite3_bind_parameter_count(sp);

#define PLACER_BLOB(_NAME_, _ITEMS_) \
        { \
            if (ii >= ll) { rc = SQLITE_ERROR; break; } \
            ii += 1; \
            if ((rc = sqlite3_bind_blob(sp, ii, pp->_NAME_, (_ITEMS_) * sizeof(placer_BLOB_t), (placer_free_t *)0)) != SQLITE_OK) { break; } \
        }
    
#define PLACER_FLOAT(_NAME_) \
        { \
            if (ii >= ll) { rc = SQLITE_ERROR; break; } \
            ii += 1; \
            if ((rc = sqlite3_bind_double(sp, ii, pp->_NAME_)) != SQLITE_OK) { break; } \
        }

#define PLACER_INTEGER(_NAME_) \
        { \
            if (ii >= ll) { rc = SQLITE_ERROR; break; } \
            ii += 1; \
            if ((rc = sqlite3_bind_int(sp, ii, pp->_NAME_)) != SQLITE_OK) { break; } \
        }

#define PLACER_INTEGER64(_NAME_) \
        { \
            if (ii >= ll) { rc = SQLITE_ERROR; break; } \
            ii += 1; \
            if ((rc = sqlite3_bind_int64(sp, ii, pp->_NAME_)) != SQLITE_OK) { break; } \
        }

#define PLACER_TEXT(_NAME_, _ITEMS_) \
        { \
            if (ii >= ll) { rc = SQLITE_ERROR; break; } \
            ii += 1; \
            if ((rc = sqlite3_bind_text(sp, ii, pp->_NAME_, (_ITEMS_) * sizeof(placer_TEXT_t), (placer_free_t *)0)) != SQLITE_OK) { break; } \
        }

#define PLACER_TEXT16(_NAME_, _ITEMS_) \
        { \
            if (ii >= ll) { rc = SQLITE_ERROR; break; } \
            ii += 1; \
            if ((rc = sqlite3_bind_text16(sp, ii, pp->_NAME_, (_ITEMS_) * sizeof(placer_TEXT16_t), (placer_free_t *)0)) != SQLITE_OK) { break; } \
        }

#define PLACER_FIELD(_CONSTRAINTS_)

#define PLACER_FINAL(_CONSTRAINTS_)

#define PLACER_END(_CONSTRAINTS_) \
        rc = SQLITE_OK; \
    } while (0); \
    if (rc != SQLITE_OK) { \
        placer_error(rc); \
    } \
    return rc; \
}
