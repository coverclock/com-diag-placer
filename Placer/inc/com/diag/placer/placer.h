/* vi: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_PLACER_
#define _H_COM_DIAG_PLACER_

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
 * Sets the debug file pointer. If the pointer is non-NULL, debugging
 * information is emitted to it. The prior debug file pointer is returned.
 * @param now is the new file pointer used for debugging, or NULL.
 * @return the prior debug file pointer (which may be NULL).
 */
extern FILE * placer_debug(FILE * now);

/**
 * Emit the SQLite error message to standard error if it is
 * non-NULL and then free it.
 * @param message is the SQLite error message or nULL.
 */
static inline void placer_message(char * message)
{
    if (message != (char *)0) {
        fputs("SQLite3: ", stderr);
        fputs(message, stderr);
        fputc('\n', stderr);
        sqlite3_free(message);
    }
}

/**
 * Turn the SQLite error number into a printable string and emit it
 * to standard error.
 * @param error is the SQLite error number.
 */
static inline void placer_error(int error)
{
    if (error != SQLITE_OK) {
        fputs("SQLite3: ", stderr);
        fputs(sqlite3_errstr(error), stderr);
        fputc('\n', stderr);
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
extern int placer_callback_generic(void * vfp, int ncols, char ** value, char ** keyword);

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
extern size_t placer_expand(char * to, const char * from, size_t tsize, size_t fsize);

/**
 * Copy characters from one buffer to a dynamically allocated buffer changing
 * each single quote into two single quotes. The dynamically allocated buffer
 * is guaranteed to be NUL terminated. The dynamically allocated buffer must
 * be freed using free(3).
 * @param  from points to the source buffer.
 * @return the dynamically acquired buffer or NULL if failure.
 */
extern char * placer_expand_alloc(const char * from);

/**
 * Format arguments in an snprintf(3) manner into a dynamically acquired
 * buffer, enarging the buffer to guarantee that the results fit. Initially
 * allocate the buffer to the specified size. The dynamically allocated
 * buffer must be freed using free(3).
 * @param size is the initial allocation size of the buffer in bytes.
 * @param format is the snprintf(3) format string.
 * @return the dynamically acquired buffer or NULL if failure.
 */
extern char * placer_format_alloc(size_t size, const char * format, ...);

#endif
