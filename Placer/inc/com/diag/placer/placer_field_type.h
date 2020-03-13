/* vi: set ts=4 expandtab shiftwidth=4: */

/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA.
 * Licensed under the terms in LICENSE.txt.

 * The Field Type feature generates an array of SQLite3 field (not data)
 * type numbers from a schema. Note that there are no SQLITE_INTEGER64 or
 * SQLITE_TEXT16 field types.
 *
 * N.B. THIS HEADER FILE CAN BE INCLUDED MORE THAN ONCE.
 */

#include "placer.h"

/**
 * @def PLACER_SCHEMA(_STRUCTURE_)
 * This macro generates the front matter for an array that contains
 * the SQLite3 field type for each field in the schema. Note that
 * SQLite3 classifies both TEXT and TEXT16 fields as type TEXT,
 * and both INTEGER and INTEGER64 fields as type INTEGER.
 * @a _STRUCTURE_ is the schema name.
 */
#define PLACER_SCHEMA(_STRUCTURE_) \
    static const int PLACER_struct_##_STRUCTURE_##_FIELD_TYPE[] = {

/**
 * @def PLACER_BLOB(_NAME_, _ITEMS_)
 * This macro generates the code for a field of type BLOB.
 * @a _NAME_ parameter is the field name.
 * @a _ITEMS_ parameters is the number of placer_BLOB_t elements in the field.
 */
#define PLACER_BLOB(_NAME_, _ITEMS_) \
	( SQLITE_BLOB )

/**
 * @def PLACER_FLOAT(_ITEM_)
 * This macro generates the code for a field of type FLOAT.
 * @a _NAME_ parameter is the field name.
 */
#define PLACER_FLOAT(_NAME_) \
	( SQLITE_FLOAT )

/**
 * @def PLACER_INTEGER(_NAME_)
 * This macro generates the code for a field of type INTEGER.
 * @a _NAME_ parameter is the field name.
 */
#define PLACER_INTEGER(_NAME_) \
	( SQLITE_INTEGER )

/**
 * @def PLACER_INTEGER64(_NAME_)
 * This macro generates the code for a field of type INTEGER64.
 * @a _NAME_ parameter is the field name.
 */
#define PLACER_INTEGER64(_NAME_) \
	( SQLITE_INTEGER )

/**
 * @def PLACER_TEXT(_NAME_, _ITEMS_)
 * This macro generates the code for a field of type TEXT.
 * @a _NAME_ parameter is the field name.
 * @a _ITEMS_ parameters is the number of placer_TEXT_t elements in the field.
 */
#define PLACER_TEXT(_NAME_, _ITEMS_) \
	( SQLITE_TEXT )

/**
 * @def PLACER_TEXT16(_NAME_, _ITEMS_)
 * This macro generates the code for a field of type TEXT16.
 * @a _NAME_ parameter is the field name.
 * @a _ITEMS_ parameters is the number of placer_TEXT16_t elements in the field.
 */
#define PLACER_TEXT16(_NAME_, _ITEMS_) \
	( SQLITE_TEXT )

/**
 * @def PLACER_FIELD(_CONSTRAINTS_)
 * This macro generates the ending for any field that is not the last field.
 * @a _CONSTRAINTS_ is any additional contraints on the field.
 */
#define PLACER_FIELD(_CONSTRAINTS_) \
        ,

/**
 * @def PLACER_FINAL(_CONSTRAINTS_)
 * This macro generates the ending for any field that is the last field.
 * @a _CONSTRAINTS_ is any additional contraints on the field.
 */
#define PLACER_FINAL(_CONSTRAINTS_)

/**
 * @def PLACER_END(_CONSTRAINTS_)
 * This macro generates the end matter for a schema.
 * @a _CONSTRAINTS_ is any additional contraints on the schema.
 */
#define PLACER_END(_CONSTRAINTS_) \
    };

