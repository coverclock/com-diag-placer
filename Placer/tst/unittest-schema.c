/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in LICENSE.txt<BR>
 * Chip Overclock <coverclock@diag.com><BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 */

#include "com/diag/diminuto/diminuto_unittest.h"
#include "com/diag/placer/placer.h"
#include "com/diag/placer/placer_schema.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "com/diag/placer/placer_schema_structure.h"
#include "unittest-schema.h"

#include "com/diag/placer/placer_schema_import.h"
#include "unittest-schema.h"

int main(void)
{
    SETLOGMASK();

    {
        struct UnitTestSchema record =
#include "com/diag/placer/placer_schema_initializer.h"
#include "unittest-schema.h"

        COMMENT("sizeof(struct UnitTestSchema)=%zu\n", sizeof(struct UnitTestSchema));
        COMMENT("sizeof(record)=%zu\n", sizeof(record));
        EXPECT(sizeof(struct UnitTestSchema) == 40);
        EXPECT(sizeof(record) == 40);
        EXPECT(record.id == (int32_t)0);
        EXPECT(record.name[0] == L'\0');
        EXPECT(record.age == (double)0.0);
        EXPECT(record.image[0] == (uint8_t)0);
        EXPECT(record.sn == (int64_t)0);
        EXPECT(record.ssn[0] == '\0');
    }

    {
        const char * create =
#include "com/diag/placer/placer_schema_create.h"
#include "unittest-schema.h"
        const char CREATE[] = "CREATE TABLE UnitTestSchema (id INTEGER PRIMARY KEY, name TEXT , age FLOAT , image BLOB , sn INTEGER , ssn TEXT , );";

        COMMENT("create=\"%s\"\n", create);
        EXPECT(strcmp(create, CREATE) == 0);
    }

    EXIT();
}
