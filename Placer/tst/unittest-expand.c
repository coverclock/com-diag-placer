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
        size_t result;
        TEST();
        result = placer_str_expand((char *)0, (const char *)0, 0);
        EXPECT(result == 0);
        STATUS();
    }
    {
        size_t result;
        const char from[] = "";
        TEST();
        result = placer_str_expand((char *)0, from, 0);
        EXPECT(result == 0);
        STATUS();
    }
    {
        size_t result;
        char to[1];
        TEST();
        result = placer_str_expand(to, (const char *)0, sizeof(to));
        EXPECT(result == 1);
        EXPECT(strncmp(to, "", sizeof(to)) == 0);
        STATUS();
    }
    {
        size_t result;
        const char from[] = "";
        char to[sizeof(from) * 2];
        TEST();
        result = placer_str_expand(to, from, sizeof(to));
        EXPECT(result == 1);
        EXPECT(strncmp(to, "", sizeof(to)) == 0);
        STATUS();
    }
    {
        size_t result;
        const char from[] = "'";
        char to[sizeof(from) * 2];
        TEST();
        result = placer_str_expand(to, from, sizeof(to));
        COMMENT("from=\"%s\" to=\"%s\" result=%zu\n", from, to, result);
        EXPECT(result == 3);
        EXPECT(strncmp(to, "''", sizeof(to)) == 0);
        STATUS();
    }
    {
        size_t result;
        const char from[] = "AA'BB'CC";
        char to[sizeof(from) * 2];
        TEST();
        result = placer_str_expand(to, from, sizeof(to));
        COMMENT("from=\"%s\" to=\"%s\" result=%zu\n", from, to, result);
        EXPECT(result == 11);
        EXPECT(strncmp(to, "AA''BB''CC", sizeof(to)) == 0);
        STATUS();
    }
    {
        size_t result;
        const char from[] = "'AA'BB'CC'";
        char to[sizeof(from) * 2];
        TEST();
        result = placer_str_expand(to, from, sizeof(to));
        COMMENT("from=\"%s\" to=\"%s\" result=%zu\n", from, to, result);
        EXPECT(result == 15);
        EXPECT(strncmp(to, "''AA''BB''CC''", sizeof(to)) == 0);
        STATUS();
    }
    {
        size_t result;
        const char from[] = "''AA''BB''CC''";
        char to[sizeof(from) * 2];
        TEST();
        result = placer_str_expand(to, from, sizeof(to));
        COMMENT("from=\"%s\" to=\"%s\" result=%zu\n", from, to, result);
        EXPECT(result == 23);
        EXPECT(strncmp(to, "''''AA''''BB''''CC''''", sizeof(to)) == 0);
        STATUS();
    }
    {
        const char from[] = "''AA''BB''CC''";
        char * to = (char *)0;
        TEST();
        to = placer_str_expanda(from);
        ASSERT(to != (char *)0);
        COMMENT("from=\"%s\" to=\"%s\"\n", from, to);
        EXPECT(strncmp(to, "''''AA''''BB''''CC''''", sizeof(to)) == 0);
        free(to);
        STATUS();
    }
    {
        const char from[] = "''''''''";
        char * to = (char *)0;
        TEST();
        to = placer_str_expanda(from);
        ASSERT(to != (char *)0);
        COMMENT("from=\"%s\" to=\"%s\"\n", from, to);
        EXPECT(strncmp(to, "''''''''''''''''", sizeof(to)) == 0);
        free(to);
        STATUS();
    }

    (void)placer_debug(debug);

    EXIT();
}
