/* vi: set ts=4 expandtab shiftwidth=4: */

/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA.
 * Licensed under the terms in LICENSE.txt.
 *
 * The Structure Name feature generates a variable pointing to
 * a string containing the name for a schema.
 *
 * N.B. THIS HEADER FILE CAN BE INCLUDED MORE THAN ONCE.
 */

#include "placer.h"

/**
 * @def PLACER_SCHEMA(_STRUCTURE_)
 * This macro generates the front matter for an variable containing
 * a pointer to a string containing the table name of a schema.
 * @a _STRUCTURE_ is the schema name.
 */
#define PLACER_SCHEMA(_STRUCTURE_) \
    static const char * PLACER_struct_##_STRUCTURE_##_SCHEMA_NAME = #_STRUCTURE_

/**
 * @def PLACER_BLOB(_NAME_, _ITEMS_)
 * This macro generates the code for a field of type BLOB.
 * @a _NAME_ parameter is the field name.
 * @a _ITEMS_ parameters is the number of placer_BLOB_t elements in the field.
 */
#define PLACER_BLOB(_NAME_, _ITEMS_)

/**
 * @def PLACER_FLOAT(_ITEM_)
 * This macro generates the code for a field of type FLOAT.
 * @a _NAME_ parameter is the field name.
 */
#define PLACER_FLOAT(_NAME_)
/**
 * @def PLACER_INTEGER(_NAME_)
 * This macro generates the code for a field of type INTEGER.
 * @a _NAME_ parameter is the field name.
 */
#define PLACER_INTEGER(_NAME_)

/**
 * @def PLACER_INTEGER64(_NAME_)
 * This macro generates the code for a field of type INTEGER64.
 * @a _NAME_ parameter is the field name.
 */
#define PLACER_INTEGER64(_NAME_)

/**
 * @def PLACER_TEXT(_NAME_, _ITEMS_)
 * This macro generates the code for a field of type TEXT.
 * @a _NAME_ parameter is the field name.
 * @a _ITEMS_ parameters is the number of placer_TEXT_t elements in the field.
 */
#define PLACER_TEXT(_NAME_, _ITEMS_)

/**
 * @def PLACER_TEXT16(_NAME_, _ITEMS_)
 * This macro generates the code for a field of type TEXT16.
 * @a _NAME_ parameter is the field name.
 * @a _ITEMS_ parameters is the number of placer_TEXT16_t elements in the field.
 */
#define PLACER_TEXT16(_NAME_, _ITEMS_)

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
 * This macro generates the end matter for a schema name array.
 * @a _CONSTRAINTS_ is any additional contraints on the schema.
 */
#define PLACER_END(_CONSTRAINTS_) \
    ;
