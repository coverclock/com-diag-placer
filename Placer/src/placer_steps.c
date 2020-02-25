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

int placer_steps_generic_callback(sqlite3_stmt * sp, void * vp) {
    static const char SEPARATOR = '|';
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
                        fputc(SEPARATOR, pp->fp);
                    }
                    kk = sqlite3_column_name(sp, ii);
                    fputs(kk, pp->fp);
                }
                fputc('\n', pp->fp);
            }

            for (ii = 0; ii < ncols; ++ii) {
                if (ii > 0) {
                    fputc(SEPARATOR, pp->fp);
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
                        if (*vv == SEPARATOR) {
                            fputc('\\', pp->fp);
                            fputc(SEPARATOR, pp->fp);
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
