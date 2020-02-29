/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
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

int main(void)
{
    SETLOGMASK();

    {
        static const placer_TEXT16_t DATA1[] = { 0 };
        static const placer_TEXT16_t DATA2[] = { 'A', 0 };
        static const placer_TEXT16_t DATA3[] = { 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 0 };
        static const placer_TEXT16_t DATA4[] = { 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 0 };
        static const placer_TEXT16_t DATA5[] = { 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'T' };

        TEST();
        EXPECT(placer_TEXT16_length(DATA1, ~(size_t)0) == 0);
        EXPECT(placer_TEXT16_length(DATA1, countof(DATA1)) == 0);
        EXPECT(placer_TEXT16_length(DATA2, countof(DATA2)) == 1);
        EXPECT(placer_TEXT16_length(DATA2, ~(size_t)0) == 1);
        EXPECT(placer_TEXT16_length(DATA3, countof(DATA3)) == 8);
        EXPECT(placer_TEXT16_length(DATA3, ~(size_t)0) == 8);
        EXPECT(placer_TEXT16_length(DATA4, countof(DATA4)) == 9);
        EXPECT(placer_TEXT16_length(DATA4, ~(size_t)0) == 9);
        EXPECT(placer_TEXT16_length(DATA5, countof(DATA5)) == 10);
        STATUS();
    }

    {
        static const placer_TEXT16_t DATA1[] = { 0 };
        static const placer_TEXT16_t DATA2[] = { 'A', 0 };
        static const placer_TEXT16_t DATA3[] = { 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 0 };
        static const placer_TEXT16_t DATA4[] = { 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 0 };
        static const placer_TEXT16_t DATA5[] = { 'B', 0 };

        TEST();

        COMMENT("1");
        EXPECT(placer_TEXT16_compare(DATA1, DATA1, countof(DATA1)) == 0);
        EXPECT(placer_TEXT16_compare(DATA1, DATA2, countof(DATA1)) == -1);
        EXPECT(placer_TEXT16_compare(DATA1, DATA3, countof(DATA1)) == -1);
        EXPECT(placer_TEXT16_compare(DATA1, DATA4, countof(DATA1)) == -1);
        EXPECT(placer_TEXT16_compare(DATA1, DATA5, countof(DATA1)) == -1);
        EXPECT(placer_TEXT16_compare(DATA1, DATA5, 1) == -1);
        EXPECT(placer_TEXT16_compare(DATA1, DATA5, 0) == 0);

        COMMENT("2");
        EXPECT(placer_TEXT16_compare(DATA2, DATA1, countof(DATA2)) == 1);
        EXPECT(placer_TEXT16_compare(DATA2, DATA2, countof(DATA2)) == 0);
        EXPECT(placer_TEXT16_compare(DATA2, DATA3, countof(DATA2)) == -1);
        EXPECT(placer_TEXT16_compare(DATA2, DATA4, countof(DATA2)) == -1);
        EXPECT(placer_TEXT16_compare(DATA2, DATA5, countof(DATA2)) == -1);
        EXPECT(placer_TEXT16_compare(DATA2, DATA5, 1) == -1);
        EXPECT(placer_TEXT16_compare(DATA2, DATA5, 0) == 0);

        COMMENT("3");
        EXPECT(placer_TEXT16_compare(DATA3, DATA1, countof(DATA3)) == 1);
        EXPECT(placer_TEXT16_compare(DATA3, DATA2, countof(DATA3)) == 1);
        EXPECT(placer_TEXT16_compare(DATA3, DATA3, countof(DATA3)) == 0);
        EXPECT(placer_TEXT16_compare(DATA3, DATA4, countof(DATA3)) == -1);
        EXPECT(placer_TEXT16_compare(DATA3, DATA5, countof(DATA3)) == 1);
        EXPECT(placer_TEXT16_compare(DATA3, DATA5, 1) == 0);
        EXPECT(placer_TEXT16_compare(DATA3, DATA5, 0) == 0);

        COMMENT("4");
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
        placer_TEXT16_t result[] = { '0', '1', '2', '3', '4', '5', '6', '7', 0, };

        TEST();

        COMMENT("1");
        EXPECT(placer_TEXT16_copy(result, DATA1, countof(result)) == result);
        EXPECT(placer_TEXT16_compare(result, DATA1, countof(result)) == 0);
        EXPECT(result[countof(result) - 1] == 0);

        COMMENT("2");
        EXPECT(placer_TEXT16_copy(result, DATA2, countof(result)) == result);
        EXPECT(placer_TEXT16_compare(result, DATA2, countof(result)) == 0);
        EXPECT(result[countof(result) - 1] == 0);

        COMMENT("3");
        EXPECT(placer_TEXT16_copy(result, DATA3, countof(result)) == result);
        EXPECT(placer_TEXT16_compare(result, DATA3, countof(result)) == 0);
        EXPECT(result[countof(result) - 1] == 0);

        COMMENT("4");
        EXPECT(placer_TEXT16_copy(result, DATA4, countof(result)) == result);
        EXPECT(placer_TEXT16_compare(result, DATA4, countof(result)) == 0);
        EXPECT(result[countof(result) - 1] == 'R');

        STATUS();
    }

    EXIT();
}
