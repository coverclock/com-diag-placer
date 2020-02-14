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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "com/diag/placer/placer_structure.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_callback_prototype.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_callback.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_bind_prototype.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_bind.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

int main(void)
{
    SETLOGMASK();

    {
        COMMENT("sizeof(struct UnitTestSchema)=%zu\n", sizeof(struct UnitTestSchema));
        EXPECT(sizeof(struct UnitTestSchema) == 4392);
    }

    {
        struct UnitTestSchema record =
#include "com/diag/placer/placer_structure_initializer.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"
        COMMENT("sizeof(record)=%zu\n", sizeof(record));
        EXPECT(sizeof(record) == 4392);
        EXPECT(record.id == (int32_t)0);
        EXPECT(record.name[0] == L'\0');
        EXPECT(record.age == (double)0.0);
        EXPECT(record.image[0] == (uint8_t)0);
        EXPECT(record.sn == (int64_t)0);
        EXPECT(record.ssn[0] == '\0');
    }

    {
        const char CREATE[] = "CREATE TABLE UnitTestSchema (id INTEGER PRIMARY KEY, name TEXT , age FLOAT , image BLOB , sn INTEGER , ssn TEXT  );";
        const char * create =
#include "com/diag/placer/placer_create.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"
        COMMENT("create=\"%s\"\n", create);
        EXPECT(strcmp(create, CREATE) == 0);
    }

    {
        placer_INTEGER_t result = 0xa5a5a5a5;
        EXPECT(placer_INTEGER_import(&result, "INVALID") == SQLITE_ERROR);
        EXPECT(placer_INTEGER_import(&result, "1NV8L1D") == SQLITE_ERROR);
        EXPECT(placer_INTEGER_import(&result, "0") == SQLITE_OK);
        EXPECT(result == 0);
        EXPECT(placer_INTEGER_import(&result, "2147483647") == SQLITE_OK);
        EXPECT(result == 2147483647);
        EXPECT(placer_INTEGER_import(&result, "-1") == SQLITE_OK);
        EXPECT(result == -1);
    }

    {
        placer_INTEGER64_t result = 0xa5a5a5a5a5a5a5a5LL;
        EXPECT(placer_INTEGER64_import(&result, "INVALID") == SQLITE_ERROR);
        EXPECT(placer_INTEGER64_import(&result, "1NV8L1D") == SQLITE_ERROR);
        EXPECT(placer_INTEGER64_import(&result, "0") == SQLITE_OK);
        EXPECT(result == 0);
        EXPECT(placer_INTEGER64_import(&result, "9223372036854775807") == SQLITE_OK);
        EXPECT(result == 9223372036854775807LL);
        EXPECT(placer_INTEGER64_import(&result, "-1") == SQLITE_OK);
        EXPECT(result == -1);
    }

    {
        placer_FLOAT_t result = 165.165;
        EXPECT(placer_FLOAT_import(&result, "INVALID") == SQLITE_ERROR);
        EXPECT(placer_FLOAT_import(&result, "1NV8L1D") == SQLITE_ERROR);
        EXPECT(placer_FLOAT_import(&result, "0") == SQLITE_OK);
        EXPECT(result == 0.0);
        EXPECT(placer_FLOAT_import(&result, "0.0") == SQLITE_OK);
        EXPECT(result == 0.0);
        EXPECT(placer_FLOAT_import(&result, "526.625") == SQLITE_OK);
        EXPECT(result == 526.625);
        EXPECT(placer_FLOAT_import(&result, "-1.0") == SQLITE_OK);
        EXPECT(result == -1.0);
    }

    {
        placer_TEXT_t result[9] = { '0', '1', '2', '3', '4', '5', '6', '7', '\0', };
        EXPECT(placer_TEXT_import(result, 9, "") == SQLITE_OK);
        EXPECT(result[0] == '\0');
        EXPECT(placer_TEXT_import(result, 9, "A") == SQLITE_OK);
        EXPECT(strcmp(result, "A") == 0);
        EXPECT(placer_TEXT_import(result, 9, "BCDEFGHI") == SQLITE_OK);
        EXPECT(strcmp(result, "BCDEFGHI") == 0);
        EXPECT(placer_TEXT_import(result, 9, "JKLMNOPQR") == SQLITE_ERROR);
        EXPECT(strcmp(result, "JKLMNOPQ") == 0);
    }

    {
        placer_TEXT16_t result[9] = { L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'\0', };
        EXPECT(placer_TEXT16_import(result, 9, (char *)L"") == SQLITE_OK);
        EXPECT(result[0] == L'\0');
        EXPECT(placer_TEXT16_import(result, 9, (char *)L"A") == SQLITE_OK);
        EXPECT(wcscmp(result, L"A") == 0);
        EXPECT(placer_TEXT16_import(result, 9, (char *)L"BCDEFGHI") == SQLITE_OK);
        EXPECT(wcscmp(result, L"BCDEFGHI") == 0);
        EXPECT(placer_TEXT16_import(result, 9, (char *)L"JKLMNOPQR") == SQLITE_ERROR);
        EXPECT(wcscmp(result, L"JKLMNOPQ") == 0);
    }

    {
        placer_BLOB_t result[8]  = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, };
        char value[2][8] = {
            { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, },
            { 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00, },
        };
        EXPECT(placer_BLOB_import(result, sizeof(result), value[0]) == SQLITE_OK);
        EXPECT(memcmp(result, value[0], sizeof(result)) == 0);
        EXPECT(placer_BLOB_import(result, sizeof(result), value[1]) == SQLITE_OK);
        EXPECT(memcmp(result, value[1], sizeof(result)) == 0);
    }

    {
        struct UnitTestSchema results[3] = { { 0, } };
        struct UnitTestSchema * gather[] = { &(results[0]), &(results[1]), &(results[2]), (struct UnitTestSchema *)0 };
        struct UnitTestSchema ** here = &(gather[0]);
        placer_BLOB_t image[4][4096] = { { 0x11, }, { 0x22, }, { 0x33, }, { 0x44 }, };
        char * value[4][6] = {
            { "1", (char *)L"Chip Overclock", "63.625", (char *)&(image[0]), "42", "123456789", },
            { "2", (char *)L"Red Black", "51.", (char *)&(image[1]), "86", "234567890", },
            { "3", (char *)L"The French Woman", "62", (char *)&(image[2]), "99", "345678901", },
            { "4", (char *)L"Dread Pirate Roberts", "31", (char *)&(image[3]), "1", "456789012", },
        };
        char * keyword[] = { "id", "name", "age", "image", "sn", "ssn", };
        int rc;

        COMMENT("1");        
        rc = placer_UnitTestSchema_callback((void *)&here, 6, value[0], keyword);
        ASSERT(rc == SQLITE_OK);
        EXPECT(here = &(gather[1]));
        EXPECT(results[0].id == (int32_t)1);
        EXPECT(wcscmp(results[0].name,  L"Chip Overclock") == 0);
        EXPECT(results[0].age == (double)63.625);
        EXPECT(results[0].image[0] == (uint8_t)0x11);
        EXPECT(results[0].sn == (int64_t)42);
        EXPECT(strcmp(results[0].ssn, "123456789") == 0);

        COMMENT("2");        
        rc = placer_UnitTestSchema_callback((void *)&here, 6, value[1], keyword);
        ASSERT(rc == SQLITE_OK);
        EXPECT(here = &(gather[2]));
        EXPECT(results[1].id == (int32_t)2);
        EXPECT(wcscmp(results[1].name,  L"Red Black") == 0);
        EXPECT(results[1].age == (double)51.0);
        EXPECT(results[1].image[0] == (uint8_t)0x22);
        EXPECT(results[1].sn == (int64_t)86);
        EXPECT(strcmp(results[1].ssn, "234567890") == 0);

        COMMENT("3");        
        rc = placer_UnitTestSchema_callback((void *)&here, 6, value[2], keyword);
        ASSERT(rc == SQLITE_OK);
        EXPECT(here = &(gather[3]));
        EXPECT(results[2].id == (int32_t)3);
        EXPECT(wcscmp(results[2].name,  L"The French Woman") == 0);
        EXPECT(results[2].age == (double)62);
        EXPECT(results[2].image[0] == (uint8_t)0x33);
        EXPECT(results[2].sn == (int64_t)99);
        EXPECT(strcmp(results[2].ssn, "345678901") == 0);

        COMMENT("4");        
        rc = placer_UnitTestSchema_callback((void *)&here, 6, value[3], keyword);
        ASSERT(rc == SQLITE_ERROR);
        EXPECT(here = &(gather[3]));
    }

    EXIT();
}
