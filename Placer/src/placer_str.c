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
#include "placer.h"
#include "com/diag/placer/placer.h"

size_t placer_str_expand(char * to, const char * from, size_t size) {
    size_t length = 0;

    if (placer_Debug != (FILE *)0) {
        fprintf(placer_Debug, "%s@%d: size=%zu\n", __FILE__, __LINE__, size);
    }

    while ((size > 1) && (*from != '\0')) {
        if (*from != '\'') {
            *(to++) = *(from++);
            size -= 1;
            length += 1;
        } else if (size > 2) {
            *(to++) = *from;
            *(to++) = *(from++);
            size -= 2;
            length += 2;
        } else {
            break;
        }
    }

    if (size > 0) {
        *to = '\0';
        length += 1;
    }

    if (placer_Debug != (FILE *)0) {
        fprintf(placer_Debug, "%s@%d: length=%zu\n", __FILE__, __LINE__, length);
    }

    return length;
}

char * placer_str_expanda(const char * from)
{
    char * to = (char *)0;
    const char * ff = (const char *)0;
    size_t count = 0;
    size_t size = 0;

    for (ff = from; *ff != '\0'; ++ff) {
        if (*ff == '\'') {
            ++count;
        }
    }

    if (placer_Debug != (FILE *)0) {
        fprintf(placer_Debug, "%s@%d: count=%zu\n", __FILE__, __LINE__, count);
    }

    size = strlen(from) + count + 1;
    to = (char *)sqlite3_malloc(size);
    if (to == (char *)0) {
        perror("sqlite3_malloc");
    } else  {
        placer_str_expand(to, from, size);
    }

    return to;
}
