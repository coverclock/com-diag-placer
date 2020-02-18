/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in LICENSE.txt<BR>
 * Chip Overclock <coverclock@diag.com><BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "com/diag/diminuto/diminuto_unittest.h"
#include "com/diag/diminuto/diminuto_countof.h"
#include "com/diag/diminuto/diminuto_dump.h"
#include "com/diag/placer/placer.h"

#include "com/diag/placer/placer_structure.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_exec_prototype.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_exec.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_steps_prototype.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_steps.h"
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
        TEST();
        COMMENT("sizeof(struct UnitTestSchema)=%zu\n", sizeof(struct UnitTestSchema));
        EXPECT(sizeof(struct UnitTestSchema) == 4264);
        STATUS();
    }

    {
        struct UnitTestSchema record =
#include "com/diag/placer/placer_structure_initializer.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

        TEST();
        COMMENT("sizeof(record)=%zu\n", sizeof(record));
        EXPECT(sizeof(record) == 4264);
        EXPECT(record.id == (int32_t)0);
        EXPECT(record.name[0] == L'\0');
        EXPECT(record.age == (double)0.0);
        EXPECT(record.image[0] == (uint8_t)0);
        EXPECT(record.sn == (int64_t)0);
        EXPECT(record.ssn[0] == '\0');
        STATUS();
    }

    {
        const char CREATE[] = "CREATE TABLE UnitTestSchema (id INTEGER PRIMARY KEY, name TEXT , age FLOAT , image BLOB , sn INTEGER , ssn TEXT  );";
        const char * create =
#include "com/diag/placer/placer_create.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

        TEST();
        COMMENT("create=\"%s\"\n", create);
        EXPECT(strcmp(create, CREATE) == 0);
        STATUS();
    }

    {
        const char INSERT[] = "INSERT INTO UnitTestSchema VALUES (:id, :name, :age, :image, :sn, :ssn );";
        const char * insert =
#include "com/diag/placer/placer_insert.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

        TEST();
        COMMENT("insert=\"%s\"\n", insert);
        EXPECT(strcmp(insert, INSERT) == 0);
        STATUS();
    }

    {
        const char REPLACE[] = "REPLACE INTO UnitTestSchema VALUES (:id, :name, :age, :image, :sn, :ssn );";
        const char * replace =
#include "com/diag/placer/placer_replace.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

        TEST();
        COMMENT("replace=\"%s\"\n", replace);
        EXPECT(strcmp(replace, REPLACE) == 0);
        STATUS();
    }

    {
        placer_INTEGER_t result = 0xa5a5a5a5;

        TEST();
        EXPECT(placer_INTEGER_import(&result, "INVALID") == SQLITE_ERROR);
        EXPECT(placer_INTEGER_import(&result, "1NV8L1D") == SQLITE_ERROR);
        EXPECT(placer_INTEGER_import(&result, "0") == SQLITE_OK);
        EXPECT(result == 0);
        EXPECT(placer_INTEGER_import(&result, "2147483647") == SQLITE_OK);
        EXPECT(result == 2147483647);
        EXPECT(placer_INTEGER_import(&result, "-1") == SQLITE_OK);
        EXPECT(result == -1);
        STATUS();
    }

    {
        placer_INTEGER64_t result = 0xa5a5a5a5a5a5a5a5LL;

        TEST();
        EXPECT(placer_INTEGER64_import(&result, "INVALID") == SQLITE_ERROR);
        EXPECT(placer_INTEGER64_import(&result, "1NV8L1D") == SQLITE_ERROR);
        EXPECT(placer_INTEGER64_import(&result, "0") == SQLITE_OK);
        EXPECT(result == 0);
        EXPECT(placer_INTEGER64_import(&result, "9223372036854775807") == SQLITE_OK);
        EXPECT(result == 9223372036854775807LL);
        EXPECT(placer_INTEGER64_import(&result, "-1") == SQLITE_OK);
        EXPECT(result == -1);
        STATUS();
    }

    {
        placer_FLOAT_t result = 165.165;

        TEST();
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
        STATUS();
    }

    {
        placer_TEXT_t result[] = { '0', '1', '2', '3', '4', '5', '6', '7', '\0', };

        TEST();
        EXPECT(placer_TEXT_import(result, "", countof(result)) == SQLITE_OK);
        EXPECT(result[0] == '\0');
        EXPECT(placer_TEXT_import(result, "A", countof(result)) == SQLITE_OK);
        EXPECT(strcmp(result, "A") == 0);
        EXPECT(placer_TEXT_import(result, "BCDEFGHI", countof(result)) == SQLITE_OK);
        EXPECT(strcmp(result, "BCDEFGHI") == 0);
        EXPECT(placer_TEXT_import(result, "JKLMNOPQR", countof(result)) == SQLITE_ERROR);
        EXPECT(strcmp(result, "JKLMNOPQ") == 0);
        STATUS();
    }

    {
        static const placer_TEXT16_t DATA1[] = { 0 };
        static const placer_TEXT16_t DATA2[] = { 'A', 0 };
        static const placer_TEXT16_t DATA3[] = { 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 0 };
        static const placer_TEXT16_t DATA4[] = { 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 0 };

        TEST();
        EXPECT(placer_TEXT16_length(DATA1, countof(DATA1)) == 0);
        EXPECT(placer_TEXT16_length(DATA2, countof(DATA2)) == 1);
        EXPECT(placer_TEXT16_length(DATA3, countof(DATA3)) == 8);
        EXPECT(placer_TEXT16_length(DATA4, countof(DATA4)) == 9);
        EXPECT(placer_TEXT16_length(DATA4, countof(DATA3)) == 9);
        EXPECT(placer_TEXT16_length(DATA4, countof(DATA3) - 1) == 8);
        STATUS();
    }

    {
        static const placer_TEXT16_t DATA1[] = { 0 };
        static const placer_TEXT16_t DATA2[] = { 'A', 0 };
        static const placer_TEXT16_t DATA3[] = { 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 0 };
        static const placer_TEXT16_t DATA4[] = { 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 0 };
        static const placer_TEXT16_t DATA5[] = { 'B', 0 };

        TEST();

        EXPECT(placer_TEXT16_compare(DATA1, DATA1, countof(DATA1)) == 0);
        EXPECT(placer_TEXT16_compare(DATA1, DATA2, countof(DATA1)) == -1);
        EXPECT(placer_TEXT16_compare(DATA1, DATA3, countof(DATA1)) == -1);
        EXPECT(placer_TEXT16_compare(DATA1, DATA4, countof(DATA1)) == -1);
        EXPECT(placer_TEXT16_compare(DATA1, DATA5, countof(DATA1)) == -1);
        EXPECT(placer_TEXT16_compare(DATA1, DATA5, 1) == -1);
        EXPECT(placer_TEXT16_compare(DATA1, DATA5, 0) == 0);

        EXPECT(placer_TEXT16_compare(DATA2, DATA1, countof(DATA2)) == 1);
        EXPECT(placer_TEXT16_compare(DATA2, DATA2, countof(DATA2)) == 0);
        EXPECT(placer_TEXT16_compare(DATA2, DATA3, countof(DATA2)) == -1);
        EXPECT(placer_TEXT16_compare(DATA2, DATA4, countof(DATA2)) == -1);
        EXPECT(placer_TEXT16_compare(DATA2, DATA5, countof(DATA2)) == -1);
        EXPECT(placer_TEXT16_compare(DATA2, DATA5, 1) == -1);
        EXPECT(placer_TEXT16_compare(DATA2, DATA5, 0) == 0);

        EXPECT(placer_TEXT16_compare(DATA3, DATA1, countof(DATA3)) == 1);
        EXPECT(placer_TEXT16_compare(DATA3, DATA2, countof(DATA3)) == 1);
        EXPECT(placer_TEXT16_compare(DATA3, DATA3, countof(DATA3)) == 0);
        EXPECT(placer_TEXT16_compare(DATA3, DATA4, countof(DATA3)) == -1);
        EXPECT(placer_TEXT16_compare(DATA3, DATA5, countof(DATA3)) == 1);
        EXPECT(placer_TEXT16_compare(DATA3, DATA5, 1) == 0);
        EXPECT(placer_TEXT16_compare(DATA3, DATA5, 0) == 0);

        EXPECT(placer_TEXT16_compare(DATA4, DATA1, countof(DATA4)) == 1);
        EXPECT(placer_TEXT16_compare(DATA4, DATA2, countof(DATA4)) == 1);
        EXPECT(placer_TEXT16_compare(DATA4, DATA3, countof(DATA4)) == 1);
        EXPECT(placer_TEXT16_compare(DATA4, DATA4, countof(DATA4)) == 0);
        EXPECT(placer_TEXT16_compare(DATA4, DATA5, countof(DATA4)) == 1);
        EXPECT(placer_TEXT16_compare(DATA4, DATA5, 1) == 1);
        EXPECT(placer_TEXT16_compare(DATA4, DATA5, 0) == 0);

        STATUS();
    }

    {
        static const placer_TEXT16_t DATA1[] = { 0 };
        static const placer_TEXT16_t DATA2[] = { 'A', 0 };
        static const placer_TEXT16_t DATA3[] = { 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 0 };
        static const placer_TEXT16_t DATA4[] = { 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 0 };
        static const placer_TEXT16_t DATA5[] = { 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 0 };
        placer_TEXT16_t result[] = { '0', '1', '2', '3', '4', '5', '6', '7', 0, };

        TEST();
        EXPECT(placer_TEXT16_copy(result, DATA1, countof(result)) == result);
        EXPECT(placer_TEXT16_compare(result, DATA1, countof(result)) == 0);
        EXPECT(result[countof(result) - 1] == 0);
        EXPECT(placer_TEXT16_copy(result, DATA2, countof(result)) == result);
        EXPECT(placer_TEXT16_compare(result, DATA2, countof(result)) == 0);
        EXPECT(result[countof(result) - 1] == 0);
        EXPECT(placer_TEXT16_copy(result, DATA3, countof(result)) == result);
        EXPECT(placer_TEXT16_compare(result, DATA3, countof(result)) == 0);
        EXPECT(result[countof(result) - 1] == 0);
        EXPECT(placer_TEXT16_copy(result, DATA4, countof(result)) == result);
        EXPECT(placer_TEXT16_compare(result, DATA4, countof(result)) == 0);
        EXPECT(result[countof(result) - 1] == 'R');
        STATUS();
    }

#if 0
    {
        static const placer_TEXT16_t DATA1[] = { '\0' };
        static const placer_TEXT16_t DATA2[] = { 'A', '\0' };
        static const placer_TEXT16_t DATA3[] = { 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', '\0' };
        static const placer_TEXT16_t DATA4[] = { 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', '\0' };
        static const placer_TEXT16_t DATA5[] = { 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', '\0' };
        placer_TEXT16_t result[] = { '0', '1', '2', '3', '4', '5', '6', '7', '\0', };

        TEST();
        EXPECT(placer_TEXT16_import(result, (char *)DATA1, countof(result)) == SQLITE_OK);
        EXPECT(result[0] == '\0');
        EXPECT(placer_TEXT16_import(result, (char *)DATA2, countof(result)) == SQLITE_OK);
        EXPECT(memcmp(result, DATA2, sizeof(DATA2)) == 0);
        EXPECT(placer_TEXT16_import(result, (char *)DATA3, countof(result)) == SQLITE_OK);
        EXPECT(memcmp(result, DATA3, sizeof(DATA3)) == 0);
        EXPECT(placer_TEXT16_import(result, (char *)DATA4, countof(result)) == SQLITE_ERROR);
        EXPECT(memcmp(result, DATA5, sizeof(DATA5)) == 0);
        STATUS();
    }
#endif

    {
        placer_BLOB_t result[]  = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, };
        char value[2][8] = {
            { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, },
            { 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00, },
        };

        TEST();
        EXPECT(placer_BLOB_import(result, value[0], countof(result)) == SQLITE_OK);
        EXPECT(memcmp(result, value[0], sizeof(result)) == 0);
        EXPECT(placer_BLOB_import(result, value[1], countof(result)) == SQLITE_OK);
        EXPECT(memcmp(result, value[1], sizeof(result)) == 0);
        STATUS();
    }

#if 0
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

        TEST();

        COMMENT("1");        
        rc = placer_struct_UnitTestSchema_exec_callback((void *)&here, 6, value[0], keyword);
        ASSERT(rc == SQLITE_OK);
        EXPECT(here = &(gather[1]));
        EXPECT(results[0].id == (int32_t)1);
        EXPECT(wcscmp(results[0].name,  L"Chip Overclock") == 0);
        EXPECT(results[0].age == (double)63.625);
        EXPECT(results[0].image[0] == (uint8_t)0x11);
        EXPECT(results[0].sn == (int64_t)42);
        EXPECT(strcmp(results[0].ssn, "123456789") == 0);

        COMMENT("2");        
        rc = placer_struct_UnitTestSchema__exec_callback((void *)&here, 6, value[1], keyword);
        ASSERT(rc == SQLITE_OK);
        EXPECT(here = &(gather[2]));
        EXPECT(results[1].id == (int32_t)2);
        EXPECT(wcscmp(results[1].name,  L"Red Black") == 0);
        EXPECT(results[1].age == (double)51.0);
        EXPECT(results[1].image[0] == (uint8_t)0x22);
        EXPECT(results[1].sn == (int64_t)86);
        EXPECT(strcmp(results[1].ssn, "234567890") == 0);

        COMMENT("3");        
        rc = placer_struct_UnitTestSchema__exec_callback((void *)&here, 6, value[2], keyword);
        ASSERT(rc == SQLITE_OK);
        EXPECT(here = &(gather[3]));
        EXPECT(results[2].id == (int32_t)3);
        EXPECT(wcscmp(results[2].name,  L"The French Woman") == 0);
        EXPECT(results[2].age == (double)62);
        EXPECT(results[2].image[0] == (uint8_t)0x33);
        EXPECT(results[2].sn == (int64_t)99);
        EXPECT(strcmp(results[2].ssn, "345678901") == 0);

        COMMENT("4");        
        rc = placer_struct_UnitTestSchema__exec_callback((void *)&here, 6, value[3], keyword);
        ASSERT(rc == SQLITE_ERROR);
        EXPECT(here = &(gather[3]));

        STATUS();
    }
#endif

    {
        static const char PATH[] = "out/host/sql/unittest-schema.db";
        static const char CREATE[] =
#include "com/diag/placer/placer_create.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"
        static const char INSERT[] =
#include "com/diag/placer/placer_insert.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"
        static const char REPLACE[] =
#include "com/diag/placer/placer_replace.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"
        static const char SELECT[] = "SELECT * FROM UnitTestSchema;";
        /*
         * Note that the name field is decared to be TEXT16. Since C (AFAIK)
         * has no direct literal support for UTF-16, we have to fake it.
         * Fortunately (also AFAIK) the code points in UTF-8 that correspond
         * to standard ASCII overlaps with UTF-16.
         */
        static const struct UnitTestSchema ROW[4] = {
            { 1, { 'C', 'h', 'i', 'p', ' ', 'O', 'v', 'e', 'r', 'c', 'l', 'o', 'c', 'k', 0 },                               63.625, { 0x11, }, 42LL, "123456789", }, 
            { 2, { 'R', 'e', 'd', ' ', 'B', 'l', 'a', 'c', 'k', 0 },                                                        51.,    { 0x22, }, 86LL, "234567890", },
            { 3, { 'T', 'h', 'e', ' ', 'F', 'r', 'e', 'n', 'c', 'h', ' ', 'W', 'o', 'm', 'a', 'n', 0 },                     62,     { 0x33, }, 99LL, "345678901", },
            { 4, { 'D', 'r', 'e', 'a', 'd', ' ', 'P', 'i', 'r', 'a', 't', 'e', ' ', 'R', 'o', 'b', 'e', 'r', 't', 's', 0 }, 31,     { 0x44, },  1LL, "456789012", },
        };
        struct UnitTestSchema row[4];
        struct UnitTestSchema * rows[] = { &row[0], &row[1], &row[2], &row[3], (struct UnitTestSchema *)0, };
        struct UnitTestSchema ** here = (struct UnitTestSchema **)0;
        sqlite3 * db = (sqlite3 *)0;
        sqlite3_stmt * stmt = (sqlite3_stmt *)0;
        int rc = 0;
        int ii = 0;
        FILE * debug = (FILE *)0;

        TEST();
        debug = placer_debug(stderr);
        (void)unlink(PATH);

        COMMENT("open");
        rc = sqlite3_open(PATH, &db);
        ASSERT(rc == SQLITE_OK);

        COMMENT("create");
        stmt = placer_db_prepare(db, CREATE);
        ASSERT(stmt != (sqlite3_stmt *)0);
        rc = placer_db_steps(stmt, (placer_step_t *)0, (void *)0);
        ASSERT(rc == SQLITE_OK);

        for (ii = 0; ii < countof(ROW); ++ii) {
            COMMENT("insert");
            stmt = placer_db_prepare(db, INSERT);
            ASSERT(stmt != (sqlite3_stmt *)0);
            rc = placer_struct_UnitTestSchema_bind(stmt, &ROW[ii]);
            ASSERT(rc == SQLITE_OK);
            rc = placer_db_steps(stmt, (placer_step_t *)0, (void *)0);
            ASSERT(rc == SQLITE_OK);
        }

        COMMENT("step");
        memset(row, 0, sizeof(row));
        here = &rows[0];
        stmt = placer_db_prepare(db, SELECT);
        ASSERT(stmt != (sqlite3_stmt *)0);
        rc = placer_db_steps(stmt, placer_struct_UnitTestSchema_steps_callback, &here);
        ASSERT(rc == SQLITE_OK);
        EXPECT(here == &(rows[4]));

        COMMENT("0");
        EXPECT(row[0].id == ROW[0].id);
        diminuto_dump(stderr, row[0].name, sizeof(row[0].name));
        diminuto_dump(stderr, ROW[0].name, sizeof(ROW[0].name));
        EXPECT(placer_TEXT16_compare(row[0].name,  ROW[0].name, countof(row[0].name)) == 0);
        EXPECT(row[0].age == ROW[0].age);
        EXPECT(row[0].image[0] == ROW[0].image[0]);
        EXPECT(row[0].sn == ROW[0].sn);
        EXPECT(strcmp(row[0].ssn, ROW[0].ssn) == 0);

        COMMENT("1");
        EXPECT(row[1].id == ROW[1].id);
        diminuto_dump(stderr, row[1].name, sizeof(row[1].name));
        diminuto_dump(stderr, ROW[1].name, sizeof(ROW[1].name));
        EXPECT(placer_TEXT16_compare(row[1].name,  ROW[1].name, countof(row[1].name)) == 0);
        EXPECT(row[1].age == ROW[1].age);
        EXPECT(row[1].image[0] == ROW[1].image[0]);
        EXPECT(row[1].sn == ROW[1].sn);
        EXPECT(strcmp(row[1].ssn, ROW[1].ssn) == 0);

        COMMENT("2");
        EXPECT(row[2].id == ROW[2].id);
        diminuto_dump(stderr, row[2].name, sizeof(row[2].name));
        diminuto_dump(stderr, ROW[2].name, sizeof(ROW[2].name));
        EXPECT(placer_TEXT16_compare(row[2].name,  ROW[2].name, countof(row[2].name)) == 0);
        EXPECT(row[2].age == ROW[2].age);
        EXPECT(row[2].image[0] == ROW[2].image[0]);
        EXPECT(row[2].sn == ROW[2].sn);
        EXPECT(strcmp(row[2].ssn, ROW[2].ssn) == 0);

        COMMENT("3");
        EXPECT(row[3].id == ROW[3].id);
        diminuto_dump(stderr, row[3].name, sizeof(row[3].name));
        diminuto_dump(stderr, ROW[3].name, sizeof(ROW[3].name));
        EXPECT(placer_TEXT16_compare(row[3].name,  ROW[3].name, countof(row[3].name)) == 0);
        EXPECT(row[3].age == ROW[3].age);
        EXPECT(row[3].image[0] == ROW[3].image[0]);
        EXPECT(row[3].sn == ROW[3].sn);
        EXPECT(strcmp(row[3].ssn, ROW[3].ssn) == 0);

        COMMENT("callback");
        memset(row, 0, sizeof(row));
        here = &rows[0];
        rc = placer_db_exec(db, SELECT, placer_struct_UnitTestSchema_exec_callback, &here);
        ASSERT(rc == SQLITE_OK);
        EXPECT(here == &(rows[4]));

        COMMENT("0");
        EXPECT(row[0].id == ROW[0].id);
        EXPECT(placer_TEXT16_compare(row[0].name,  ROW[0].name, countof(row[0].name)) == 0);
        EXPECT(row[0].age == ROW[0].age);
        EXPECT(row[0].image[0] == ROW[0].image[0]);
        EXPECT(row[0].sn == ROW[0].sn);
        EXPECT(strcmp(row[0].ssn, ROW[0].ssn) == 0);

        COMMENT("1");
        EXPECT(row[1].id == ROW[1].id);
        EXPECT(placer_TEXT16_compare(row[1].name,  ROW[1].name, countof(row[1].name)) == 0);
        EXPECT(row[1].age == ROW[1].age);
        EXPECT(row[1].image[0] == ROW[1].image[0]);
        EXPECT(row[1].sn == ROW[1].sn);
        EXPECT(strcmp(row[1].ssn, ROW[1].ssn) == 0);

        COMMENT("2");
        EXPECT(row[2].id == ROW[2].id);
        EXPECT(placer_TEXT16_compare(row[2].name,  ROW[2].name, countof(row[2].name)) == 0);
        EXPECT(row[2].age == ROW[2].age);
        EXPECT(row[2].image[0] == ROW[2].image[0]);
        EXPECT(row[2].sn == ROW[2].sn);
        EXPECT(strcmp(row[2].ssn, ROW[2].ssn) == 0);

        COMMENT("3");
        EXPECT(row[3].id == ROW[3].id);
        EXPECT(placer_TEXT16_compare(row[3].name,  ROW[3].name, countof(row[3].name)) == 0);
        EXPECT(row[3].age == ROW[3].age);
        EXPECT(row[3].image[0] == ROW[3].image[0]);
        EXPECT(row[3].sn == ROW[3].sn);
        EXPECT(strcmp(row[3].ssn, ROW[3].ssn) == 0);

        COMMENT("close");
        rc = sqlite3_close(db);
        ASSERT(rc == SQLITE_OK);
        placer_debug(debug);
        STATUS();
    }

    EXIT();
}
