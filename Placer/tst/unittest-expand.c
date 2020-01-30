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
    SETLOGMASK();

    {
        size_t result;
        result = placer_expand((char *)0, (const char *)0, 0, 0);
        EXPECT(result == 0);
    }
    {
        size_t result;
        const char from[] = "";
        result = placer_expand((char *)0, from, 0, sizeof(from));
        EXPECT(result == 0);
    }
    {
        size_t result;
        char to[1];
        result = placer_expand(to, (const char *)0, sizeof(to), 0);
        EXPECT(result == 1);
        EXPECT(strncmp(to, "", sizeof(to)) == 0);
    }
    {
        size_t result;
        const char from[] = "";
        char to[sizeof(from) * 2];
        result = placer_expand(to, from, sizeof(to), sizeof(from));
        EXPECT(result == 1);
        EXPECT(strncmp(to, "", sizeof(to)) == 0);
    }
    {
        size_t result;
        const char from[] = "'";
        char to[sizeof(from) * 2];
        result = placer_expand(to, from, sizeof(to), sizeof(from));
        COMMENT("from=\"%s\" to=\"%s\" result=%zu\n", from, to, result);
        EXPECT(result == 3);
        EXPECT(strncmp(to, "''", sizeof(to)) == 0);
    }
    {
        size_t result;
        const char from[] = "AA'BB'CC";
        char to[sizeof(from) * 2];
        result = placer_expand(to, from, sizeof(to), sizeof(from));
        COMMENT("from=\"%s\" to=\"%s\" result=%zu\n", from, to, result);
        EXPECT(result == 11);
        EXPECT(strncmp(to, "AA''BB''CC", sizeof(to)) == 0);
    }
    {
        size_t result;
        const char from[] = "'AA'BB'CC'";
        char to[sizeof(from) * 2];
        result = placer_expand(to, from, sizeof(to), sizeof(from));
        COMMENT("from=\"%s\" to=\"%s\" result=%zu\n", from, to, result);
        EXPECT(result == 15);
        EXPECT(strncmp(to, "''AA''BB''CC''", sizeof(to)) == 0);
    }
    {
        size_t result;
        const char from[] = "''AA''BB''CC''";
        char to[sizeof(from) * 2];
        result = placer_expand(to, from, sizeof(to), sizeof(from));
        COMMENT("from=\"%s\" to=\"%s\" result=%zu\n", from, to, result);
        EXPECT(result == 23);
        EXPECT(strncmp(to, "''''AA''''BB''''CC''''", sizeof(to)) == 0);
    }
    {
        const char from[] = "''AA''BB''CC''";
        char * to = (char *)0;
        to = placer_expand_alloc(from);
        ASSERT(to != (char *)0);
        COMMENT("from=\"%s\" to=\"%s\"\n", from, to);
        EXPECT(strncmp(to, "''''AA''''BB''''CC''''", sizeof(to)) == 0);
        free(to);
    }
    {
        const char from[] = "''''''''";
        char * to = (char *)0;
        to = placer_expand_alloc(from);
        ASSERT(to != (char *)0);
        COMMENT("from=\"%s\" to=\"%s\"\n", from, to);
        EXPECT(strncmp(to, "''''''''''''''''", sizeof(to)) == 0);
        free(to);
    }


    EXIT();
}
