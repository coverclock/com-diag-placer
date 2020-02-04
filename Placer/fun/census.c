/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 *
 * ABSTRACT
 *
 * Walks the file system tree starting at the indicate root or roots,
 * and inserts the resulting paths plus attributes into the specified
 * database, and then manipulatges the result. This is just an excuse
 * for me to re-learn how to use SQLite after having been away from it
 * for a few years.
 *
 * USAGE
 *
 * census [ -? ] [ -d ] [ -v ] -D DATABASE [ -1 ] [ -2 ] [ -3 ]  ROOT [ ROOT ... ]
 *
 * EXAMPLES
 *
 * census -D out/host/sql/census.db /
 *
 * REFERENCES
 *
 * https://github.com/coverclock/com-diag-diminuto/Diminuto/bin/walker.c
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <errno.h>
#include <assert.h>
#include "com/diag/diminuto/diminuto_fs.h"
#include "com/diag/placer/placer.h"

static const char * Program = (const char *)0;
static int Debug = 0;
static int Verbose = 0;
static size_t Buffersize = 256;

static int mark(sqlite3 * db)
{
    int xc = 0;
    int rc = 0;
    char * sql = (char *)0;
    char * sqlmessage = (char *)0;
   
    do {

        /*
         *
         */

        sql = placer_format_alloc(Buffersize,
            "UPDATE census SET mark = 1 WHERE nlink > 1;"
        );

        if (sql == (char *)0) {
            xc = -12;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        sqlmessage = (char *)0;
        rc = sqlite3_exec(db, sql, placer_callback_generic, stderr, &sqlmessage);
        free(sql);

        if (rc != SQLITE_OK) {
            placer_message(sqlmessage);
            placer_error(rc);
            xc = -13;
            break;
        }

        /*
         *
         */

        sql = placer_format_alloc(Buffersize,
            "SELECT * FROM census WHERE mark != 0;"
        );

        if (sql == (char *)0) {
            xc = -12;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        sqlmessage = (char *)0;
        rc = sqlite3_exec(db, sql, placer_callback_generic, stderr, &sqlmessage);
        free(sql);

        if (rc != SQLITE_OK) {
            placer_message(sqlmessage);
            placer_error(rc);
            xc = -13;
            break;
        }

        /*
         *
         */

        sql = placer_format_alloc(Buffersize,
            "SELECT * FROM census WHERE (mark != 0) AND (nlink <= 1);"
        );

        if (sql == (char *)0) {
            xc = -12;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        sqlmessage = (char *)0;
        rc = sqlite3_exec(db, sql, placer_callback_generic, stderr, &sqlmessage);
        free(sql);

        if (rc != SQLITE_OK) {
            placer_message(sqlmessage);
            placer_error(rc);
            xc = -13;
            break;
        }

        /*
         *
         */

        sql = placer_format_alloc(Buffersize,
            "SELECT * FROM census WHERE (mark == 0) AND (nlink > 1);"
        );

        if (sql == (char *)0) {
            xc = -12;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        sqlmessage = (char *)0;
        rc = sqlite3_exec(db, sql, placer_callback_generic, stderr, &sqlmessage);
        free(sql);

        if (rc != SQLITE_OK) {
            placer_message(sqlmessage);
            placer_error(rc);
            xc = -13;
            break;
        }

        /*
         *
         */

        sql = placer_format_alloc(Buffersize,
            "UPDATE census SET mark = 0 WHERE mark != 0;"
        );

        if (sql == (char *)0) {
            xc = -12;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        sqlmessage = (char *)0;
        rc = sqlite3_exec(db, sql, placer_callback_generic, stderr, &sqlmessage);
        free(sql);

        if (rc != SQLITE_OK) {
            placer_message(sqlmessage);
            placer_error(rc);
            xc = -13;
            break;
        }

    } while (0);

    return xc;
}

/*
 * For every file in the file system, see if there is a file in the DB
 * with the same inode number but a different path name. This could be
 * a hard link, in which case the number of links field should be greater
 * than one. But it could also be that the file system has changed since
 * the census, and the inode number reused. We make that less likely by
 * only selecting those DB entries whose link count is greater than one.
 */

static int identifier(void * vp, int ncols, char ** value, char ** keyword)
{
    const char * pp = (char *)0;
    int ii = 0;

    pp = (const char *)vp;

    for (ii = 0; ii < ncols; ++ii) {
        if ((strcmp(keyword[ii], "path") == 0)  && (strcmp(value[ii], pp) != 0)) {
            fputs("Aliased: \"", stderr);
            fputs(value[ii], stderr);
            fputs("\" \"", stderr);
            fputs(pp, stderr);
            fputs("\"\n", stderr);
        }
    }

    return SQLITE_OK;
}

static int identify(void * vp, const char * name, const char * path, size_t depth, const struct stat * statp)
{
    int xc = 0;
    int rc = 0;
    sqlite3 * db = (sqlite3 *)0;
    char * sql = (char *)0;
    char * sqlmessage = (char *)0;
   
    do {

        if (statp->st_nlink <= 1) {
            break;
        }

        db = (sqlite3 *)vp;

        /*
         * Select the row from the database.
         */

        sql = placer_format_alloc(Buffersize,
            "SELECT * FROM census WHERE (ino = %d) AND (devmajor = %d) AND (devminor = %d) AND (nlink > 1);"
            , statp->st_ino
            , major(statp->st_dev)
            , minor(statp->st_dev)
        );

        if (sql == (char *)0) {
            xc = -12;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        sqlmessage = (char *)0;
        rc = sqlite3_exec(db, sql, identifier, (char *)path, &sqlmessage);
        free(sql);

        if (rc != SQLITE_OK) {
            placer_message(sqlmessage);
            placer_error(rc);
            xc = -13;
            break;
        }

    } while (0);

    return xc;
}

/*
 * For every file in the file system, enumerate the number of entries
 * for the same path in the DB. Something is amiss if that number if
 * greater than one. But it could also be zero, if the file has been
 * deleted since the census was done.
 */

static int enumerator(void * vp, int ncols, char ** value, char ** keyword)
{
    int * ip = (int *)0;

    ip = (int *)vp;
    (*ip) += 1;

    return SQLITE_OK;
}

static int enumerate(void * vp, const char * name, const char * path, size_t depth, const struct stat * statp)
{
    int xc = 0;
    sqlite3 * db = (sqlite3 *)0; 
    int rc = 0;
    char * to = (char *)0;
    char * sql = (char *)0;
    char * sqlmessage = (char *)0;
    int enumeration = 0;
   
    do {

        db = (sqlite3 *)vp;

        to = placer_expand_alloc(path);

        /*
         * Select the row from the database.
         */

        sql = placer_format_alloc(Buffersize,
            "SELECT * FROM census WHERE (path = '%s');"
            , to
        );
        free(to);

        if (sql == (char *)0) {
            xc = -12;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        enumeration = 0;

        sqlmessage = (char *)0;
        rc = sqlite3_exec(db, sql, enumerator, &enumeration, &sqlmessage);
        free(sql);

        if (rc != SQLITE_OK) {
            placer_message(sqlmessage);
            placer_error(rc);
            xc = -13;
            break;
        }

        if (enumeration < 1) {
            fputs("NONE: ", stderr);
            fputs(path, stderr);
            fputc('\n', stderr);
        } else if (enumeration > 1) {
            fputs("MANY: ", stderr);
            fputs(path, stderr);
            fputc('\n', stderr);
        } else {
            /* Do nothing. */
        }

    } while (0);

    return xc;
}

/*
 * Find all files in the census which have the specified type and
 * for which any of the higher order mode bits - those that don't
 * deal with the basic user, group, other permissions - are set.
 */

static int extract(sqlite3 * db, diminuto_fs_type_t type)
{
    int xc = 0;
    int rc = 0;
    char tt[2] = { '\0', '\0' };
    char * to = (char *)0;
    char * sql = (char *)0;
    char * sqlmessage = (char *)0;

    do {

        tt[0] = type;
        to = placer_expand_alloc(tt);

        sql = placer_format_alloc(Buffersize, 
            "SELECT * FROM census WHERE (type = '%s') AND (mode > %u);"
            , to
            , 0777U
        );
        free(to);

        if (sql == (char *)0) {
            xc = -20;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        sqlmessage = (char *)0;
        rc = sqlite3_exec(db, sql, placer_callback_generic, stderr, &sqlmessage);
        free(sql);

        if (rc != SQLITE_OK) {
            placer_message(sqlmessage);
            placer_error(rc);
            xc = -21;
            break;
        }

    } while (0);

    return xc;
}

/*
 * Perform a census by walking the file system and storing the absolute
 * path and attributes of every object encountered.
 */

static int insert(void * vp, const char * name, const char * path, size_t depth, const struct stat * statp)
{
    int xc = 0;
    sqlite3 * db = (sqlite3 *)0;
    int rc = 0;
    char * to = (char *)0;
    char * sql = (char *)0;
    char * sqlmessage = (char *)0;

    do {

        db = (sqlite3 *)vp;
   
        /*
         * Expand any single quotes in path.
         */

        to = placer_expand_alloc(path);

        /*
         * Insert the row into the database.
         */

        sql = placer_format_alloc(Buffersize,
            "INSERT INTO census VALUES ('%s', '%c', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)"
            , to
            , diminuto_fs_type(statp->st_mode)
            , statp->st_nlink
            , statp->st_uid
            , statp->st_gid
            , (statp->st_mode & ~S_IFMT)
            , statp->st_ino
            , statp->st_size
            , statp->st_blocks
            , major(statp->st_rdev)
            , minor(statp->st_rdev)
            , major(statp->st_dev)
            , minor(statp->st_dev)
            , statp->st_ctim.tv_sec
            , statp->st_ctim.tv_nsec
            , 0
        );
        free(to);

        if (sql == (char *)0) {
            xc = -12;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        sqlmessage = (char *)0;
        rc = sqlite3_exec(db, sql, placer_callback_generic, stderr, &sqlmessage);
        free(sql);

        if (rc != SQLITE_OK) {
            placer_message(sqlmessage);
            placer_error(rc);
            xc = -13;
            break;
        }

    } while (0);

    return xc;
}

/*
 * Main programn.
 */

int main(int argc, char * argv[])
{
    int xc = 0;
    int rc = 0;
    int ii = 0;
    sqlite3 * db = (sqlite3 *)0;
    int opt = 0;
    const char * database = (const char *)0;
    int test1 = 0;
    int test2 = 0;
    int test3 = 0;
    int test4 = 0;
    int test5 = 0;
    char * end = (char *)0;
    static const char USAGE[] = "-D DATABASE [ -B BLOCKSIZE ] [ -1 ] [ -2 ] [ -3 ] [ -4 ] [ -5 ] [ ROOT [ ROOT ... ] ]\n";
    extern char * optarg;
    extern int optind;
    extern int opterr;
    extern int optopt;

    do {

        Program = ((Program = strrchr(argv[0], '/')) == (const char *)0) ? argv[0] : Program + 1;

        while ((opt = getopt(argc, argv, "?12345B:D:dv")) >= 0) {
            switch (opt) {
            case '?':
                fprintf(stderr, "usage: %s %s\n", Program, USAGE);
                break;
            case '1':
                test1 = !0;
                break;
            case '2':
                test2 = !0;
                break;
            case '3':
                test3 = !0;
                break;
            case '4':
                test4 = !0;
                break;
            case '5':
                test5 = !0;
                break;
            case 'B':
                Buffersize = strtoul(optarg, &end, 0);
                if ((end != (char *)0) && (end[0] != '\0')) {
                    errno = EINVAL;
                    perror(optarg);
                    xc = 1;
                }
                break;
            case 'D':
                database = optarg;
                break;
            case 'd':
                Debug = !0;
                break;
            case 'v':
                Verbose = !0;
                break;
            default:
                fprintf(stderr, "usage: %s %s\n", Program, USAGE);
                xc = 1;
                break;
            }
        }

        if (xc != 0) {
            break;
        }

        if (database == (const char *)0) {
            errno = EINVAL;
            perror("-D");
            xc = 1;
            break;
        }

        if (Debug) {
            (void)placer_debug(stderr);
        }

        if ((rc = sqlite3_open(database, &db)) != SQLITE_OK) {
            placer_error(rc);
            xc = 2;
            break;
        } else if (db == (sqlite3 *)0) {
            errno = EADDRNOTAVAIL;
            perror(database);
            xc = 3;
            break;
        } else {
            /* Do nothing. */
        }

        for (; optind < argc; ++optind) {
            if ((rc = diminuto_fs_walk(argv[optind], insert, db)) == 0) {
                /* Do nothing. */
            } else {
                xc = rc;
                break;
            }
        }

        if (!test1) {
            /* Do nothing. */
        } else if ((rc = extract(db, DIMINUTO_FS_TYPE_FILE)) == 0) {
            /* Do nothing. */
        } else {
            xc = 5;
        }

        if (!test2) {
            /* Do nothing. */
        } else if ((rc = extract(db, DIMINUTO_FS_TYPE_DIRECTORY)) == 0) {
            /* Do nothing. */
        } else {
            xc = 5;
        }

        if (!test3) {
            /* Do nothing. */
        } else if ((rc = diminuto_fs_walk("/", enumerate, db)) == 0) {
            /* Do nothing. */
        } else {
            xc = 5;
        }

        if (!test4) {
            /* Do nothing. */
        } else if ((rc = diminuto_fs_walk("/", identify, db)) == 0) {
            /* Do nothing. */
        } else {
            xc = 5;
        }

        if (!test5) {
            /* Do nothing. */
        } else if ((rc = mark(db)) == 0) {
            /* Do nothing. */
        } else {
            xc = 5;
        }

        if ((rc = sqlite3_close(db)) != SQLITE_OK) {
            placer_error(rc);
            if (xc == 0) {
                xc = 9;
            }
            break;
        } else {
            db = (sqlite3 *)0;
        }

    } while (0);

    return (xc < 0) ? -xc : xc;
}
