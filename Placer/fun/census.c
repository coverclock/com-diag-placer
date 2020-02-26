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
 * census [ -? ] [ -d ] [ -v ] -D DATABASE [ -1 ] [ -2 ] [ -3 ] [ -4 ] [ -5 ] [ -6 ] [ [ -r ] ROOT ... ]
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

/*
 * TEST6
 *
 * Remove the entries for files that are not marked following a replace, then
 * unmark all entries.
 */

static int clean(sqlite3 * db)
{
    int xc = 0;
    int rc = 0;
    char * sql = (char *)0;
    placer_generic_callback_t state = PLACER_GENERIC_CALLBACK_INITIALIZER;
   
    do {

        /*
         *
         */

        sql = placer_sql_formata(Buffersize,
            "SELECT * FROM census WHERE mark == 0;"
        );
        if (sql == (char *)0) {
            xc = -100;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        rc = placer_exec(db, sql, placer_exec_generic_callback, &state);
        sqlite3_free(sql);
        if (rc != SQLITE_OK) {
            xc = -101;
            break;
        }

        /*
         *
         */

        sql = placer_sql_formata(Buffersize,
            "DELETE FROM census WHERE mark == 0;"
        );
        if (sql == (char *)0) {
            xc = -102;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        rc = placer_exec(db, sql, placer_exec_generic_callback, &state);
        sqlite3_free(sql);
        if (rc != SQLITE_OK) {
            xc = -103;
            break;
        }

        /*
         *
         */

        sql = placer_sql_formata(Buffersize,
            "UPDATE census SET mark = 0 WHERE mark != 0;"
        );
        if (sql == (char *)0) {
            xc = -104;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        rc = placer_exec(db, sql, placer_exec_generic_callback, &state);
        sqlite3_free(sql);
        if (rc != SQLITE_OK) {
            xc = -105;
            break;
        }

    } while (0);

    return xc;
}

/*
 * TEST5
 *
 * For every file in the file system, see if there is a file in the DB
 * with the same inode number but a different path name. This could be
 * a hard link, in which case the number of links field should be greater
 * than one. But it could also be that the file system has changed since
 * the census, and the inode number reused. We make that less likely by
 * only selecting those DB entries whose link count is greater than one.
 */

static int mark(sqlite3 * db)
{
    int xc = 0;
    int rc = 0;
    char * sql = (char *)0;
    placer_generic_callback_t state = PLACER_GENERIC_CALLBACK_INITIALIZER;
   
    do {

        /*
         *
         */

        sql = placer_sql_formata(Buffersize,
            "UPDATE census SET mark = 1 WHERE nlink > 1;"
        );
        if (sql == (char *)0) {
            xc = -110;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        rc = placer_exec(db, sql, placer_exec_generic_callback, &state);
        sqlite3_free(sql);
        if (rc != SQLITE_OK) {
            xc = -111;
            break;
        }

        /*
         *
         */

        sql = placer_sql_formata(Buffersize,
            "SELECT * FROM census WHERE mark != 0;"
        );
        if (sql == (char *)0) {
            xc = -112;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        rc = placer_exec(db, sql, placer_exec_generic_callback, &state);
        sqlite3_free(sql);
        if (rc != SQLITE_OK) {
            xc = -113;
            break;
        }

        /*
         *
         */

        sql = placer_sql_formata(Buffersize,
            "SELECT * FROM census WHERE (mark != 0) AND (nlink <= 1);"
        );
        if (sql == (char *)0) {
            xc = -114;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        rc = placer_exec(db, sql, placer_exec_generic_callback, &state);
        sqlite3_free(sql);
        if (rc != SQLITE_OK) {
            xc = -115;
            break;
        }

        /*
         *
         */

        sql = placer_sql_formata(Buffersize,
            "SELECT * FROM census WHERE (mark == 0) AND (nlink > 1);"
        );
        if (sql == (char *)0) {
            xc = -116;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        rc = placer_exec(db, sql, placer_exec_generic_callback, &state);
        sqlite3_free(sql);
        if (rc != SQLITE_OK) {
            xc = -117;
            break;
        }

        /*
         *
         */

        sql = placer_sql_formata(Buffersize,
            "UPDATE census SET mark = 0 WHERE mark != 0;"
        );
        if (sql == (char *)0) {
            xc = -118;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        rc = placer_exec(db, sql, placer_exec_generic_callback, &state);
        sqlite3_free(sql);
        if (rc != SQLITE_OK) {
            xc = -119;
            break;
        }

        /*
         *
         */

        sql = placer_sql_formata(Buffersize,
            "SELECT * FROM census WHERE mark != 0;"
        );
        if (sql == (char *)0) {
            xc = -120;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        rc = placer_exec(db, sql, placer_exec_generic_callback, &state);
        sqlite3_free(sql);
        if (rc != SQLITE_OK) {
            xc = -121;
            break;
        }

    } while (0);

    return xc;
}

/*
 * TEST4
 *
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
            fputs("Aliased: \"", stdout);
            fputs(value[ii], stdout);
            fputs("\" \"", stdout);
            fputs(pp, stdout);
            fputs("\"\n", stdout);
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
   
    do {

        if (statp->st_nlink <= 1) {
            break;
        }

        db = (sqlite3 *)vp;

        /*
         * Select the row from the database.
         */

        sql = placer_sql_formata(Buffersize,
            "SELECT * FROM census WHERE (ino = %d) AND (devmajor = %d) AND (devminor = %d) AND (nlink > 1);"
            , statp->st_ino
            , major(statp->st_dev)
            , minor(statp->st_dev)
        );
        if (sql == (char *)0) {
            xc = -130;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        rc = placer_exec(db, sql, identifier, (char *)path);
        sqlite3_free(sql);
        if (rc != SQLITE_OK) {
            xc = -131;
            break;
        }

    } while (0);

    return xc;
}

/*
 * TEST3
 *
 * For every file in the file system, enumerate the number of entries
 * for the same path in the DB. Something is amiss if that number if
 * greater than one. But it could also be zero, if the file has been
 * deleted since the census was done.
 */

static int enumerate(void * vp, const char * name, const char * path, size_t depth, const struct stat * statp)
{
    int xc = 0;
    sqlite3 * db = (sqlite3 *)0; 
    int rc = 0;
    char * to = (char *)0;
    char * sql = (char *)0;
    placer_generic_callback_t state = { (FILE *)0, 0 };
   
    do {

        db = (sqlite3 *)vp;

        to = placer_str_expanda(path);

        /*
         * Select the row from the database.
         */

        sql = placer_sql_formata(Buffersize,
            "SELECT * FROM census WHERE (path = '%s');"
            , to
        );
        sqlite3_free(to);
        if (sql == (char *)0) {
            xc = -140;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        state.count = 0;
        rc = placer_exec(db, sql, placer_exec_generic_callback, &state);
        sqlite3_free(sql);
        if (rc != SQLITE_OK) {
            xc = -141;
            break;
        }

        if (state.count < 1) {
            fputs("NONE: ", stdout);
            fputs(path, stdout);
            fputc('\n', stdout);
        } else if (state.count > 1) {
            fputs("MANY: ", stdout);
            fputs(path, stdout);
            fputc('\n', stdout);
        } else {
            /* Do nothing. */
        }

    } while (0);

    return xc;
}

/*
 * TEST1, TEST2
 *
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
    placer_generic_callback_t state = PLACER_GENERIC_CALLBACK_INITIALIZER;

    do {

        tt[0] = type;
        to = placer_str_expanda(tt);

        sql = placer_sql_formata(Buffersize, 
            "SELECT * FROM census WHERE (type = '%s') AND (mode > %u);"
            , to
            , 0777U
        );
        sqlite3_free(to);
        if (sql == (char *)0) {
            xc = -150;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        rc = placer_exec(db, sql, placer_exec_generic_callback, &state);
        sqlite3_free(sql);
        if (rc != SQLITE_OK) {
            xc = -151;
            break;
        }

    } while (0);

    return xc;
}

/*
 * Update a census by walking the file system and storing the absolute
 * path and attributes of every object encountered.
 */

static int replace(void * vp, const char * name, const char * path, size_t depth, const struct stat * statp)
{
    int xc = 0;
    sqlite3 * db = (sqlite3 *)0;
    int rc = 0;
    char * to = (char *)0;
    char * sql = (char *)0;
    placer_generic_callback_t state = PLACER_GENERIC_CALLBACK_INITIALIZER;

    do {

        db = (sqlite3 *)vp;
   
        /*
         * Expand any single quotes in path.
         */

        to = placer_str_expanda(path);

        /*
         * Insert the row into the database.
         */

        sql = placer_sql_formata(Buffersize,
            "REPLACE INTO census VALUES ('%s', '%c', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %lld, %d);"
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
            , (unsigned long long)((1000000000ULL * statp->st_ctim.tv_sec) + statp->st_ctim.tv_nsec)
            , 1
        );
        sqlite3_free(to);
        if (sql == (char *)0) {
            xc = -160;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        rc = placer_exec(db, sql, placer_exec_generic_callback, &state);
        sqlite3_free(sql);
        if (rc != SQLITE_OK) {
            xc = -161;
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
    placer_generic_callback_t state = PLACER_GENERIC_CALLBACK_INITIALIZER;

    do {

        db = (sqlite3 *)vp;
   
        /*
         * Expand any single quotes in path.
         */

        to = placer_str_expanda(path);

        /*
         * Insert the row into the database.
         */

        sql = placer_sql_formata(Buffersize,
            "INSERT INTO census VALUES ('%s', '%c', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %lld, %d);"
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
            , (unsigned long long)((1000000000ULL * statp->st_ctim.tv_sec) + statp->st_ctim.tv_nsec)
            , 0
        );
        sqlite3_free(to);
        if (sql == (char *)0) {
            xc = -170;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        rc = placer_exec(db, sql, placer_exec_generic_callback, &state);
        sqlite3_free(sql);
        if (rc != SQLITE_OK) {
            xc = -171;
            break;
        }

    } while (0);

    return xc;
}

/*
 * TEST0
 *
 * Print the contents of the DB and report the number of entries.
 */

static int show(sqlite3 * db)
{
    int xc = 0;
    int rc = 0;
    char * sql = (char *)0;
    placer_generic_callback_t state = PLACER_GENERIC_CALLBACK_INITIALIZER;
   
    do {

        /*
         *
         */

        sql = placer_sql_formata(Buffersize,
            "SELECT * FROM census;"
        );
        if (sql == (char *)0) {
            xc = -180;
            break;
        }

        if (Verbose) {
            fputs(sql, stderr);
            fputc('\n', stderr);
        }

        rc = placer_exec(db, sql, placer_exec_generic_callback, &state);
        sqlite3_free(sql);
        if (rc != SQLITE_OK) {
            xc = -181;
            break;
        }

        fprintf(stderr, "count=%zu\n", state.count);

    } while (0);

    return xc;
}

/* 
 * Create the table.
 */

static int create(sqlite3 * db)
{
    static const char CREATE[] = "CREATE TABLE census ( path TEXT PRIMARY KEY, type TEXT, nlink INTEGER, uid INTEGER, gid INTEGER, mode INTEGER, ino INTEGER, size INTEGER, blocks INTEGER, rdevmajor INTEGER, rdevminor INTEGER, devmajor INTEGER, devminor INTEGER, ctime INTEGER, mark INTEGER);";
    int xc = 0;
    int rc = 0;

    if (Verbose) {
        fputs(CREATE, stderr);
        fputc('\n', stderr);
    }

    rc = placer_exec(db, CREATE, (placer_exec_callback_t *)0, (void *)0);
    if (rc != SQLITE_OK) {
        xc = -190;
    }

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
    diminuto_fs_walker_t * cp = (diminuto_fs_walker_t *)0;
    const char * database = (const char *)0;
    int creating = 0;
    int test0 = 0;
    int test1 = 0;
    int test2 = 0;
    int test3 = 0;
    int test4 = 0;
    int test5 = 0;
    int test6 = 0;
    char * end = (char *)0;
    static const char USAGE[] = "-D DATABASE [ -B BLOCKSIZE ] [ -d ] [ -v ] [ -0 ] [ -1 ] [ -2 ] [ -3 ] [ -4 ] [ -5 ] [ -6 ] [ -c ] [ [ -r ]  ROOT [ ROOT ... ] ]\n";
    int opt = 0;
    extern char * optarg;
    extern int optind;
    extern int opterr;
    extern int optopt;

    do {

        Program = ((Program = strrchr(argv[0], '/')) == (const char *)0) ? argv[0] : Program + 1;

        cp = insert;

        while ((opt = getopt(argc, argv, "?0123456B:D:cdrv")) >= 0) {
            switch (opt) {
            case '?':
                fprintf(stdout, "usage: %s %s\n", Program, USAGE);
                break;
            case '0':
                test0 = !0;
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
            case '6':
                test6 = !0;
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
            case 'c':
                creating = !0;
                break;
            case 'd':
                Debug = !0;
                break;
            case 'r':
                cp = replace;
                break;
            case 'v':
                Verbose = !0;
                break;
            default:
                fprintf(stdout, "usage: %s %s\n", Program, USAGE);
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

        /*
         * Create, or do nothing.
         */

        if (!creating) {
            /* Do nothing. */
        } else if ((rc = create(db)) == 0) {
            /* Do nothing. */
        } else {
            xc = rc;
            break;
        }

        /*
         * Insert, Replace, or Do Nothing.
         */

        for (; optind < argc; ++optind) {
            if ((rc = diminuto_fs_walk(argv[optind], cp, db)) == 0) {
                /* Do nothing. */
            } else {
                xc = rc;
                break;
            }
        }

        /*
         * Perform a test.
         */

        if (!test0) {
            /* Do nothing. */
        } else if ((rc = show(db)) == 0) {
            /* Do nothing. */
        } else {
            xc = rc;
        }

        if (!test1) {
            /* Do nothing. */
        } else if ((rc = extract(db, DIMINUTO_FS_TYPE_FILE)) == 0) {
            /* Do nothing. */
        } else {
            xc = rc;
        }

        if (!test2) {
            /* Do nothing. */
        } else if ((rc = extract(db, DIMINUTO_FS_TYPE_DIRECTORY)) == 0) {
            /* Do nothing. */
        } else {
            xc = rc;
        }

        if (!test3) {
            /* Do nothing. */
        } else if ((rc = diminuto_fs_walk("/", enumerate, db)) == 0) {
            /* Do nothing. */
        } else {
            xc = rc;
        }

        if (!test4) {
            /* Do nothing. */
        } else if ((rc = diminuto_fs_walk("/", identify, db)) == 0) {
            /* Do nothing. */
        } else {
            xc = rc;
        }

        if (!test5) {
            /* Do nothing. */
        } else if ((rc = mark(db)) == 0) {
            /* Do nothing. */
        } else {
            xc = rc;
        }

        if (!test6) {
            /* Do nothing. */
        } else if ((rc = clean(db)) == 0) {
            /* Do nothing. */
        } else {
            xc = rc;
        }

    } while (0);

    if (db == (sqlite3 *)0) {
        /* Do nothing. */
    } else if ((rc = sqlite3_close(db)) == SQLITE_OK) {
        /* Do nothing. */
    } else {
        placer_error(rc);
        if (xc == 0) {
            xc = 9;
        }
    }

    db = (sqlite3 *)0;

    return (xc < 0) ? -xc : xc;
}
