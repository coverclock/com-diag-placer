/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in LICENSE.txt<BR>
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
 * survey [ -? ] -D DATABASE [ -B BLOCKSIZE ] [ -d ] [ -v ] [ -0 ] [ -1 ] [ -2 ] [ -3 ] [ -4 ] [ -5 ] [ -6 ] [ -7 ] [ -8 ] [ -9 ] [ -P PATH ] [ -I INODE ] [ [ -c ]  [ -i | -r ]  ROOT [ ROOT ... ] ]
 *
 * EXAMPLES
 *
 * survey -D out/host/sql/survey.db /
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
#include "com/diag/diminuto/diminuto_log.h"
#include "com/diag/placer/placer.h"

/*
 * Path Schema
 */

#include "com/diag/placer/placer_structure_definition.h"
#include "SchemaPath.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_structure_prototype.h"
#include "SchemaPath.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_structure_display.h"
#include "SchemaPath.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_steps_prototype.h"
#include "SchemaPath.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_steps_callback.h"
#include "SchemaPath.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_stmt_prototype.h"
#include "SchemaPath.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_stmt_bind.h"
#include "SchemaPath.h"
#include "com/diag/placer/placer_end.h"

/*
 * Node Schema
 */

#include "com/diag/placer/placer_structure_definition.h"
#include "SchemaNode.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_structure_prototype.h"
#include "SchemaNode.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_structure_display.h"
#include "SchemaNode.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_steps_prototype.h"
#include "SchemaNode.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_steps_callback.h"
#include "SchemaNode.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_stmt_prototype.h"
#include "SchemaNode.h"
#include "com/diag/placer/placer_end.h"

#include "com/diag/placer/placer_stmt_bind.h"
#include "SchemaNode.h"
#include "com/diag/placer/placer_end.h"

static const char * Program = (const char *)0;
static int Debug = 0;
static int Verbose = 0;
static size_t Buffersize = 256;
static const char * Directory = (const char *)0;

/*
 * TEST9
 *
 * Derive the Node table from the Path table. Note: does NOt use the Write
 * Ahead Log (WAL) which wouyld allow readers too work concurrently with
 * a writer.
 */

static int derive_select_path_callback(sqlite3_stmt * sp, void * vp)
{
    int rc = SQLITE_OK;
    FILE * fp = (FILE *)0;
    struct Path path =
#include "com/diag/placer/placer_structure_initializer.h"
#include "SchemaPath.h"
#include "com/diag/placer/placer_end.h"
    struct Node node =
#include "com/diag/placer/placer_structure_initializer.h"
#include "SchemaNode.h"
#include "com/diag/placer/placer_end.h"
    struct Path * list[] = { &path, (struct Path *)0 };
    struct Path ** here = (struct Path **)0;
    placer_generic_callback_t state = PLACER_GENERIC_CALLBACK_INITIALIZER;
    size_t size = 0;

    do {

        fp = (FILE *)vp;

        here = &list[0];
        rc = placer_struct_Path_steps_callback(sp, &here);
        if (rc != SQLITE_OK) {
            placer_error(rc);
            break;
        }

        if (Verbose) {
            state.count = 1;
            rc = placer_generic_steps_callback(sp, &state);
            if (rc != SQLITE_OK) {
                placer_error(rc);
                break;
            }
        }

        node.ino = path.ino;
        node.nlink = path.nlink;
        node.uid = path.uid;
        node.gid = path.gid;
        node.mode = path.mode;
        node.blocks = path.blocks;
        node.size = path.size;
        node.devmajor = path.devmajor;
        node.devminor = path.devminor;
        node.ctime = path.ctime;

        if (Verbose) {
            placer_struct_Node_display(stdout, &node);
        }
        
        size = fwrite(&node, sizeof(node), 1, fp);
        if (size != 1) {
            diminuto_perror("fwrite");
            rc = SQLITE_ERROR;
            break;
        }

    } while (0);

    return rc;
}

static int derive(sqlite3 * db)
{
    int xc = 0;
    int rc = 0;
    char * temporary = (char *)0;
    int fd = -1;
    FILE * fp = (FILE *)0;
    sqlite3_stmt * sp = (sqlite3_stmt *)0;
    static const char SELECTPATH[] = "SELECT * FROM Path;";
#include "com/diag/placer/placer_sql_replace.h"
#include "SchemaNode.h"
#include "com/diag/placer/placer_end.h"
    struct Node node =
#include "com/diag/placer/placer_structure_initializer.h"
#include "SchemaNode.h"
#include "com/diag/placer/placer_end.h"
    placer_generic_callback_t state = PLACER_GENERIC_CALLBACK_INITIALIZER;
    static const char SELECTNODE[] = "SELECT * FROM Node;";
    size_t size = 0;

    do {

        size = strlen(Directory) + 1 + strlen(Program) + 1 + sizeof("XXXXXX");
        temporary = (char *)malloc(size);
        if (temporary == (char *)0) {
            diminuto_perror("malloc");
            xc = -200;
            break;
        }

        strcpy(temporary, Directory);
        strcat(temporary, "/");
        strcat(temporary, Program);
        strcat(temporary, "-");
        strcat(temporary, "XXXXXX");

        if (Verbose) {
            fputs(temporary, stdout);
            fputc('\n', stdout);
        }

        fd = mkstemp(temporary);
        if (fd < 0) {
            diminuto_perror(temporary);
            xc = -201;
            break;
        }

        fp = fdopen(fd, "r+");
        if (fp == (FILE *)0) {
            diminuto_perror(temporary);
            xc = -202;
            break;
        }

        sp = placer_prepare(db, SELECTPATH);
        if (sp == (sqlite3_stmt *)0) {
            xc = -203;
            break;
        }

        rc = placer_steps(sp, derive_select_path_callback, fp);
        if (rc != SQLITE_OK) {
            xc = -204;
            break;
        }

        rewind(fp);

        while (!0) {

            size = fread(&node, sizeof(node), 1, fp);
            if (size == 1) {
                /* Do nothing. */
            } else if (feof(fp)) {
                break;
            } else {
                diminuto_perror(temporary);
                xc = -205;
                break;
            }

            sp = placer_prepare(db, PLACER_struct_Node_SQL_REPLACE);
            if (sp == (sqlite3_stmt *)0) {
                xc = -206;
                break;
            }

            rc = placer_struct_Node_stmt_bind(sp, &node);
            if (rc != SQLITE_OK) {
                xc = -207;
                break;
            }

            rc = placer_steps(sp, placer_generic_steps_callback, &state);
            if (rc != SQLITE_OK) {
                xc = -208;
                break;
            }

        }

        if (xc != 0) {
            break;
        }

        if (Verbose) {

            sp = placer_prepare(db, SELECTNODE);
            if (sp == (sqlite3_stmt *)0) {
                xc = -208;
                break;
            }

            state.count = 0;

            rc = placer_steps(sp, placer_generic_steps_callback, &state);
            if (rc != SQLITE_OK) {
                xc = -209;
                break;
            }

        }

    } while (0);

    if (fp == (FILE *)0) {
        /* Do nothing. */
    } else if (fclose(fp) != EOF) {
        /* Do nothing. */
    }  else {
        diminuto_perror(temporary);
        xc = -220;
    }

    if (fd < 0) {
        /* Do nothing. */
    } else if (unlink(temporary) == 0) {
        /* Do nothing. */
    } else {
        diminuto_perror(temporary);
        xc = SQLITE_ERROR;
    }

    if (temporary != (char *)0) {
        free(temporary);
    }

    return xc;
}

/*
 * TEST8
 *
 * Find every entry who has a specific inode number. This includes
 * files in different file systems which may coincidentally have
 * the same inode number.
 */

static int reveal(sqlite3 * db, ino_t ino)
{
    int xc = 0;
    int rc = 0;
    sqlite3_stmt * sp = (sqlite3_stmt *)0;
    int ii = 0;

    do {

        printf("INODE: %lld\n", (long long)ino);

        {
            static const char SELECT[] = "SELECT * FROM Node WHERE (ino = ?);";
            struct Node data[8];
            struct Node * pointers[] = { &data[0], &data[1], &data[2], &data[3], &data[4], &data[5], &data[6], &data[7], (struct Node *)0 };
            struct Node ** current = &pointers[0];

            sp = placer_prepare(db, SELECT);
            if (sp == (sqlite3_stmt *)0) {
                xc = -190;
                break;
            }

            rc = sqlite3_bind_int(sp, 1, ino);
            if (rc != SQLITE_OK) {
                xc = -191;
                break;
            }

            rc = placer_steps(sp, placer_struct_Node_steps_callback, &current);
            if (rc != SQLITE_OK) {
                xc = -192;
                break;
            }

            for (ii = 0; &pointers[ii] != current; ++ii) {
                placer_struct_Node_display(stdout, &data[ii]);
            }
        }

        {
            static const char SELECT[] = "SELECT * FROM Path WHERE (ino = ?);";
            struct Path data[8];
            struct Path * pointers[] = { &data[0], &data[1], &data[2], &data[3], &data[4], &data[5], &data[6], &data[7], (struct Path *)0 };
            struct Path ** current = &pointers[0];

            sp = placer_prepare(db, SELECT);
            if (sp == (sqlite3_stmt *)0) {
                xc = -190;
                break;
            }

            rc = sqlite3_bind_int(sp, 1, ino);
            if (rc != SQLITE_OK) {
                xc = -191;
                break;
            }

            rc = placer_steps(sp, placer_struct_Path_steps_callback, &current);
            if (rc != SQLITE_OK) {
                xc = -192;
                break;
            }

            for (ii = 0; &pointers[ii] != current; ++ii) {
                placer_struct_Path_display(stdout, &data[ii]);
            }
        }

    } while (0);

    return xc;
}

/*
 * TEST6
 *
 * Remove the entries for files that are marked (for exmaple following
 * a -r).
 */

static int clean(sqlite3 * db)
{
    int xc = 0;
    int rc = 0;
    sqlite3_stmt * sp = (sqlite3_stmt *)0;
    static const char * SQL[] = {
        "SELECT * FROM Path WHERE mark == 0;",
        "SELECT * FROM Path WHERE mark != 0;",
        "DELETE FROM Path WHERE mark == 1;",
        "SELECT * FROM Path WHERE mark == 0;",
        "SELECT * FROM Path WHERE mark != 0;",
    };
    placer_generic_callback_t state = PLACER_GENERIC_CALLBACK_INITIALIZER;
    int ii = 0;

    if (!Verbose) {
        state.fp = (FILE *)0;
    }

    for (ii = 0; ii < countof(SQL); ii++) {

        if (Verbose) {
            fputs(SQL[ii], state.fp);
            fputc('\n', state.fp);
        }

        sp = placer_prepare(db, SQL[ii]);
        if (sp == (sqlite3_stmt *)0) {
            xc = -100;
            break;
        }

        rc = placer_steps(sp, placer_generic_steps_callback, &state);
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
 * Mark every file in the database. This is so you can rescan the file
 * system with -r and then subsequently remove the entries that are
 * stale.
 */

static int mark(sqlite3 * db)
{
    int xc = 0;
    int rc = 0;
    sqlite3_stmt * sp = (sqlite3_stmt *)0;
    static const char * SQL[] = {
        "SELECT * FROM Path WHERE mark != 0;",
        "UPDATE Schema SET mark = 1;",
        "SELECT * FROM Path WHERE mark != 0;",
    };
    placer_generic_callback_t state = PLACER_GENERIC_CALLBACK_INITIALIZER;
    int ii = 0;

    if (!Verbose) {
        state.fp = (FILE *)0;
    }

    for (ii = 0; ii < countof(SQL); ii++) {

        if (Verbose) {
            fputs(SQL[ii], state.fp);
            fputc('\n', state.fp);
        }

        sp = placer_prepare(db, SQL[ii]);
        if (sp == (sqlite3_stmt *)0) {
            xc = -110;
            break;
        }

        rc = placer_steps(sp, placer_generic_steps_callback, &state);
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
 * the survey, and the inode number reused. We make that less likely by
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
    static const char SELECT[] = "SELECT * FROM Path WHERE (ino = ?) AND (devmajor = ?) AND (devminor = ?) AND (nlink > 1);";
    int ii = 0;
   
    do {

        if (statp->st_nlink <= 1) {
            break;
        }

        db = (sqlite3 *)vp;

        if (Verbose) {
            fputs(path, stdout);
            fputc('\n', stdout);
        }

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
 * TEST3, TEST7
 *
 * For every file in the file system, enumerate the number of entries
 * for the same path in the DB. Something is amiss if that number if
 * greater than one. But it could also be zero, if the file has been
 * deleted since the survey was done.
 */

static int enumerate(void * vp, const char * name, const char * path, size_t depth, const struct stat * statp)
{
    int xc = 0;
    sqlite3 * db = (sqlite3 *)0;
    sqlite3_stmt * sp = (sqlite3_stmt *)0;
    static const char SELECT[] = "SELECT * FROM Path WHERE (path = ?);";
    int rc = 0;
    struct Path data[2];
    struct Path * pointers[] = { &data[0], &data[1], (struct Path *)0 };
    struct Path ** current = &pointers[0];
 
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

        rc = placer_steps(sp, placer_struct_Path_steps_callback, &current);
        if (rc != SQLITE_OK) {
            xc = -132;
            break;
        }

        if (current == &pointers[0]) {
            fprintf(stdout, "NONE: \"%s\"\n", path);
        } else if (current == &pointers[1]) {
            fprintf(stdout, "ONLY: \"%s\"\n", path);
            placer_struct_Path_display(stdout, &data[0]);
        } else if (current == &pointers[2]) {
            fprintf(stdout, "MANY: \"%s\"\n", path);
            placer_struct_Path_display(stdout, &data[0]);
            placer_struct_Path_display(stdout, &data[1]);
        } else {
            diminuto_core_fatal();
        }

    } while (0);

    return xc;
}

/*
 * TEST1, TEST2
 *
 * Find all files in the survey which have the specified type and
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
    static const char SELECT[] = "SELECT * FROM Path WHERE (type = '%s') AND (mode > %u);";

    do {

        if (!Verbose) {
            state.fp = (FILE *)0;
        }

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

        rc = placer_steps(sp, placer_generic_steps_callback, &state);
        if (rc != SQLITE_OK) {
            xc = -142;
            break;
        }

    } while (0);

    return xc;
}

/*
 * Update a survey by walking the file system and storing the absolute
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
#include "SchemaPath.h"
#include "com/diag/placer/placer_end.h"
    struct Path schema;

    if (!Verbose) {
        state.fp = (FILE *)0;
    } else {
        fputs(path, stdout);
        fputc('\n', stdout);
    }

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

        sp = placer_prepare(db, PLACER_struct_Path_SQL_REPLACE);
        if (sp == (sqlite3_stmt *)0) {
            xc = -150;
            break;
        }

        rc = placer_struct_Path_stmt_bind(sp, &schema);
        if (rc != SQLITE_OK) {
            xc = -151;
            break;
        }

        rc = placer_steps(sp, placer_generic_steps_callback, &state);
        if (rc != SQLITE_OK) {
            xc = -152;
            break;
        }

    } while (0);

    return xc;
}


/*
 * Perform a survey by walking the file system and storing the absolute
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
#include "SchemaPath.h"
#include "com/diag/placer/placer_end.h"
    struct Path schema;

    if (!Verbose) {
        state.fp = (FILE *)0;
    } else {
        fputs(path, stdout);
        fputc('\n', stdout);
    }

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

        sp = placer_prepare(db, PLACER_struct_Path_SQL_INSERT);
        if (sp == (sqlite3_stmt *)0) {
            xc = -160;
            break;
        }

        rc = placer_struct_Path_stmt_bind(sp, &schema);
        if (rc != SQLITE_OK) {
            xc = -161;
            break;
        }

        rc = placer_steps(sp, placer_generic_steps_callback, &state);
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
    static const char SELECT[] = "SELECT * FROM Path;";

    do {

        if (!Verbose) {
            state.fp = (FILE *)0;
        }

        sp = placer_prepare(db, SELECT);
        if (sp == (sqlite3_stmt *)0) {
            xc = -170;
            break;
        }

        rc = placer_steps(sp, placer_generic_steps_callback, &state);
        if (rc != SQLITE_OK) {
            xc = -171;
            break;
        }

        fprintf(stdout, "count=%zu\n", state.count);

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
#include "com/diag/placer/placer_sql_create.h"
#include "SchemaPath.h"
#include "com/diag/placer/placer_end.h"
#include "com/diag/placer/placer_sql_create.h"
#include "SchemaNode.h"
#include "com/diag/placer/placer_end.h"
    sqlite3_stmt * sp = (sqlite3_stmt *)0;

    do {

        sp = placer_prepare(db, PLACER_struct_Path_SQL_CREATE);
        if (sp == (sqlite3_stmt *)0) {
            xc = -180;
            break;
        }

        rc = placer_steps(sp, (placer_steps_callback_t *)0, (void *)0);
        if (rc != SQLITE_OK) {
            xc = -181;
            break;
        }

fprintf(stderr, "CREATE=\"%s\"\n", PLACER_struct_Node_SQL_CREATE);

        sp = placer_prepare(db, PLACER_struct_Node_SQL_CREATE);
        if (sp == (sqlite3_stmt *)0) {
            xc = -182;
            break;
        }

        rc = placer_steps(sp, (placer_steps_callback_t *)0, (void *)0);
        if (rc != SQLITE_OK) {
            xc = -183;
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
    char * database = (char *)0;
    char * path = (char *)0;
    char * here = (char *)0;
    ino_t ino = 1;
    int creating = 0;
    int test0 = 0;
    int test1 = 0;
    int test2 = 0;
    int test3 = 0;
    int test4 = 0;
    int test5 = 0;
    int test6 = 0;
    int test7 = 0;
    int test8 = 0;
    int test9 = 0;
    char * end = (char *)0;
    int opt = 0;
    extern char * optarg;
    extern int optind;
    extern int opterr;
    extern int optopt;

    do {

        Program = ((Program = strrchr(argv[0], '/')) == (const char *)0) ? argv[0] : Program + 1;

        cp = insert;

        while ((opt = getopt(argc, argv, "?0123456789B:D:I:P:cdirv")) >= 0) {

            static const char USAGE[] = "[ -? ] -D DATABASE [ -B BLOCKSIZE ] [ -d ] [ -v ] [ -0 ] [ -1 ] [ -2 ] [ -3 ] [ -4 ] [ -5 ] [ -6 ] [ -7 ] [ -8 ] [ -9 ] [ -P PATH ] [ -I INODE ] [ [ -c ]  [ -i | -r ]  ROOT [ ROOT ... ] ]\n";

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
            case '8':
                test8 = !0;
                break;
            case '9':
                test9 = !0;
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
            case 'I':
                ino = strtoul(optarg, &end, 0);
                if ((end != (char *)0) && (end[0] != '\0')) {
                    errno = EINVAL;
                    perror(optarg);
                    xc = -18;
                }
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
            case 'i':
                cp = insert;
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
            xc = -19;
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
         * Save the directory path.
         */

        Directory = database;
        here = strrchr(Directory, '/');
        if (here != (const char *)0) {
            *here = '\0';
        } else {
            Directory = ".";
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

        if (!test8) {
            /* Do nothing. */
        } else if ((rc = reveal(db, ino)) == 0) {
            /* Do nothing. */
        } else {
            xc = rc;
        }

        if (!test9) {
            /* Do nothing. */
        } else if ((rc = derive(db)) == 0) {
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
