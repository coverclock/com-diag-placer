
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
#include "com/diag/diminuto/diminuto_types.h"
#include "com/diag/placer/placer.h"
#include "placer.h" /* Private API. */

sqlite3_stmt * placer_prepare(sqlite3 * db, const char * sql)
{
    sqlite3_stmt * sp = (sqlite3_stmt *)0;
    int rc = SQLITE_ERROR;

    rc = sqlite3_prepare_v2(db, sql, -1, &sp, (const char **)0);
    if (rc != SQLITE_OK) {
        placer_error(rc);
        if (sp != (sqlite3_stmt *)0) {
            rc = sqlite3_finalize(sp);
            if (rc != SQLITE_OK) {
                placer_error(rc);
            }
            sp = (sqlite3_stmt *)0;
        }
    }

    return sp;
}
