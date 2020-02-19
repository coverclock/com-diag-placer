/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "sqlite3.h"
#include "com/diag/diminuto/diminuto_types.h"
#include "placer.h" /* Private API. */

FILE * placer_Debug = (FILE *)0;

FILE * placer_debug(FILE * now)
{
    FILE * was;

    was = placer_Debug;
    placer_Debug = now;

    return was;
}

void placer_message(char * message)
{
    if (message != (char *)0) {
        fputs("SQLite: \"", stderr);
        fputs(message, stderr);
        fputs("\"\n", stderr);
        sqlite3_free(message);
    }
}

void placer_error(int error)
{
    if (error != SQLITE_OK) {
        fprintf(stderr, "SQLite: [%d] \"", error);
        fputs(sqlite3_errstr(error), stderr);
        fputs("\"\n", stderr);
    }
}
