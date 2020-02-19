/* vi: set ts=4 expandtab shiftwidth=4: */

/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 * These X macros generate a SQL REPLACE command for a schema.
 * N.B. THIS HEADER FILE CAN BE INCLUDED MORE THAN ONCE.
 */

#define PLACER_SCHEMA(_STRUCTURE_) \
    static const char PLACER_STRUCT_##_STRUCTURE_##_SQL_REPLACE[] = \
        "REPLACE INTO " #_STRUCTURE_ " VALUES ("

#define PLACER_BLOB(_NAME_, _ITEMS_) \
            ":"#_NAME_

#define PLACER_FLOAT(_NAME_) \
            ":"#_NAME_

#define PLACER_INTEGER(_NAME_) \
            ":"#_NAME_

#define PLACER_INTEGER64(_NAME_) \
            ":"#_NAME_

#define PLACER_TEXT(_NAME_, _ITEMS_) \
            ":"#_NAME_

#define PLACER_TEXT16(_NAME_, _ITEMS_) \
            ":"#_NAME_

#define PLACER_FIELD(_CONSTRAINTS_) \
            ", "

#define PLACER_FINAL(_CONSTRAINTS_) \
            " "

#define PLACER_END(_CONSTRAINTS_) \
        ");";
