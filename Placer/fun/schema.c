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
#include "com/diag/diminuto/diminuto_countof.h"
#include "com/diag/diminuto/diminuto_core.h"
#include "com/diag/placer/placer.h"

#include "com/diag/placer/placer_structure.h"
#include "schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_steps_prototype.h"
#include "schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_steps_callback.h"
#include "schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_stmt_prototype.h"
#include "schema.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_stmt_bind.h"
#include "schema.h"
#include "com/diag/placer/placer_end.h"

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
    sqlite3_stmt * sp = (sqlite3_stmt *)0;
    static const char * SELECT[] = {
        "SELECT * FROM Schema WHERE mark == 0;",
        "DELETE FROM Schema WHERE mark == 0;",
        "UPDATE Schema SET mark = 0 WHERE mark != 0;",
        "SELECT * FROM Schema WHERE mark != 0;"
    };
    placer_generic_callback_t state = PLACER_GENERIC_CALLBACK_INITIALIZER;
    int ii = 0;

    for (ii = 0; ii < countof(SELECT); ii++) {

        fputs(SELECT[ii], state.fp);
        fputc('\n', state.fp);

        sp = placer_prepare(db, SELECT[ii]);
        if (sp == (sqlite3_stmt *)0) {
            xc = -100;
            break;
        }

        rc = placer_steps(sp, placer_steps_generic_callback, &state);
        if (rc != SQLITE_OK) {
            xc = -101;
            break;
        }

    }
   
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
    sqlite3_stmt * sp = (sqlite3_stmt *)0;
    static const char * SELECT[] = {
        "UPDATE Schema SET mark = 1 WHERE nlink > 1;",
        "SELECT * FROM Schema WHERE mark != 0;",
        "SELECT * FROM Schema WHERE (mark != 0) AND (nlink <= 1);",
        "SELECT * FROM Schema WHERE (mark == 0) AND (nlink > 1);",
        "UPDATE Schema SET mark = 0 WHERE mark != 0;",
        "SELECT * FROM Schema WHERE mark != 0;"
    };
    placer_generic_callback_t state = PLACER_GENERIC_CALLBACK_INITIALIZER;
    int ii = 0;

    for (ii = 0; ii < countof(SELECT); ii++) {

        fputs(SELECT[ii], state.fp);
        fputc('\n', state.fp);

        sp = placer_prepare(db, SELECT[ii]);
        if (sp == (sqlite3_stmt *)0) {
            xc = -110;
            break;
        }

        rc = placer_steps(sp, placer_steps_generic_callback, &state);
        if (rc != SQLITE_OK) {
            xc = -111;
            break;
        }

    }

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

static int identifier(sqlite3_stmt * sp, void * vp)
{
    const char * pp = (char *)0;
    const char * vv = (char *)0;
    int ii = 0;

    pp = (const char *)vp;
    vv = sqlite3_column_text(sp, 0);

    if (strcmp(pp, vv) != 0) {
        fputs("Aliased: \"", stdout);
        fputs(vv, stdout);
        fputs("\" \"", stdout);
        fputs(pp, stdout);
        fputs("\"\n", stdout);
    }

    return SQLITE_OK;
}

static int identify(void * vp, const char * name, const char * path, size_t depth, const struct stat * statp)
{
    int xc = 0;
    int rc = 0;
    sqlite3 * db = (sqlite3 *)0;
    sqlite3_stmt * sp = (sqlite3_stmt *)0;
    static const char SELECT[] = "SELECT * FROM Schema WHERE (ino = ?) AND (devmajor = ?) AND (devminor = ?) AND (nlink > 1);";
    int ii = 0;
   
    do {

        if (statp->st_nlink <= 1) {
            break;
        }

        db = (sqlite3 *)vp;

        fputs(path, stdout);
        fputc('\n', stdout);

        sp = placer_prepare(db, SELECT);
        if (sp == (sqlite3_stmt *)0) {
            xc = -120;
            break;
        }

        rc = sqlite3_bind_int(sp, ++ii, statp->st_ino);
        if (rc != SQLITE_OK) {
            xc = -121;
            break;
        }

        rc = sqlite3_bind_int(sp, ++ii, major(statp->st_dev));
        if (rc != SQLITE_OK) {
            xc = -122;
            break;
        }

        rc = sqlite3_bind_int(sp, ++ii, minor(statp->st_dev));
        if (rc != SQLITE_OK) {
            xc = -123;
            break;
        }

        /*
         * Select the row from the database.
         */

        rc = placer_steps(sp, identifier, (void *)path);
        if (rc != SQLITE_OK) {
            xc = -124;
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
    sqlite3_stmt * sp = (sqlite3_stmt *)0;
    static const char SELECT[] = "SELECT * FROM Schema WHERE (path = ?);";
    int rc = 0;
    struct Schema data[2];
    struct Schema * pointers[] = { &data[0], &data[1], (struct Schema *)0 };
    struct Schema ** current = &pointers[0];
 
    do {

        db = (sqlite3 *)vp;

        /*
         * Select the row from the database.
         */

        sp = placer_prepare(db, SELECT);
        if (sp == (sqlite3_stmt *)0) {
            xc = -130;
            break;
        }

        rc = sqlite3_bind_text(sp, 1, path, strlen(path), (placer_bind_callback_t *)0);
        if (rc != SQLITE_OK) {
            xc = -131;
            break;
        }

        rc = placer_steps(sp, placer_steps_struct_Schema_callback, &current);
        if (rc != SQLITE_OK) {
            xc = -132;
            break;
        }

        if (current == &pointers[0]) {
            fputs("NONE: ", stdout);
            fputs(path, stdout);
            fputc('\n', stdout);
        } else if (current == &pointers[1]) {
            fputs("ONLY: ", stdout);
            fputs(path, stdout);
            fputs(data[0].path, stdout);
            fputc('\n', stdout);
        } else if (current == &pointers[2]) {
            fputs("MANY: ", stdout);
            fputs(path, stdout);
            fputs(data[0].path, stdout);
            fputs(data[1].path, stdout);
            fputc('\n', stdout);
        } else {
            diminuto_core_fatal();
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
    sqlite3_stmt * sp = (sqlite3_stmt *)0;
    static const char SELECT[] = "SELECT * FROM Schema WHERE (type = '%s') AND (mode > %u);";

    do {

        tt[0] = type;
        to = placer_str_expanda(tt);

        sql = placer_sql_formata(Buffersize, SELECT, to, 0777U);
        sqlite3_free(to);
        if (sql == (char *)0) {
            xc = -140;
            break;
        }

        sp = placer_prepare(db, sql);
        sqlite3_free(sql);
        if (sp == (sqlite3_stmt *)0) {
            xc = -141;
            break;
        }

        rc = placer_steps(sp, placer_steps_generic_callback, &state);
        if (rc != SQLITE_OK) {
            xc = -142;
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
    sqlite3_stmt * sp = (sqlite3_stmt *)0;
    int rc = 0;
    placer_generic_callback_t state = PLACER_GENERIC_CALLBACK_INITIALIZER;
#include "com/diag/placer/placer_sql_replace.h"
#include "schema.h"
#include "com/diag/placer/placer_end.h"
    struct Schema schema;

    fputs(path, stdout);
    fputc('\n', stdout);

    strncpy(schema.path, path, sizeof(schema.path));
    schema.type[0] = diminuto_fs_type(statp->st_mode); schema.type[1] = '\0';
    schema.nlink = statp->st_nlink;
    schema.uid = statp->st_uid;
    schema.gid = statp->st_gid;
    schema.mode = statp->st_mode & ~S_IFMT;
    schema.ino = statp->st_ino;
    schema.size = statp->st_size;
    schema.blocks = statp->st_blocks;
    schema.rdevmajor = major(statp->st_rdev);
    schema.rdevminor = minor(statp->st_rdev);
    schema.devmajor = major(statp->st_dev);
    schema.devminor = minor(statp->st_dev);
    schema.ctime = (unsigned long long)((1000000000ULL * statp->st_ctim.tv_sec) + statp->st_ctim.tv_nsec);
    memcpy(schema.status, statp, sizeof(schema.status));
    schema.mark = 0;

    /*
     * Replace the row in the database.
     */

    do {

        db = (sqlite3 *)vp;

        sp = placer_prepare(db, PLACER_SQL_struct_Schema_REPLACE);
        if (sp == (sqlite3_stmt *)0) {
            xc = -150;
            break;
        }

        rc = placer_stmt_struct_Schema_bind(sp, &schema);
        if (rc != SQLITE_OK) {
            xc = -151;
            break;
        }

        rc = placer_steps(sp, placer_steps_generic_callback, &state);
        if (rc != SQLITE_OK) {
            xc = -152;
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
    sqlite3_stmt * sp = (sqlite3_stmt *)0;
    int rc = 0;
    placer_generic_callback_t state = PLACER_GENERIC_CALLBACK_INITIALIZER;
#include "com/diag/placer/placer_sql_insert.h"
#include "schema.h"
#include "com/diag/placer/placer_end.h"
    struct Schema schema;

    fputs(path, stdout);
    fputc('\n', stdout);

    strncpy(schema.path, path, sizeof(schema.path));
    schema.type[0] = diminuto_fs_type(statp->st_mode); schema.type[1] = '\0';
    schema.nlink = statp->st_nlink;
    schema.uid = statp->st_uid;
    schema.gid = statp->st_gid;
    schema.mode = statp->st_mode & ~S_IFMT;
    schema.ino = statp->st_ino;
    schema.size = statp->st_size;
    schema.blocks = statp->st_blocks;
    schema.rdevmajor = major(statp->st_rdev);
    schema.rdevminor = minor(statp->st_rdev);
    schema.devmajor = major(statp->st_dev);
    schema.devminor = minor(statp->st_dev);
    schema.ctime = (unsigned long long)((1000000000ULL * statp->st_ctim.tv_sec) + statp->st_ctim.tv_nsec);
    memcpy(schema.status, statp, sizeof(schema.status));
    schema.mark = 0;

    db = (sqlite3 *)vp;
   
    /*
     * Insert the row into the database.
     */

    do {

        sp = placer_prepare(db, PLACER_SQL_struct_Schema_INSERT);
        if (sp == (sqlite3_stmt *)0) {
            xc = -160;
            break;
        }

        rc = placer_stmt_struct_Schema_bind(sp, &schema);
        if (rc != SQLITE_OK) {
            xc = -161;
            break;
        }

        rc = placer_steps(sp, placer_steps_generic_callback, &state);
        if (rc != SQLITE_OK) {
            xc = -162;
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
    placer_generic_callback_t state = PLACER_GENERIC_CALLBACK_INITIALIZER;
    sqlite3_stmt * sp = (sqlite3_stmt *)0;
    static const char SELECT[] = "SELECT * FROM Schema;";

    do {

        sp = placer_prepare(db, SELECT);
        if (sp == (sqlite3_stmt *)0) {
            xc = -170;
            break;
        }

        rc = placer_steps(sp, placer_steps_generic_callback, &state);
        if (rc != SQLITE_OK) {
            xc = -171;
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
    int xc = 0;
    int rc = 0;
    placer_generic_callback_t state = PLACER_GENERIC_CALLBACK_INITIALIZER;
#include "com/diag/placer/placer_sql_create.h"
#include "schema.h"
#include "com/diag/placer/placer_end.h"
    sqlite3_stmt * sp = (sqlite3_stmt *)0;

    do {

        sp = placer_prepare(db, PLACER_SQL_struct_Schema_CREATE);
        if (sp == (sqlite3_stmt *)0) {
            xc = -180;
            break;
        }

        rc = placer_steps(sp, (placer_steps_callback_t *)0, (void *)0);
        if (rc != SQLITE_OK) {
            xc = -181;
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
    diminuto_fs_walker_t * cp = (diminuto_fs_walker_t *)0;
    const char * database = (const char *)0;
    const char * path = "/";
    int creating = 0;
    int test0 = 0;
    int test1 = 0;
    int test2 = 0;
    int test3 = 0;
    int test4 = 0;
    int test5 = 0;
    int test6 = 0;
    int test7 = 0;
    char * end = (char *)0;
    static const char USAGE[] = "-D DATABASE [ -B BLOCKSIZE ] [ -d ] [ -v ] [ -0 ] [ -1 ] [ -2 ] [ -3 ] [ -4 ] [ -5 ] [ -6 ] [ -7 ] [ -P PATH ] [ [ -c | -r ]  ROOT [ ROOT ... ] ]\n";
    int opt = 0;
    extern char * optarg;
    extern int optind;
    extern int opterr;
    extern int optopt;

    do {

        Program = ((Program = strrchr(argv[0], '/')) == (const char *)0) ? argv[0] : Program + 1;

        cp = insert;

        while ((opt = getopt(argc, argv, "?01234567B:D:P:cdrv")) >= 0) {
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
            case '7':
                test7 = !0;
                break;
            case 'B':
                Buffersize = strtoul(optarg, &end, 0);
                if ((end != (char *)0) && (end[0] != '\0')) {
                    errno = EINVAL;
                    perror(optarg);
                    xc = -10;
                }
                break;
            case 'D':
                database = optarg;
                break;
            case 'P':
                path = optarg;
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
                xc = -11;
                break;
            }
        }

        if (xc != 0) {
            break;
        }

        if (database == (const char *)0) {
            errno = EINVAL;
            perror("-D");
            xc = -12;
            break;
        }

        if (Debug) {
            (void)placer_debug(stderr);
        }

        if ((rc = diminuto_fs_mkdir_p(database, 0755, 0)) < 0) {
            perror(database);
            xc = -13;
            break;
        }

        if ((rc = sqlite3_open(database, &db)) != SQLITE_OK) {
            placer_error(rc);
            xc = -14;
            break;
        } else if (db == (sqlite3 *)0) {
            errno = EADDRNOTAVAIL;
            perror(database);
            xc = -15;
            break;
        } else {
            /* Do nothing. */
        }

        /*
         * Create, or Do Nothing.
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
        } else if ((rc = diminuto_fs_walk(path, enumerate, db)) == 0) {
            /* Do nothing. */
        } else {
            xc = rc;
        }

        if (!test4) {
            /* Do nothing. */
        } else if ((rc = diminuto_fs_walk(path, identify, db)) == 0) {
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

        if (!test7) {
            /* Do nothing. */
        } else if ((rc = enumerate(db, path, path, 0, (struct stat *)0)) == 0) {
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
            xc = -16;
        }
    }

    db = (sqlite3 *)0;

    return (xc < 0) ? -xc : xc;
}
