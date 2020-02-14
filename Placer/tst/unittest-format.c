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

int main(void)
{
    FILE * debug;

    SETLOGMASK();

    debug = placer_debug(stderr);

    {
        char * pp = (char *)0;
        TEST();
        pp = placer_sql_formata(0, "SELECT * FROM %s;", "table");
        EXPECT(pp == (char *)0);
        STATUS();
    }
    {
        char * pp = (char *)0;
        TEST();
        pp = placer_sql_formata(1, "SELECT * FROM %s;", "table");
        ASSERT(pp != (char *)0);
        EXPECT(strcmp("SELECT * FROM table;", pp) == 0);;
        sqlite3_free(pp);
        STATUS();
    }
    {
        char * pp = (char *)0;
        TEST();
        pp = placer_sql_formata(2, "SELECT * FROM %s;", "table");
        ASSERT(pp != (char *)0);
        EXPECT(strcmp("SELECT * FROM table;", pp) == 0);;
        sqlite3_free(pp);
        STATUS();
    }
    {
        char * pp = (char *)0;
        TEST();
        pp = placer_sql_formata(512, "SELECT * FROM %s;", "table");
        ASSERT(pp != (char *)0);
        EXPECT(strcmp("SELECT * FROM table;", pp) == 0);;
        sqlite3_free(pp);
        STATUS();
    }

    (void)placer_debug(debug);

    EXIT();
}

