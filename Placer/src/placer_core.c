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

int placer_exec(sqlite3 * db, const char * sql, placer_exec_callback_t * cp, void * vp)
{
    int rc = SQLITE_ERROR;
    char * sqlmessage = (char *)0;

    rc = sqlite3_exec(db, sql, cp, vp, &sqlmessage);
    if (rc != SQLITE_OK) {
        placer_message(sqlmessage);
        placer_error(rc);
    }

    return rc;
}

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

int placer_steps(sqlite3_stmt * sp, placer_steps_callback_t * cp, void * vp)
{
    int rc = SQLITE_ERROR;
    int tc = SQLITE_ERROR;
    int ii = 0;

    while (!0) {

        if (placer_Debug != (FILE *)0) {
            fprintf(placer_Debug, "%s@%d: step=%d\n", __FILE__, __LINE__, ii);
        }

        rc = sqlite3_step(sp);

        if (placer_Debug != (FILE *)0) {
            fprintf(placer_Debug, "%s@%d: step=%d rc=%d=\"%s\"\n", __FILE__, __LINE__, ii, rc, sqlite3_errstr(rc));
        }

        if (rc == SQLITE_OK) {
            rc = SQLITE_ERROR;
            placer_error(rc);
            break;
        }

        if (rc == SQLITE_DONE) {
            rc = SQLITE_OK;
            break;
        }

        if (rc != SQLITE_ROW) {
            placer_error(rc);
            break;
        }

        if (cp != (placer_steps_callback_t *)0) {
            rc = (*cp)(sp, vp);
            if (rc != SQLITE_OK) {
                placer_error(rc);
                break;
            }
        }

    } 

    tc = sqlite3_finalize(sp);
    if (tc != SQLITE_OK) {
        placer_error(tc);
        rc = tc;
    }

    return rc;
}
