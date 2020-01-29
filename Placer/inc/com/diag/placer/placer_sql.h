/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_PLACER_SQL_
#define _H_COM_DIAG_PLACER_SQL_

/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 */

#include "sqlite3.h"

/**
 * Emit the SQLite error message to standard error if it is
 * non-NULL.
 * @param message is the SQLite error message or nULL.
 */
static inline void placer_sql_message(const char * message)
{
    if (message != (const char *)0) {
        fputs("SQLite3: ", stderr);
        fputs(message, stderr);
        fputc('\n', stderr);
    }
}

/**
 * Turn the SQLite error number into a printable string and emit it
 * to standard error.
 * @param error is the SQLite error number.
 */
static inline void placer_sql_error(int error)
{
    if (error != SQLITE_OK) {
        placer_sql_message(sqlite3_errstr(error));
    }
}

/**
 * Implement a generic SQLite callback useful for debugging.
 * @param vfp is the FILE * to be used to emit debugging information.
 * @param ncols is the number of columns provided by SQLite.
 * @param value is the array of column values provided by SQLite.
 * @param keyword is the array of column names provided by SQLite.
 * @return always SQLITE_OK (0).
 */
extern int placer_sql_callback_generic(void * vfp, int ncols, char ** value, char ** keyword);

/**
 * Copy characters from one buffer to another changing each single quote into
 * two single quotes. The destination buffer is guaranteeed to be NUL terminated
 * providing it is at least one byte in length.
 * @param to points to the destination buffer.
 * @param from points to the source buffer.
 * @param tsize is the size of the destination buffer.
 * @param fsize is the size of the source buffer.
 * @return the number of bytes in the destination including the terminating NUL.
 */
extern size_t placer_sql_expand(char * to, const char * from, size_t tsize, size_t fsize);

#endif
