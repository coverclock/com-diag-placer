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
#include "com/diag/diminuto/diminuto_fd.h"
#include "com/diag/placer/placer.h"

static const char * Program = (const char *)0;
static int Debug = 0;
static int Verbose = 0;
static size_t Buffersize = 256;

static int enumerator(void * vp, int ncols, char ** value, char ** keyword)
{
    int * ip = (int *)0;

    ip = (int *)vp;
    (*ip) += 1;

    return SQLITE_OK;
}

static int enumerate(sqlite3 * db, const char * name, char * path, size_t total, size_t depth)
{
    int xc = 0;
    DIR * dp = (DIR *)0;
    struct dirent * ep = (struct dirent *)0;
    struct stat status = { 0 };
    int rc = 0;
    size_t dd = 0;
    size_t prior = 0;
    size_t length = 0;
    char * to = (char *)0;
    char * sql = (char *)0;
    char * sqlmessage = (char *)0;
    int enumeration = 0;
   
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
     * string for an actual file could not be represented.
     */

    length = strnlen(name, PATH_MAX);

    do {

        if ((total + 1 /* '/' */ + length + 1 /* '\0' */) > PATH_MAX) {
            errno = E2BIG;
            perror(path);
            xc = -10;
            break;
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
         * N.B. It is *not* sufficient to just use the opendir(3)
         * success to determine whether the path points to a
         * directory; it will succeeed for soft links as well.
         * Folling such soft links that point to directories can
         * result in a file system loop, where the soft link
         * ultimately points to the directory containing the
         * soft link.
         */

        rc = lstat(path, &status);
        if (rc >= 0) {
            /* Do nothing. */
        } else if ((errno == EACCES) || (errno == ENOENT))  {
            perror(path);
            break;
        } else {
            perror(path);
            xc = -11;
            break;
        }

        /*
         * Expand any single quotes in path.
         */

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

        /*
         * If a flat file, we're done; if a directory, recurse and descend.
         */

        if (S_ISDIR(status.st_mode)) {

            dp = opendir(path);
            if (dp != (DIR *)0) {
                /* Do nothing. */
            } else if ((errno == EACCES) || (errno == ENOENT))  {
                perror(path);
                break;
            } else {
                perror(path);
                xc = -14;
                break;
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
                    xc = -15;
                    break;
                }

                if (strcmp(ep->d_name, "..") == 0) {
                    /* Do ntohing. */
                } else if (strcmp(ep->d_name, ".") == 0) {
                    /* Do ntohing. */
                } else if ((rc = enumerate(db, ep->d_name, path, total, depth)) == 0) {
                    /* Do ntohing. */
                } else {
                    xc = rc;
                    break;
                }

            }

            if (closedir(dp) < 0) {
                perror(path);
                xc = -16;
                break;
            }

        }

    } while (0);

    if (xc == 0) {
        path[prior] = '\0';
    }

    return xc;
}

static int extract(sqlite3 * db, diminuto_fd_type_t type)
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

static int insert(sqlite3 * db, const char * name, char * path, size_t total, size_t depth)
{
    int xc = 0;
    DIR * dp = (DIR *)0;
    struct dirent * ep = (struct dirent *)0;
    struct stat status = { 0 };
    int rc = 0;
    size_t dd = 0;
    size_t prior = 0;
    size_t length = 0;
    char * to = (char *)0;
    char * sql = (char *)0;
    char * sqlmessage = (char *)0;
   
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
     * string for an actual file could not be represented.
     */

    length = strnlen(name, PATH_MAX);

    do {

        if ((total + 1 /* '/' */ + length + 1 /* '\0' */) > PATH_MAX) {
            errno = E2BIG;
            perror(path);
            xc = -10;
            break;
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
            break;
        } else {
            perror(path);
            xc = -11;
            break;
        }

        /*
         * Expand any single quotes in path.
         */

        to = placer_expand_alloc(path);

        /*
         * Insert the row into the database.
         */

        sql = placer_format_alloc(Buffersize,
            "INSERT INTO census VALUES ('%s', '%c', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)"
            , to
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

        /*
         * If a flat file, we're done; if a directory, recurse and descend.
         */

        if (S_ISDIR(status.st_mode)) {

            dp = opendir(path);
            if (dp != (DIR *)0) {
                /* Do nothing. */
            } else if ((errno == EACCES) || (errno == ENOENT))  {
                perror(path);
                break;
            } else {
                perror(path);
                xc = -14;
                break;
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
                    xc = -15;
                    break;
                }

                if (strcmp(ep->d_name, "..") == 0) {
                    /* Do ntohing. */
                } else if (strcmp(ep->d_name, ".") == 0) {
                    /* Do ntohing. */
                } else if ((rc = insert(db, ep->d_name, path, total, depth)) == 0) {
                    /* Do ntohing. */
                } else {
                    xc = rc;
                    break;
                }

            }

            if (closedir(dp) < 0) {
                perror(path);
                xc = -16;
                break;
            }

        }

    } while (0);

    if (xc == 0) {
        path[prior] = '\0';
    }

    return xc;
}

int main(int argc, char * argv[])
{
    int xc = 0;
    int rc = 0;
    int ii = 0;
    char path[PATH_MAX] = { '\0', };
    char effective[PATH_MAX] = { '\0', };
    sqlite3 * db = (sqlite3 *)0;
    int opt = 0;
    const char * database = (const char *)0;
    int test1 = 0;
    int test2 = 0;
    int test3 = 0;
    char * end = (char *)0;
    static const char USAGE[] = "-D DATABASE [ -B BLOCKSIZE ] [ -1 ] [ -2 ] [ -3 ] [ ROOT [ ROOT ... ] ]\n";
    extern char * optarg;
    extern int optind;
    extern int opterr;
    extern int optopt;

    do {

        Program = ((Program = strrchr(argv[0], '/')) == (const char *)0) ? argv[0] : Program + 1;

        while ((opt = getopt(argc, argv, "?123B:D:dv")) >= 0) {
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
            if (realpath(argv[optind], effective) == (char *)0) {
                perror(argv[optind]);
                xc = 4;
                break;
            } else if ((rc = insert(db, effective, path, 0, 0)) != 0) {
                xc = rc;
                break;
            } else {
                /* Do nothing. */
            }
        }

        if (!test1) {
            /* Do nothing. */
        } else if ((rc = extract(db, DIMINUTO_FD_TYPE_FILE)) == 0) {
            /* Do nothing. */
        } else {
            xc = 5;
        }

        if (!test2) {
            /* Do nothing. */
        } else if ((rc = extract(db, DIMINUTO_FD_TYPE_DIRECTORY)) == 0) {
            /* Do nothing. */
        } else {
            xc = 6;
        }

        if (!test3) {
            /* Do nothing. */
        } else if ((rc = enumerate(db, "/", path, 0, 0)) == 0) {
            /* Do nothing. */
        } else {
            xc = 7;
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
