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

#define COM_DIAG_PLACER_SCHEMA_BEGIN(_STRUCTURE_) \
static struct _STRUCTURE_ * com_diag_placer_schema__##_STRUCTURE_##_import(struct _STRUCTURE_ * pp, int nn, char ** vv, char ** kk) { \
    int ii = 0; \

#define COM_DIAG_PLACER_SCHEMA_BLOB(_NAME_, _ITEMS_, _CONSTRAINTS_) \
    if (ii < nn) { \
        placer_schema_BLOB_import(pp->_NAME_, _ITEMS_, vv[ii++]); \
    } \

#define COM_DIAG_PLACER_SCHEMA_BLOB_LAST(_NAME_, _ITEMS_, _CONSTRAINTS_) \
    if (ii < nn) { \
        placer_schema_BLOB_import(pp->_NAME_, _ITEMS_, vv[ii++]); \
    } \

#define COM_DIAG_PLACER_SCHEMA_FLOAT(_NAME_, _CONSTRAINTS_) \
    if (ii < nn) { \
        placer_schema_FLOAT_import(&(pp->_NAME_), vv[ii++]); \
    } \

#define COM_DIAG_PLACER_SCHEMA_FLOAT_LAST(_NAME_, _CONSTRAINTS_) \
    if (ii < nn) { \
        placer_schema_FLOAT_import(&(pp->_NAME_), vv[ii++]); \
    } \

#define COM_DIAG_PLACER_SCHEMA_INTEGER(_NAME_, _CONSTRAINTS_) \
    if (ii < nn) { \
        placer_schema_INTEGER_import(&(pp->_NAME_), vv[ii++]); \
    } \

#define COM_DIAG_PLACER_SCHEMA_INTEGER_LAST(_NAME_, _CONSTRAINTS_) \
    if (ii < nn) { \
        placer_schema_INTEGER_import(&(pp->_NAME_), vv[ii++]); \
    } \

#define COM_DIAG_PLACER_SCHEMA_INTEGER64(_NAME_, _CONSTRAINTS_) \
    if (ii < nn) { \
        placer_schema_INTEGER64_import(&(pp->_NAME_), vv[ii++]); \
    } \

#define COM_DIAG_PLACER_SCHEMA_INTEGER64_LAST(_NAME_, _CONSTRAINTS_) \
    if (ii < nn) { \
        placer_schema_INTEGER64_import(&(pp->_NAME_), vv[ii++]); \
    } \

#define COM_DIAG_PLACER_SCHEMA_TEXT(_NAME_, _ITEMS_, _CONSTRAINTS_) \
    if (ii < nn) { \
        placer_schema_TEXT_import(pp->_NAME_, _ITEMS_, vv[ii++]); \
    } \

#define COM_DIAG_PLACER_SCHEMA_TEXT_LAST(_NAME_, _ITEMS_, _CONSTRAINTS_) \
    if (ii < nn) { \
        placer_schema_TEXT_import(pp->_NAME_, _ITEMS_, vv[ii++]); \
    } \

#define COM_DIAG_PLACER_SCHEMA_TEXT16(_NAME_, _ITEMS_, _CONSTRAINTS_) \
    if (ii < nn) { \
        placer_schema_TEXT16_import(pp->_NAME_, _ITEMS_, vv[ii++]); \
    } \

#define COM_DIAG_PLACER_SCHEMA_TEXT16_LAST(_NAME_, _ITEMS_, _CONSTRAINTS_) \
    if (ii < nn) { \
        placer_schema_TEXT16_import(pp->_NAME_, _ITEMS_, vv[ii++]); \
    } \

#define COM_DIAG_PLACER_SCHEMA_END(_CONSTRAINTS_) \
    return pp; \
}
