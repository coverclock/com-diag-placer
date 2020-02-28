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
#include "com/diag/diminuto/diminuto_phex.h"
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

int placer_steps_generic_callback(sqlite3_stmt * sp, void * vp) {
    placer_generic_callback_t * pp = (placer_generic_callback_t *)0;
    int ncols = 0;
    int ii = 0;
    int tt = 0;
    int ll = 0;
    const char * kk = (const char *)0;
    const char * vv = (const char *)0;

    if (vp != (void *)0) {

        pp = (placer_generic_callback_t *)vp;
        ++(pp->count);

        if (pp->fp != (FILE *)0) {

            ncols = sqlite3_column_count(sp);

            if (pp->count == 1) {
                for (ii = 0; ii < ncols; ++ii) {
                    if (ii > 0) {
                        fputc(placer_Separator, pp->fp);
                    }
                    kk = sqlite3_column_name(sp, ii);
                    fputs(kk, pp->fp);
                }
                fputc('\n', pp->fp);
            }

            for (ii = 0; ii < ncols; ++ii) {
                if (ii > 0) {
                    fputc(placer_Separator, pp->fp);
                }
                tt = sqlite3_column_type(sp, ii);
                if (tt != SQLITE_BLOB) {
                    vv = sqlite3_column_text(sp, ii);
                    fputs(vv, pp->fp);
                } else {
                    size_t current = 0;
                    int end = 0;
                    ll = sqlite3_column_bytes(sp, ii);
                    vv = (const char *)sqlite3_column_blob(sp, ii);
                    while ((ll--) > 0) {
                        if (*vv == placer_Separator) {
                            fputc('\\', pp->fp);
                            fputc(placer_Separator, pp->fp);
                        } else {
                            diminuto_phex_emit(pp->fp, *(vv++), ~(size_t)0, 0, 0, 0, &current, &end, 0);
                        }
                    }
                }
            }
            fputc('\n', pp->fp);

            fflush(pp->fp);

        }

    }

    return SQLITE_OK;
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
