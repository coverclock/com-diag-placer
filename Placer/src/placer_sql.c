/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include "com/diag/diminuto/diminuto_types.h"
#include "com/diag/placer/placer_sql.h"

int placer_sql_callback_generic(void * vfp, int ncols, char ** value, char ** keyword)
{
    FILE * fp = (FILE *)0;
    int ii = 0;

    fp = (FILE *)vfp;

    for (ii = 0; ii < ncols; ++ii) {
        fprintf(fp, "%s[%d]=\"%s\"[%zu] ", keyword[ii], ii, value[ii], strlen(value[ii]));
    }
    fputc('\n', fp);

    return SQLITE_OK;
}

size_t placer_sql_expand(char * to, const char * from, size_t tsize, size_t fsize) {
    size_t size = 0;

    while ((fsize > 0) && (tsize > 1) && (*from != '\0')) {
        *(to++) = *from;
        if (*from != '\'') {
            /* Do nothing. */
        } else if (tsize > 2) {
            *(to++) = '\'';
            --tsize;
            ++size;
        } else {
            break;
        }
        ++from;
        --fsize;
        --tsize;
        ++size;
    }
    if (tsize > 0) {
        *to = '\0';
        ++size;
    }

    return size;
}

char * placer_sql_expand_alloc(const char * from)
{
    char * to = (char *)0;
    size_t fsize = 0;
    size_t tsize = 0;

    fsize = strlen(from);
    tsize = (fsize * 2) + 1;
    to = malloc(tsize);
    if (to != (char *)0) {
        placer_sql_expand(to, from, tsize, fsize);
    }

    return to;
}
