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
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include "sqlite3.h"
#include "com/diag/diminuto/diminuto_log.h"
#include "com/diag/placer/placer.h"
#include "placer.h" /* Private API. */

FILE * placer_Debug = (FILE *)0;

char placer_Separator = '|';

FILE * placer_debug(FILE * now)
{
    FILE * was;

    was = placer_Debug;
    placer_Debug = now;

    return was;
}

char placer_separator(char now)
{
    char was;

    was = placer_Separator;
    placer_Separator = now;

    return was;
}

void placer_message_f(const char * file, int line, char * message)
{
    if (message != (char *)0) {
        diminuto_log_log(DIMINUTO_LOG_PRIORITY_ERROR, "%s@%d: SQLite: \"%s\"\n", file, line, message);
        sqlite3_free(message);
    }
}

void placer_error_f(const char * file, int line, int error)
{
    if (error != SQLITE_OK) {
        diminuto_log_log(DIMINUTO_LOG_PRIORITY_ERROR, "%s@%d[%d]: SQLite: \"%s\"\n", file, line, error, sqlite3_errstr(error));
    }
}
