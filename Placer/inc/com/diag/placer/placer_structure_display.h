/* vi: set ts=4 expandtab shiftwidth=4: */

/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA.
 * Licensed under the terms in LICENSE.txt.
 *
 * The Structure Display feature generates a structure display function
 * from a schema.
 *
 * N.B. THIS HEADER FILE CAN BE INCLUDED MORE THAN ONCE.
 */

#include <stdio.h>
#include "com/diag/diminuto/diminuto_phex.h"
#include "com/diag/placer/placer.h"
#include "placer.h"

/**
 * @def PLACER_SCHEMA(_STRUCTURE_)
 * This macro generates the front matter for a function to display
 * a schema structure on the specified FILE stream.
 * @a _STRUCTURE_ is the schema name.
 */
#define PLACER_SCHEMA(_STRUCTURE_) \
    void placer_struct_##_STRUCTURE_##_display(FILE * fp, const struct _STRUCTURE_ * pp) { \
        fprintf(fp, "[%s]\n", #_STRUCTURE_);

/**
 * @def PLACER_BLOB(_NAME_, _ITEMS_)
 * This macro generates the code for a field of type BLOB.
 * @a _NAME_ parameter is the field name.
 * @a _ITEMS_ parameters is the number of placer_BLOB_t elements in the field.
 */
#define PLACER_BLOB(_NAME_, _ITEMS_) \
        { \
            const placer_BLOB_t * bb = (const placer_BLOB_t *)0; \
            size_t ll = 0; \
            int ii = 0; \
            size_t current = 0; \
            int end = 0; \
            fprintf(fp, "%s=\"", #_NAME_); \
            bb = &(pp->_NAME_[0]); \
            ll = sizeof(pp->_NAME_); \
            for (ii = 0; ii < ll; ++ii, ++bb) { \
                if (*bb == '\"') { \
                    fputs("\\\"", fp); \
                } else { \
                    diminuto_phex_emit(fp, *bb, ~(size_t)0, 0, 0, 0, &current, &end, 0); \
                } \
            } \
            fputs("\";\n", fp); \
        }

/**
 * @def PLACER_FLOAT(_ITEM_)
 * This macro generates the code for a field of type FLOAT.
 * @a _NAME_ parameter is the field name.
 */
#define PLACER_FLOAT(_NAME_) \
         { \
             fprintf(fp, "%s=%lf\n", #_NAME_, pp->_NAME_); \
         }

/**
 * @def PLACER_INTEGER(_NAME_)
 * This macro generates the code for a field of type INTEGER.
 * @a _NAME_ parameter is the field name.
 */
#define PLACER_INTEGER(_NAME_) \
         { \
             fprintf(fp, "%s=%lld\n", #_NAME_, (long long)(pp->_NAME_)); \
         }

/**
 * @def PLACER_INTEGER64(_NAME_)
 * This macro generates the code for a field of type INTEGER64.
 * @a _NAME_ parameter is the field name.
 */
#define PLACER_INTEGER64(_NAME_) \
        { \
            fprintf(fp, "%s=%lld\n", #_NAME_, (long long)(pp->_NAME_)); \
        }

/**
 * @def PLACER_TEXT(_NAME_, _ITEMS_)
 * This macro generates the code for a field of type TEXT.
 * @a _NAME_ parameter is the field name.
 * @a _ITEMS_ parameters is the number of placer_TEXT_t elements in the field.
 */
#define PLACER_TEXT(_NAME_, _ITEMS_) \
        { \
            const placer_TEXT_t * tt = (const placer_TEXT_t *)0; \
            size_t ll = 0; \
            int ii = 0; \
            size_t current = 0; \
            int end = 0; \
            fprintf(fp, "%s=\"", #_NAME_); \
            tt = &(pp->_NAME_[0]); \
            ll = strnlen(pp->_NAME_, (_ITEMS_)); \
            for (ii = 0; ii < ll; ++ii, ++tt) { \
                if (*tt == '\"') { \
                    fputs("\\\"", fp); \
                } else { \
                    diminuto_phex_emit(fp, *tt, ~(size_t)0, 0, 0, 0, &current, &end, 0); \
                } \
            } \
            fputs("\";\n", fp); \
        }

/**
 * @def PLACER_TEXT16(_NAME_, _ITEMS_)
 * This macro generates the code for a field of type TEXT16.
 * @a _NAME_ parameter is the field name.
 * @a _ITEMS_ parameters is the number of placer_TEXT16_t elements in the field.
 */
#define PLACER_TEXT16(_NAME_, _ITEMS_) \
        { \
            const placer_TEXT16_t * uu = (const placer_TEXT16_t *)0; \
            size_t ll = 0; \
            int ii = 0; \
            size_t current = 0; \
            int end = 0; \
            fprintf(fp, "%s=\"", #_NAME_); \
            uu = &(pp->_NAME_[0]); \
            ll = placer_TEXT16_length(pp->_NAME_, (_ITEMS_)); \
            for (ii = 0; ii < ll; ++ii, ++uu) { \
                if (!((' ' <= *uu) && (*uu <= '~'))) { \
                    fprintf(fp, "\\x%2.2x\\x%2.2x", (((*uu) & 0xff00) >> 16), ((*uu) & 0x00ff)); \
                } else if (*uu == '\"') { \
                    fputs("\\\"", fp); \
                } else { \
                    fputc((char)*uu, fp); \
                } \
            } \
            fputs("\";\n", fp); \
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
 * This macro generates the end matter for a function to display
 * a schema structure on the specified FILE stream.
 * @a _CONSTRAINTS_ is any additional contraints on the schema.
 */
#define PLACER_END(_CONSTRAINTS_) \
    }
