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
 * This defines the public API for Placer.
 */

#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include "sqlite3.h"

/*******************************************************************************
 * TYPES
 ******************************************************************************/

/**
 * This is the type that Placer uses for SQLite3 BLOB data.
 */
typedef uint8_t placer_BLOB_t;

/**
 * This is the type that Placer uses for SQLite3 FLOAT data.
 */
typedef double placer_FLOAT_t;

/**
 * This is the type that Placer uses for SQLite3 INTEGER data.
 */
typedef int32_t placer_INTEGER_t;

/**
 * This is the type that Placer uses for SQLite3 INTEGER64 data.
 */
typedef int64_t placer_INTEGER64_t;

/**
 * This is the type that Placer uses for SQLite3 TEXT (UTF-8) data.
 */
typedef unsigned char placer_TEXT_t;

/**
 * This is the type that Placer uses for SQLite3 TEXT16 (UTF-16) data.
 */
typedef unsigned short placer_TEXT16_t;

/*******************************************************************************
 * STRUCTURES
 ******************************************************************************/

/**
 * Defines a structure used by generic callbacks to (if not null)
 * emit a message for each call and (if initially zeroed) count the
 * number of entries. The file pointer should be initialized by the
 * application to a standard I/O stream (e.g. stdout), and the count
 * to zero.
 */
typedef struct PlacerGenericCallback {
    FILE * fp;
    size_t count;
} placer_generic_callback_t;

/**
 * @def PLACER_GENERIC_CALLBACK_INITIALIZER
 * Generates code for a completely optional convenience
 * initializer for a generic callback structure. Mostly
 * this just serves as an example.
 */
#define PLACER_GENERIC_CALLBACK_INITIALIZER \
    { stdout, 0 }

/*******************************************************************************
 * CALLBACKS
 ******************************************************************************/

/**
 * THis is the prototype for the callback function for the Placer exec feature.
 */
typedef int (placer_exec_callback_t)(void *, int, char **, char **);

/**
 * THis is the prototype for the free function for the Placer bind feature.
 */
typedef void (placer_bind_callback_t)(void *);

/**
 * THis is the prototype for the callback function for the Placer steps feature.
 */
typedef int (placer_steps_callback_t)(sqlite3_stmt *, void *);

/*******************************************************************************
 * STRINGS
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
 * STRUCTURE QUERY LANGUAGE
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
 * EXEC
 ******************************************************************************/

/**
 * Implement a generic SQLite callback useful for debugging.
 * @param vp is a pointer to a generic callback structure or NULL.
 * @param ncols is the number of columns provided by SQLite.
 * @param value is the array of column values provided by SQLite.
 * @param keyword is the array of column names provided by SQLite.
 * @return always SQLITE_OK (0).
 */
extern int placer_exec_generic_callback(void * vp, int ncols, char ** value, char ** keyword);

/*******************************************************************************
 * STEPS
 ******************************************************************************/

typedef int (placer_steps_callback_t)(sqlite3_stmt *, void *);

/*******************************************************************************
 * CORE
 ******************************************************************************/

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
extern void placer_message(char * message);

/**
 * Turn the SQLite error number into a printable string and emit it
 * to standard error.
 * @param error is the SQLite error number.
 */
extern void placer_error(int error);

/**
 * A convenience function that calls sqlite3_exec(), emits any error messages to standard
 * error, and returns the SQLite3 return code. The sql statement is NOT freed.
 * @param db points to the database object.
 * @param sql points to the sql statement.
 * @param cp points to the callback function or NULL.
 * @param vp points to the callback state object or NULL.
 * @return the SQLite3 return code.
 */
extern int placer_exec(sqlite3 * db, const char * sql, placer_exec_callback_t * cp, void * vp);

/**
 * A convenience function that calls sqlite3_prepare() to compile an SQL string
 * into a SQLite3 statement object. Parameters from a schema can be bound to the
 * resulting object.
 * @param db points to the database object.
 * @param sql points to the SQL string.
 * @return a pointer to SQLite3 statement object.
 */
extern sqlite3_stmt * placer_prepare(sqlite3 * db, const char * sql);

/**
 * Incrementally runs the SQLite3 step function with the provided statement
 * object, and applies the callback function with the void pointer to every row.
 * The callback function generated by the steps schema macros will import each row
 * into a schema structure. You can use the generated callback function directly,
 * or call it from your own callback function. The statement object is automatically
 * finalized once the last row is processed.
 * @param sp points to the SQLite3 statement object
 * @param cp points to the callback function or NULL.
 * @param vp points to the callback state object or NULL.
 * @return an SQLite3 return code.
 */
extern int placer_steps(sqlite3_stmt * sp, placer_steps_callback_t * cp, void * vp);

/*******************************************************************************
 * UTF-16
 ******************************************************************************/

/**
 * Copy no more than the specified number of UTF-16 characters from one TEXT16
 * string to antother up to and including the NUL UTF-16 character. If the
 * destination string isn't large enough, it is NOT guaranenteed to be NUL
 * terminated.
 * @param dest points to the destination string.
 * @param src points to the source string.
 * @param items is the maximum number of UTF-16 characters (NOT BYTES) to copy.
 * @return the pointer to the destination string.
 */
extern placer_TEXT16_t * placer_TEXT16_copy(placer_TEXT16_t * dest, const placer_TEXT16_t * src, size_t items);

/**
 * Compare no more than the specified number of UTF-16 characters from two
 * TEXT16 strings.
 * @param src1 points to the first string.
 * @param src2 points to the second string.
 * @param items is the maximum number of UTF-16 characters (NOT BYTES) to compare.
 * @return -1, 0, or 1 for less than, equal to, or greater than.
 */
extern int placer_TEXT16_compare(const placer_TEXT16_t * src1, const placer_TEXT16_t * src2, size_t items);

/**
 * Return the number of UTF-16 characters in a TEXT16 string up to the specified
 * number of characters, and not including any terminating NUL.
 * @param src points to the string.
 * @param items is the maximum number of UTF-16 characters (NOT BYTES) to count.
 * @return the length of the string in UTF-16 characters (NOT BYTES).
 */
extern size_t placer_TEXT16_length(const placer_TEXT16_t * src, size_t items);

#endif
