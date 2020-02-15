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

#include <stdint.h>
#include <stdio.h>
#include <wchar.h>
#include <sys/types.h>
#include "sqlite3.h"

/*******************************************************************************
 *
 ******************************************************************************/

/**
 * Sets the debug file pointer. If the pointer is non-NULL, debugging
 * information is emitted to it. The prior debug file pointer is returned.
 * @param now is the new file pointer used for debugging, or NULL.
 * @return the prior debug file pointer (which may be NULL).
 */
extern FILE * placer_debug(FILE * now);

/*******************************************************************************
 *
 ******************************************************************************/

typedef uint8_t placer_BLOB_t;

typedef double placer_FLOAT_t;

typedef int32_t placer_INTEGER_t;

typedef int64_t placer_INTEGER64_t;

typedef unsigned char placer_TEXT_t;

#if !0
typedef wchar_t placer_TEXT16_t;
#else
typedef uint16_t placer_TEXT16_t;
#endif

/*******************************************************************************
 *
 ******************************************************************************/

typedef int (placer_callback_t)(void *, int, char **, char **);

typedef void (placer_free_t)(void *);

typedef int (placer_step_t)(sqlite3_stmt *, void *);

/*******************************************************************************
 *
 ******************************************************************************/

/**
 * Emit the SQLite error message to standard error if it is
 * non-NULL and then free it.
 * @param message is the SQLite error message or nULL.
 */
extern void placer_message(char * message);

/**
 * Turn the SQLite error number into a printable string and emit it
 * to standard error.
 * @param error is the SQLite error number.
 */
extern void placer_error(int error);

/*******************************************************************************
 *
 ******************************************************************************/

/**
 * Defines a structure used by the generic callback to (if not null)
 * emit a message for each call and (if initially zeroed) count the
 * number of entries.
 */
typedef struct PlacerCallbackGeneric {
    FILE * fp;
    size_t count;
} placer_callback_generic_t;

/**
 * Implement a generic SQLite callback useful for debugging.
 * @param vp is a pointer to a generic callback structure or NULL.
 * @param ncols is the number of columns provided by SQLite.
 * @param value is the array of column values provided by SQLite.
 * @param keyword is the array of column names provided by SQLite.
 * @return always SQLITE_OK (0).
 */
extern int placer_callback_generic(void * vp, int ncols, char ** value, char ** keyword);

/*******************************************************************************
 *
 ******************************************************************************/

/**
 * Copy characters from one buffer to another changing each single quote into
 * two single quotes. The destination buffer is guaranteeed to be NUL terminated
 * providing it is at least one byte in length.
 * @param to points to the destination buffer.
 * @param from points to the source buffer.
 * @param size is the size of the destination buffer in bytes.
 * @return the number of bytes in the destination including the terminating NUL.
 */
extern size_t placer_str_expand(char * to, const char * from, size_t size);

/**
 * Copy characters from one buffer to a dynamically allocated buffer changing
 * each single quote into two single quotes. The dynamically allocated buffer
 * is guaranteed to be NUL terminated. The dynamically allocated buffer must
 * be freed using sqlite3_free().
 * @param  from points to the source buffer.
 * @return the dynamically acquired buffer or NULL if failure.
 */
extern char * placer_str_expanda(const char * from);

/*******************************************************************************
 *
 ******************************************************************************/

/**
 * Format arguments in an vsnprintf(3) manner into a dynamically acquired
 * buffer, enarging the buffer to guarantee that the results fit. Initially
 * allocate the buffer to the specified size. The dynamically allocated
 * buffer must be freed using sqlite3_free().
 * @param size is the initial allocation size of the buffer in bytes.
 * @param format is the snprintf(3) format string.
 * @param op is the variable length argument list.
 * @return the dynamically acquired buffer or NULL if failure.
 */
extern char * placer_sql_vformata(size_t size, const char * format, va_list op);

/**
 * Format arguments in an snprintf(3) manner into a dynamically acquired
 * buffer, enarging the buffer to guarantee that the results fit. Initially
 * allocate the buffer to the specified size. The dynamically allocated
 * buffer must be freed using sqlite3_free().
 * @param size is the initial allocation size of the buffer in bytes.
 * @param format is the snprintf(3) format string.
 * @return the dynamically acquired buffer or NULL if failure.
 */
extern char * placer_sql_formata(size_t size, const char * format, ...);

/*******************************************************************************
 *
 ******************************************************************************/

/**
 * A convenience function that calls sqlite3_exec(), emits any error messages to standard
 * error, and returns the SQLite3 return code. The sql statement is NOT freed.
 * @param db points to the database object.
 * @param sql points to the sql statement.
 * @param cp points to the callback function or NULL.
 * @param vp points to the callback state object or NULL.
 * @return the SQLite3 return code.
 */
extern int placer_db_exec(sqlite3 * db, const char * sql, int (*cp)(void *, int, char **, char **), void * vp);

extern sqlite3_stmt * placer_db_prepare(sqlite3 * db, const char * sql);

extern int placer_db_steps(sqlite3_stmt * sp, placer_step_t * cp, void * vp);

/*******************************************************************************
 *
 ******************************************************************************/

extern int placer_BLOB_import(placer_BLOB_t * dest, size_t items, const char * src);

extern int placer_FLOAT_import(placer_FLOAT_t * dest, const char * src);

extern int placer_INTEGER_import(placer_INTEGER_t * dest, const char * src);

extern int placer_INTEGER64_import(placer_INTEGER64_t * dest, const char * src);

extern int placer_TEXT_import(placer_TEXT_t * dest, size_t items, const char * src);

extern int placer_TEXT16_import(placer_TEXT16_t * dest, size_t items, const char * src);

/*******************************************************************************
 *
 ******************************************************************************/

#endif
