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
 *
 * The actual test of the placer_exec() function against a database
 * is in the steps unit test, mostly just for convenience. This tests
 * the exec infrastructure.
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

#include "com/diag/placer/placer_structure_prototype.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_structure_display.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_exec_prototype.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_exec_callback.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_steps_prototype.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_steps_callback.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_stmt_prototype.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_stmt_bind.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_structure_name.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_field_name.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_field_type.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_field_sizeof.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_null.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

int main(void)
{
    SETLOGMASK();

    {
        TEST();
        EXPECT(sizeof(placer_INTEGER_t) == 4);
        EXPECT(sizeof(placer_TEXT16_t) == 2);
        EXPECT(sizeof(placer_FLOAT_t) == 8);
        EXPECT(sizeof(placer_BLOB_t) == 1);
        EXPECT(sizeof(placer_INTEGER64_t) == 8);
        EXPECT(sizeof(placer_TEXT_t) == 1);
        STATUS();
    }

    {
        TEST();
        EXPECT(countof(PLACER_struct_UnitTestSchema_FIELD_BYTES) == 6);
        EXPECT(PLACER_struct_UnitTestSchema_FIELD_BYTES[0] == 4);
        EXPECT(PLACER_struct_UnitTestSchema_FIELD_BYTES[1] == (64 * 2));
        EXPECT(PLACER_struct_UnitTestSchema_FIELD_BYTES[2] == 8);
        EXPECT(PLACER_struct_UnitTestSchema_FIELD_BYTES[3] == (256 * 1));
        EXPECT(PLACER_struct_UnitTestSchema_FIELD_BYTES[4] == 8);
        EXPECT(PLACER_struct_UnitTestSchema_FIELD_BYTES[5] == (10 * 1));
        STATUS();
    }

    {
        TEST();
        EXPECT(strcmp((const char *)PLACER_struct_UnitTestSchema_SCHEMA_NAME, "UnitTestSchema") == 0);
        EXPECT(countof(PLACER_struct_UnitTestSchema_FIELD_NAME) == 6);
        EXPECT(strcmp((const char *)PLACER_struct_UnitTestSchema_FIELD_NAME[0], "id") == 0);
        EXPECT(strcmp((const char *)PLACER_struct_UnitTestSchema_FIELD_NAME[1], "name") == 0);
        EXPECT(strcmp((const char *)PLACER_struct_UnitTestSchema_FIELD_NAME[2], "age") == 0);
        EXPECT(strcmp((const char *)PLACER_struct_UnitTestSchema_FIELD_NAME[3], "image") == 0);
        EXPECT(strcmp((const char *)PLACER_struct_UnitTestSchema_FIELD_NAME[4], "sn") == 0);
        EXPECT(strcmp((const char *)PLACER_struct_UnitTestSchema_FIELD_NAME[5], "ssn") == 0);
        STATUS();
    }

    {
        TEST();
        EXPECT(countof(PLACER_struct_UnitTestSchema_FIELD_TYPE) == 6);
        EXPECT(PLACER_struct_UnitTestSchema_FIELD_TYPE[0] == SQLITE_INTEGER);
        EXPECT(PLACER_struct_UnitTestSchema_FIELD_TYPE[1] == SQLITE_TEXT);
        EXPECT(PLACER_struct_UnitTestSchema_FIELD_TYPE[2] == SQLITE_FLOAT);
        EXPECT(PLACER_struct_UnitTestSchema_FIELD_TYPE[3] == SQLITE_BLOB);
        EXPECT(PLACER_struct_UnitTestSchema_FIELD_TYPE[4] == SQLITE_INTEGER);
        EXPECT(PLACER_struct_UnitTestSchema_FIELD_TYPE[5] == SQLITE_TEXT);
        STATUS();
    }

    {
        TEST();
        COMMENT("sizeof(struct UnitTestSchema)=%zu\n", sizeof(struct UnitTestSchema));
        EXPECT(sizeof(struct UnitTestSchema) == 424);
        STATUS();
    }

    {
        struct UnitTestSchema record =
#include "com/diag/placer/placer_structure_initializer.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

        TEST();
        COMMENT("sizeof(record)=%zu\n", sizeof(record));
        EXPECT(sizeof(record) == 424);
        EXPECT(record.id == (int32_t)0);
        EXPECT(record.name[0] == L'\0');
        EXPECT(record.age == (double)0.0);
        EXPECT(record.image[0] == (uint8_t)0);
        EXPECT(record.sn == (int64_t)0);
        EXPECT(record.ssn[0] == '\0');
        STATUS();
    }

    {
        static const char CREATE[] = "CREATE TABLE UnitTestSchema (id INTEGER PRIMARY KEY, name TEXT , age FLOAT , image BLOB , sn INTEGER , ssn TEXT  );";
#include "com/diag/placer/placer_sql_create.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

        TEST();
        COMMENT("create=\"%s\"\n", PLACER_struct_UnitTestSchema_SQL_CREATE);
        EXPECT(strcmp((const char *)PLACER_struct_UnitTestSchema_SQL_CREATE, (const char *)CREATE) == 0);
        STATUS();
    }

    {
        static const char INSERT[] = "INSERT INTO UnitTestSchema VALUES (:id, :name, :age, :image, :sn, :ssn );";
#include "com/diag/placer/placer_sql_insert.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

        TEST();
        COMMENT("insert=\"%s\"\n", PLACER_struct_UnitTestSchema_SQL_INSERT);
        EXPECT(strcmp((const char *)PLACER_struct_UnitTestSchema_SQL_INSERT, (const char *)INSERT) == 0);
        STATUS();
    }

    {
        static const char REPLACE[] = "REPLACE INTO UnitTestSchema VALUES (:id, :name, :age, :image, :sn, :ssn );";
#include "com/diag/placer/placer_sql_replace.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"

        TEST();
        COMMENT("replace=\"%s\"\n", PLACER_struct_UnitTestSchema_SQL_REPLACE);
        EXPECT(strcmp((const char *)PLACER_struct_UnitTestSchema_SQL_REPLACE, (const char *)REPLACE) == 0);
        STATUS();
    }

    {
        static const char PATH[] = "out/host/sql/unittest-schema.db";
#include "com/diag/placer/placer_sql_create.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"
#include "com/diag/placer/placer_sql_insert.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"
#include "com/diag/placer/placer_sql_replace.h"
#include "unittest-schema.h"
#include "com/diag/placer/placer_end.h"
        static const char SELECT[] = "SELECT * FROM UnitTestSchema;";
        static const struct UnitTestSchema INSERTED[4] = {
            { 1, { 'C', 'h', 'i', 'p', ' ', 'O', 'v', 'e', 'r', 'c', 'l', 'o', 'c', 'k', 0 },                               63.625, { 0x11, }, 42LL, "123456789", }, 
            { 2, { 'R', 'e', 'd', ' ', 'B', 'l', 'a', 'c', 'k', 0 },                                                        51.,    { 0x22, }, 86LL, "234567890", },
            { 3, { 'T', 'h', 'e', ' ', 'F', 'r', 'e', 'n', 'c', 'h', ' ', 'W', 'o', 'm', 'a', 'n', 0 },                     62,     { 0x33, }, 99LL, "345678901", },
            { 4, { 'D', 'r', 'e', 'a', 'd', ' ', 'P', 'i', 'r', 'a', 't', 'e', ' ', 'R', 'o', 'b', 'e', 'r', 't', 's', 0 }, 31,     { 0x44, },  1LL, "456789012", },
        };
        static const struct UnitTestSchema REPLACED[4] = {
            { 1, { 'C', 'h', 'i', 'p', ' ', 'O', 'v', 'e', 'r', 'c', 'l', 'o', 'c', 'k', 0 },                               63.625, { 0x55, }, 2LL,    "567890123", }, 
            { 2, { 'R', 'e', 'd', ' ', 'B', 'l', 'a', 'c', 'k', 0 },                                                        51.,    { 0x66, }, 4LL,  "456789012", },
            { 3, { 'T', 'h', 'e', ' ', 'F', 'r', 'e', 'n', 'c', 'h', ' ', 'W', 'o', 'm', 'a', 'n', 0 },                     62,     { 0x77, }, 6LL,  "345678901", },
            { 4, { 'D', 'r', 'e', 'a', 'd', ' ', 'P', 'i', 'r', 'a', 't', 'e', ' ', 'R', 'o', 'b', 'e', 'r', 't', 's', 0 }, 31,     { 0x88, }, 10LL, "234567890", },
        };
        struct UnitTestSchema row[4];
        struct UnitTestSchema * rows[] = { &row[0], &row[1], &row[2], &row[3], (struct UnitTestSchema *)0, };
        struct UnitTestSchema ** here = (struct UnitTestSchema **)0;
        placer_generic_callback_t generic = { stderr, 0 };
        sqlite3 * db = (sqlite3 *)0;
        sqlite3_stmt * stmt = (sqlite3_stmt *)0;
        int rc = 0;
        int ii = 0;
        FILE * debug = (FILE *)0;
        char separator = '\0';

        TEST();
        debug = placer_debug(stderr);
        rc = diminuto_fs_mkdir_p(PATH, 0755, 0);
        ASSERT(rc == 0);

        COMMENT("setup");
        ASSERT((mkdir("out", 0755) == 0) || (errno == EEXIST));
        ASSERT((mkdir("out/host", 0755) == 0) || (errno == EEXIST));
        ASSERT((mkdir("out/host/sql", 0755) == 0) || (errno == EEXIST));
        ASSERT((unlink(PATH) == 0) || (errno == ENOENT));

        COMMENT("open");
        rc = sqlite3_open(PATH, &db);
        ASSERT(rc == SQLITE_OK);

        COMMENT("create");
        stmt = placer_prepare(db, PLACER_struct_UnitTestSchema_SQL_CREATE);
        ASSERT(stmt != (sqlite3_stmt *)0);
        rc = placer_steps(stmt, (placer_steps_callback_t *)0, (void *)0);
        ASSERT(rc == SQLITE_OK);

        COMMENT("insert");
        for (ii = 0; ii < countof(INSERTED); ++ii) {
            COMMENT("%d\n", ii);
            placer_struct_UnitTestSchema_display(stderr, &INSERTED[ii]);
            stmt = placer_prepare(db, PLACER_struct_UnitTestSchema_SQL_INSERT);
            ASSERT(stmt != (sqlite3_stmt *)0);
            rc = placer_struct_UnitTestSchema_stmt_bind(stmt, &INSERTED[ii]);
            ASSERT(rc == SQLITE_OK);
            rc = placer_steps(stmt, (placer_steps_callback_t *)0, (void *)0);
            ASSERT(rc == SQLITE_OK);
        }

        COMMENT("steps");
        memset(row, 0, sizeof(row));
        here = &rows[0];
        stmt = placer_prepare(db, SELECT);
        ASSERT(stmt != (sqlite3_stmt *)0);
        rc = placer_steps(stmt, placer_struct_UnitTestSchema_steps_callback, &here);
        ASSERT(rc == SQLITE_OK);
        EXPECT(here == &(rows[4]));

        COMMENT("0");
        EXPECT(row[0].id == INSERTED[0].id);
        diminuto_dump(stderr, row[0].name, sizeof(row[0].name));
        diminuto_dump(stderr, INSERTED[0].name, sizeof(INSERTED[0].name));
        EXPECT(placer_TEXT16_compare(row[0].name,  INSERTED[0].name, countof(row[0].name)) == 0);
        EXPECT(row[0].age == INSERTED[0].age);
        EXPECT(row[0].image[0] == INSERTED[0].image[0]);
        EXPECT(row[0].sn == INSERTED[0].sn);
        EXPECT(strcmp((const char *)row[0].ssn, (const char *)INSERTED[0].ssn) == 0);

        COMMENT("1");
        EXPECT(row[1].id == INSERTED[1].id);
        diminuto_dump(stderr, row[1].name, sizeof(row[1].name));
        diminuto_dump(stderr, INSERTED[1].name, sizeof(INSERTED[1].name));
        EXPECT(placer_TEXT16_compare(row[1].name,  INSERTED[1].name, countof(row[1].name)) == 0);
        EXPECT(row[1].age == INSERTED[1].age);
        EXPECT(row[1].image[0] == INSERTED[1].image[0]);
        EXPECT(row[1].sn == INSERTED[1].sn);
        EXPECT(strcmp((const char *)row[1].ssn, (const char *)INSERTED[1].ssn) == 0);

        COMMENT("2");
        EXPECT(row[2].id == INSERTED[2].id);
        diminuto_dump(stderr, row[2].name, sizeof(row[2].name));
        diminuto_dump(stderr, INSERTED[2].name, sizeof(INSERTED[2].name));
        EXPECT(placer_TEXT16_compare(row[2].name,  INSERTED[2].name, countof(row[2].name)) == 0);
        EXPECT(row[2].age == INSERTED[2].age);
        EXPECT(row[2].image[0] == INSERTED[2].image[0]);
        EXPECT(row[2].sn == INSERTED[2].sn);
        EXPECT(strcmp((const char *)row[2].ssn, (const char *)INSERTED[2].ssn) == 0);

        COMMENT("3");
        EXPECT(row[3].id == INSERTED[3].id);
        diminuto_dump(stderr, row[3].name, sizeof(row[3].name));
        diminuto_dump(stderr, INSERTED[3].name, sizeof(INSERTED[3].name));
        EXPECT(placer_TEXT16_compare(row[3].name,  INSERTED[3].name, countof(row[3].name)) == 0);
        EXPECT(row[3].age == INSERTED[3].age);
        EXPECT(row[3].image[0] == INSERTED[3].image[0]);
        EXPECT(row[3].sn == INSERTED[3].sn);
        EXPECT(strcmp((const char *)row[3].ssn, (const char *)INSERTED[3].ssn) == 0);

        COMMENT("replace");
        for (ii = 0; ii < countof(REPLACED); ++ii) {
            COMMENT("%d\n", ii);
            placer_struct_UnitTestSchema_display(stderr, &REPLACED[ii]);
            stmt = placer_prepare(db, PLACER_struct_UnitTestSchema_SQL_REPLACE);
            ASSERT(stmt != (sqlite3_stmt *)0);
            rc = placer_struct_UnitTestSchema_stmt_bind(stmt, &REPLACED[ii]);
            ASSERT(rc == SQLITE_OK);
            rc = placer_steps(stmt, (placer_steps_callback_t *)0, (void *)0);
            ASSERT(rc == SQLITE_OK);
        }

        COMMENT("exec");
        memset(row, 0, sizeof(row));
        here = &rows[0];
        rc = placer_exec(db, SELECT, placer_struct_UnitTestSchema_exec_callback, &here);
        ASSERT(rc == SQLITE_OK);
        EXPECT(here == &(rows[4]));

        COMMENT("0");
        EXPECT(row[0].id == REPLACED[0].id);
        EXPECT(placer_TEXT16_compare(row[0].name,  REPLACED[0].name, countof(row[0].name)) == 0);
        EXPECT(row[0].age == REPLACED[0].age);
        EXPECT(row[0].image[0] == REPLACED[0].image[0]);
        EXPECT(row[0].sn == REPLACED[0].sn);
        EXPECT(strcmp((const char *)row[0].ssn, (const char *)REPLACED[0].ssn) == 0);

        COMMENT("1");
        EXPECT(row[1].id == REPLACED[1].id);
        EXPECT(placer_TEXT16_compare(row[1].name,  REPLACED[1].name, countof(row[1].name)) == 0);
        EXPECT(row[1].age == REPLACED[1].age);
        EXPECT(row[1].image[0] == REPLACED[1].image[0]);
        EXPECT(row[1].sn == REPLACED[1].sn);
        EXPECT(strcmp((const char *)row[1].ssn, (const char *)REPLACED[1].ssn) == 0);

        COMMENT("2");
        EXPECT(row[2].id == REPLACED[2].id);
        EXPECT(placer_TEXT16_compare(row[2].name,  REPLACED[2].name, countof(row[2].name)) == 0);
        EXPECT(row[2].age == REPLACED[2].age);
        EXPECT(row[2].image[0] == REPLACED[2].image[0]);
        EXPECT(row[2].sn == REPLACED[2].sn);
        EXPECT(strcmp((const char *)row[2].ssn, (const char *)REPLACED[2].ssn) == 0);

        COMMENT("3");
        EXPECT(row[3].id == REPLACED[3].id);
        EXPECT(placer_TEXT16_compare(row[3].name,  REPLACED[3].name, countof(row[3].name)) == 0);
        EXPECT(row[3].age == REPLACED[3].age);
        EXPECT(row[3].image[0] == REPLACED[3].image[0]);
        EXPECT(row[3].sn == REPLACED[3].sn);
        EXPECT(strcmp((const char *)row[3].ssn, (const char *)REPLACED[3].ssn) == 0);

        COMMENT("generic");
        separator = placer_separator(':');
        EXPECT(separator == '|');
        stmt = placer_prepare(db, SELECT);
        ASSERT(stmt != (sqlite3_stmt *)0);
        rc = placer_steps(stmt, placer_generic_steps_callback, &generic);
        ASSERT(rc == SQLITE_OK);
        ASSERT(generic.count == 4);

        COMMENT("close");
        rc = sqlite3_close(db);
        ASSERT(rc == SQLITE_OK);
        placer_debug(debug);
        STATUS();
    }

    EXIT();
}
