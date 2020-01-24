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
 * USAGE
 *
 * EXAMPLES
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
#include "sqlite3.h"
#include "com/diag/diminuto/diminuto_fd.h"
#include "com/diag/placer/placer_sql.h"

static const char * Program = (const char *)0;
static int Debug = 0;
static int Verbose = 0;

static int walk(sqlite3 * db, const char * name, char * path, size_t total, size_t depth)
{
    DIR * dp = (DIR *)0;
    struct dirent * ep = (struct dirent *)0;
    struct stat status = { 0 };
    int rc = 0;
    size_t dd = 0;
    size_t prior = 0;
    size_t length = 0;
    char sqlbuffer[1024] = { '\0' };
    sqlite3_stmt * sqlstatement = (sqlite3_stmt *)0;
    const char * sqltail = (const char *)0;
    char * sqlmessage = (char *)0;
    int bytes = 0;
    char buffer[PATH_MAX * 2] = { '\0' };
    char * pp = (char *)0;
    char * bp = (char *)0;

    /*
     * If we're at the root of the tree, initialize the path buffer.
     */

    if (depth == 0) {
        path[0] = '\0';
        path[PATH_MAX - 1] = '\0';
        total = 0;
    }

    /*
     * Insure the path buffer has sufficient room. I'd be surprised
     * if this failed on a modern system, but back when MAXPATHLEN
     * was 512 I have seen file systems for which an absolute path
     * string could not be represented.
     */

    length = strnlen(name, PATH_MAX);
    if ((total + 1 /* '/' */ + length + 1 /* '\0' */) > PATH_MAX) {
        errno = E2BIG;
        perror(path);
        return -10;
    }

    /*
     * Contstruct a path (maybe be relative or absolute depending
     * on the root).
     */

    prior = total;
    if (total == 0) {
        /* Do nothing. */
    } else if (path[total - 1] == '/') {
        /* Do nothing. */
    } else {
        path[total++] = '/';
        path[total] = '\0';
    }
    strcat(path, name);
    total += length;

    /*
     * Get the attributes for the file identified by the path.
     */

    rc = lstat(path, &status);
    if (rc >= 0) {
        /* Do nothing. */
    } else if ((errno == EACCES) || (errno == ENOENT))  {
        perror(path);
        path[prior] = '\0';
        return 0;
    } else {
        perror(path);
        return -11;
    }

    /*
     * Expand any single quotes in path.
     */

    placer_sql_expand(buffer, path, sizeof(buffer), total);

    /*
     * Insert the row into the database.
     */

    bytes = snprintf(sqlbuffer, sizeof(sqlbuffer),
        "INSERT INTO census VALUES ('%s', '%c', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)"
        , buffer
        , diminuto_fd_mode2type(status.st_mode)
        , status.st_nlink
        , status.st_uid
        , status.st_gid
        , (status.st_mode & ~S_IFMT)
        , status.st_ino
        , status.st_size
        , status.st_blocks
        , major(status.st_rdev)
        , minor(status.st_rdev)
        , major(status.st_dev)
        , minor(status.st_dev)
        , status.st_ctim.tv_sec
        , status.st_ctim.tv_nsec
    );

    if ((bytes >= sizeof(sqlbuffer)) || (sqlbuffer[bytes] != '\0')) {
        sqlbuffer[sizeof(sqlbuffer) - 1] = '\0';
        errno = E2BIG;
        perror(sqlbuffer);
        return -12;
    }

    fputs(sqlbuffer, stderr);
    fputc('\n', stderr);

    rc = sqlite3_exec(db, sqlbuffer, placer_sql_callback, stderr, &sqlmessage);
    if (rc != SQLITE_OK) {
        placer_sql_error(rc);
        return -22;
    }

    /*
     * If a flat file, we're done; if a directory, recurse and descend.
     */

    if (S_ISDIR(status.st_mode)) {

        dp = opendir(path);
        if (dp != (DIR *)0) {
            /* Do nothing. */
        } else if ((errno == EACCES) || (errno == ENOENT))  {
            perror(path);
            path[prior] = '\0';
            return 0;
        } else {
            perror(path);
            return -30;
        }

        depth += 1;

        while (!0) {

            errno = 0;
            if ((ep = readdir(dp)) != (struct dirent *)0) {
                /* Do nothing. */
            } else if (errno == 0) {
                break;
            } else {
                perror(path);
                return -31;
            }

            if (strcmp(ep->d_name, "..") == 0) {
                /* Do ntohing. */
            } else if (strcmp(ep->d_name, ".") == 0) {
                /* Do ntohing. */
            } else if ((rc = walk(db, ep->d_name, path, total, depth)) == 0) {
                /* Do ntohing. */
            } else {
                return rc;
            }

        }

        if (closedir(dp) < 0) {
            perror(path);
            return -32;
        }

    }

    path[prior] = '\0';
    return 0;
}

int main(int argc, char * argv[])
{
    int xc = 0;
    int rc = 0;
    int ii = 0;
    char path[PATH_MAX] = { '\0', };
    sqlite3 * db = (sqlite3 *)0;
    int opt = 0;
    const char * database = (const char *)0;
    extern char * optarg;
    extern int optind;
    extern int opterr;
    extern int optopt;

    Program = ((Program = strrchr(argv[0], '/')) == (const char *)0) ? argv[0] : Program + 1;

    while ((opt = getopt(argc, argv, "?D:dv")) >= 0) {
        switch (opt) {
        case '?':
            fprintf(stderr, "usage: %s [ -D DATABASE ] [ ROOT [ ROOT ... ] ]\n", Program);
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
            fprintf(stderr, "usage: %s [ -? ] [ -D DATABASE ] [ ROOT [ ROOT ... ] ]\n", Program);
            return 1;
            break;
        }
    }

    if (database == (const char *)0) {
        return 1;
    }

    if ((rc = sqlite3_open(database, &db)) != SQLITE_OK) {
        placer_sql_error(rc);
        return 2;
    } else if (db == (sqlite3 *)0) {
        errno = EADDRNOTAVAIL;
        perror(database);
        return 3;
    } else {
        /* Do nothing. */
    }

    if (optind >= argc) {
        xc = walk(db, ".", path, 0, 0);
    } else {
        for (; optind < argc; ++optind) {
            if ((xc = walk(db, argv[optind], path, 0, 0)) != 0) {
                break;
            }
        }
    }

    if ((rc = sqlite3_close(db)) != SQLITE_OK) {
        placer_sql_error(rc);
        return 2;
    } else {
        db = (sqlite3 *)0;
    }

    return (xc < 0) ? -xc : xc;
}
