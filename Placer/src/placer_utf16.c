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

placer_TEXT16_t * placer_TEXT16_copy(placer_TEXT16_t * dest, const placer_TEXT16_t * src, size_t items)
{
    placer_TEXT16_t * dp = (placer_TEXT16_t *)0;
    const placer_TEXT16_t * sp = (const placer_TEXT16_t *)0;
    size_t ii = 0;

    dp = dest;
    sp = src;
    ii = items;

    while ((ii--) > 0) {
        *(dp++) = *sp;
        if (*(sp++) == 0) {
            break;
        }
    }

    /*
     * Like strncpy(3) and wcsncpy(3), null termination is not guaranteed.
     */

    return dest;
}

int placer_TEXT16_compare(const placer_TEXT16_t * src1, const placer_TEXT16_t * src2, size_t items)
{
    int result = 0;

    while ((items--) > 0) {
        if (*src1 < *src2) {
            result = -1;
            break;
        } else if (*src1 > *src2) {
            result = 1;
            break;
        } else if (*src1 == 0) {
            break;
        } else if (*src2 == 0) {
            break; /* Redundant. */
        } else {
            src1 += 1;
            src2 += 1;
        }
    }

    return result;
}

size_t placer_TEXT16_length(const placer_TEXT16_t * src, size_t items)
{
    size_t result = 0;

    while ((items--) > 0) {
        if (*(src++) == 0) {
            break;
        }
        result += 1;
    }

    return result;
}
