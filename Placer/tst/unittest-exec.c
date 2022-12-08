/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2020-2022 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in LICENSE.txt<BR>
 * Chip Overclock <coverclock@diag.com><BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 *
 * Note that the name field is decared to be TEXT16. Since C (AFAIK)
 * has no direct support for UTF-16 literals, we have to fake it.
 * Fortunately (also AFAIK) the code points in UTF-8 that correspond
 * to standard ASCII overlaps with UTF-16. UTF-16 is durectly supported
 * in Java and is used in a lot of database systems for
 * internationalization.
 *
 * The sqlite3_exec() callback returns all fields except blobs as TEXT
 * (UTF-8). I haven't quite figured out what this means for TEXT16
 * (UTF-16) fields. But it probably means you should use sqlite3_exec()
 * (or placer_exec()) if you are using TEXT16 fields. The
 * sqlite3_step() (or placer_steps()) approach seems to work just
 * fine.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "com/diag/diminuto/diminuto_unittest.h"
#include "com/diag/diminuto/diminuto_fs.h"
#include "com/diag/diminuto/diminuto_countof.h"
#include "com/diag/diminuto/diminuto_dump.h"
#include "../src/placer.h" /* Private API. */
#include "com/diag/placer/placer.h"

#include "com/diag/placer/placer_structure_definition.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_exec_prototype.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_exec_callback.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

int main(void)
{
    SETLOGMASK();

    {
        placer_INTEGER_t result = 0xa5a5a5a5;

        TEST();
        EXPECT(placer_INTEGER_exec_import(&result, "INVALID") != SQLITE_OK);
        EXPECT(placer_INTEGER_exec_import(&result, "1NV8L1D") != SQLITE_OK);
        EXPECT(placer_INTEGER_exec_import(&result, "0") == SQLITE_OK);
        EXPECT(result == 0);
        EXPECT(placer_INTEGER_exec_import(&result, "2147483647") == SQLITE_OK);
        EXPECT(result == 2147483647);
        EXPECT(placer_INTEGER_exec_import(&result, "-1") == SQLITE_OK);
        EXPECT(result == -1);
        STATUS();
    }

    {
        placer_INTEGER64_t result = 0xa5a5a5a5a5a5a5a5LL;

        TEST();
        EXPECT(placer_INTEGER64_exec_import(&result, "INVALID") != SQLITE_OK);
        EXPECT(placer_INTEGER64_exec_import(&result, "1NV8L1D") != SQLITE_OK);
        EXPECT(placer_INTEGER64_exec_import(&result, "0") == SQLITE_OK);
        EXPECT(result == 0);
        EXPECT(placer_INTEGER64_exec_import(&result, "9223372036854775807") == SQLITE_OK);
        EXPECT(result == 9223372036854775807LL);
        EXPECT(placer_INTEGER64_exec_import(&result, "-1") == SQLITE_OK);
        EXPECT(result == -1);
        STATUS();
    }

    {
        placer_FLOAT_t result = 165.165;

        TEST();
        EXPECT(placer_FLOAT_exec_import(&result, "INVALID") != SQLITE_OK);
        EXPECT(placer_FLOAT_exec_import(&result, "1NV8L1D") != SQLITE_OK);
        EXPECT(placer_FLOAT_exec_import(&result, "0") == SQLITE_OK);
        EXPECT(result == 0.0);
        EXPECT(placer_FLOAT_exec_import(&result, "0.0") == SQLITE_OK);
        EXPECT(result == 0.0);
        EXPECT(placer_FLOAT_exec_import(&result, "526.625") == SQLITE_OK);
        EXPECT(result == 526.625);
        EXPECT(placer_FLOAT_exec_import(&result, "-1.0") == SQLITE_OK);
        EXPECT(result == -1.0);
        STATUS();
    }

    {
        placer_TEXT_t result[] = { '0', '1', '2', '3', '4', '5', '6', '7', '\0', };

        TEST();
        EXPECT(placer_TEXT_exec_import((placer_TEXT_t *)0, (const char *)0, 0) == SQLITE_OK);
        EXPECT(placer_TEXT_exec_import(result, "", countof(result)) == SQLITE_OK);
        EXPECT(result[0] == '\0');
        EXPECT(placer_TEXT_exec_import(result, "A", countof(result)) == SQLITE_OK);
        EXPECT(strcmp((const char *)result, "A") == 0);
        EXPECT(placer_TEXT_exec_import(result, "BCDEFGHI", countof(result)) == SQLITE_OK);
        EXPECT(strcmp((const char *)result, "BCDEFGHI") == 0);
        EXPECT(placer_TEXT_exec_import(result, "JKLMNOPQR", countof(result)) == SQLITE_OK);
        EXPECT(strcmp((const char *)result, "JKLMNOPQR") == 0);
        STATUS();
    }

    {
        static const char DATA1[] = "";
        static const char DATA2[] = "A";
        static const char DATA3[] = "BCDEFGHI";
        static const char DATA4[] = "JKLMNOPQR";
        static const placer_TEXT16_t RESULT1[] = { '\0' };
        static const placer_TEXT16_t RESULT2[] = { 'A', '\0' };
        static const placer_TEXT16_t RESULT3[] = { 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', '\0' };
        static const placer_TEXT16_t RESULT4[] = { 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', '\0' };
        placer_TEXT16_t result[]  = { '0', '1', '2', '3', '4', '5', '6', '7', '\0' };

        TEST();

        COMMENT("0");
        EXPECT(placer_TEXT16_exec_import((placer_TEXT16_t *)0, (const char *)0, 0) == SQLITE_OK);
 
        COMMENT("1");
        EXPECT(placer_TEXT16_exec_import(result, (char *)DATA1, countof(result)) == SQLITE_OK);
        diminuto_dump(stderr, DATA1, sizeof(DATA1));
        diminuto_dump(stderr, RESULT1, sizeof(RESULT1));
        diminuto_dump(stderr, result, sizeof(result));
        EXPECT(result[0] == '\0');
 
        COMMENT("2");
        EXPECT(placer_TEXT16_exec_import(result, (char *)DATA2, countof(result)) == SQLITE_OK);
        diminuto_dump(stderr, DATA2, sizeof(DATA2));
        diminuto_dump(stderr, RESULT2, sizeof(RESULT2));
        diminuto_dump(stderr, result, sizeof(result));
        EXPECT(placer_TEXT16_compare(result, RESULT2, countof(result)) == 0);
 
        COMMENT("3");
        EXPECT(placer_TEXT16_exec_import(result, (char *)DATA3, countof(result)) == SQLITE_OK);
        diminuto_dump(stderr, DATA3, sizeof(DATA3));
        diminuto_dump(stderr, RESULT3, sizeof(RESULT3));
        diminuto_dump(stderr, result, sizeof(result));
        EXPECT(placer_TEXT16_compare(result, RESULT3, countof(result)) == 0);
 
        COMMENT("4");
        EXPECT(placer_TEXT16_exec_import(result, (char *)DATA4, countof(result)) == SQLITE_OK);
        diminuto_dump(stderr, DATA4, sizeof(DATA4));
        diminuto_dump(stderr, RESULT4, sizeof(RESULT4));
        diminuto_dump(stderr, result, sizeof(result));
        EXPECT(placer_TEXT16_compare(result, RESULT4, countof(result)) == 0);

        STATUS();
    }

    {
        placer_BLOB_t result[]  = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, };
        char value[2][8] = {
            { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, },
            { 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00, },
        };

        TEST();

        EXPECT(placer_BLOB_exec_import((placer_BLOB_t *)0, (const char *)0, 0) == SQLITE_OK);

        EXPECT(placer_BLOB_exec_import(result, value[0], countof(result)) == SQLITE_OK);
        EXPECT(memcmp(result, value[0], sizeof(result)) == 0);

        EXPECT(placer_BLOB_exec_import(result, value[1], countof(result)) == SQLITE_OK);
        EXPECT(memcmp(result, value[1], sizeof(result)) == 0);

        STATUS();
    }

    {
        static const placer_TEXT16_t RESULT1[] = { 'C', 'h', 'i', 'p', ' ', 'O', 'v', 'e', 'r', 'c', 'l', 'o', 'c', 'k', 0 };
        static const placer_TEXT16_t RESULT2[] = { 'R', 'e', 'd', ' ', 'B', 'l', 'a', 'c', 'k', 0 };
        static const placer_TEXT16_t RESULT3[] = { 'T', 'h', 'e', ' ', 'F', 'r', 'e', 'n', 'c', 'h', ' ', 'W', 'o', 'm', 'a', 'n', 0 };
        struct UnitTestSchema results[3] = { { 0, } };
        struct UnitTestSchema * gather[] = { &(results[0]), &(results[1]), &(results[2]), (struct UnitTestSchema *)0 };
        struct UnitTestSchema ** here = &(gather[0]);
        placer_BLOB_t image[4][256] = { { 0x11, }, { 0x22, }, { 0x33, }, { 0x44 }, };
        char * value[4][6] = {
            { "1", "Chip Overclock", "63.625", (char *)&(image[0]), "42", "123456789", },
            { "2", "Red Black", "51.", (char *)&(image[1]), "86", "234567890", },
            { "3", "The French Woman", "62", (char *)&(image[2]), "99", "345678901", },
            { "4", "Dread Pirate Roberts", "31", (char *)&(image[3]), "1", "456789012", },
        };
        char * keyword[] = { "id", "name", "age", "image", "sn", "ssn", };
        int rc;

        TEST();

        COMMENT("1");        
        rc = placer_struct_UnitTestSchema_exec_callback((void *)&here, 6, value[0], keyword);
        ASSERT(rc == SQLITE_OK);
        EXPECT(here = &(gather[1]));
        EXPECT(results[0].id == (int32_t)1);
        diminuto_dump(stderr, RESULT1, sizeof(RESULT1));
        diminuto_dump(stderr, results[0].name, sizeof(results[0].name));
        EXPECT(placer_TEXT16_compare(results[0].name, RESULT1, countof(results[0].name)) == 0);
        EXPECT(results[0].age == (double)63.625);
        EXPECT(results[0].image[0] == (uint8_t)0x11);
        EXPECT(results[0].sn == (int64_t)42);
        EXPECT(strcmp((const char *)results[0].ssn, "123456789") == 0);

        COMMENT("2");        
        rc = placer_struct_UnitTestSchema_exec_callback((void *)&here, 6, value[1], keyword);
        ASSERT(rc == SQLITE_OK);
        EXPECT(here = &(gather[2]));
        EXPECT(results[1].id == (int32_t)2);
        diminuto_dump(stderr, RESULT2, sizeof(RESULT2));
        diminuto_dump(stderr, results[1].name, sizeof(results[1].name));
        EXPECT(placer_TEXT16_compare(results[1].name, RESULT2, countof(results[1].name)) == 0);
        EXPECT(results[1].age == (double)51.0);
        EXPECT(results[1].image[0] == (uint8_t)0x22);
        EXPECT(results[1].sn == (int64_t)86);
        EXPECT(strcmp((const char *)results[1].ssn, "234567890") == 0);

        COMMENT("3");        
        rc = placer_struct_UnitTestSchema_exec_callback((void *)&here, 6, value[2], keyword);
        ASSERT(rc == SQLITE_OK);
        EXPECT(here = &(gather[3]));
        EXPECT(results[2].id == (int32_t)3);
        diminuto_dump(stderr, RESULT3, sizeof(RESULT3));
        diminuto_dump(stderr, results[2].name, sizeof(results[2].name));
        EXPECT(placer_TEXT16_compare(results[2].name, RESULT3, countof(results[2].name)) == 0);
        EXPECT(results[2].age == (double)62);
        EXPECT(results[2].image[0] == (uint8_t)0x33);
        EXPECT(results[2].sn == (int64_t)99);
        EXPECT(strcmp((const char *)results[2].ssn, "345678901") == 0);

        COMMENT("4");        
        rc = placer_struct_UnitTestSchema_exec_callback((void *)&here, 6, value[3], keyword);
        ASSERT(rc != SQLITE_OK);
        EXPECT(here = &(gather[3]));

        STATUS();
    }


    EXIT();
}
