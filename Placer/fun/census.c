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
 * walker walks the file system tree at the specified root or by default
 * at the current directory and adds the metadata to a database.
 *
 * USAGE
 *
 * census [ root [ root ... ] ]
 *
 * EXAMPLES
 *
 * census
 * census .
 * census /
 * census foo/bar
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

static const char * Program = (const char *)0;
static int Debug = 0;
static int Verbose = 0;

static void sqlerror(int error)
{
    fputs(sqlite3_errstr(error), stderr);
    fputc('\n', stderr);
}

static int walk(sqlite3 * db, const char * name, char * path, size_t total, size_t depth)
{
    int fc = 0;
    DIR * dp = (DIR *)0;
    struct dirent * ep = (struct dirent *)0;
    struct stat status = { 0 };
    int rc = 0;
    size_t dd = 0;
    size_t prior = 0;
    size_t length = 0;

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
        return -1;
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
    if (rc < 0) {
        perror(path);
        return -2;
    }

#if 0
struct statx_timestamp {
        __s64   tv_sec;
        __u32   tv_nsec;
        __s32   __reserved;
};
struct statx {
        /* 0x00 */
        __u32   stx_mask;       /* What results were written [uncond] */
        __u32   stx_blksize;    /* Preferred general I/O size [uncond] */
        __u64   stx_attributes; /* Flags conveying information about the file [uncond] */
        /* 0x10 */
        __u32   stx_nlink;      /* Number of hard links */
        __u32   stx_uid;        /* User ID of owner */
        __u32   stx_gid;        /* Group ID of owner */
        __u16   stx_mode;       /* File mode */
        __u16   __spare0[1];
        /* 0x20 */
        __u64   stx_ino;        /* Inode number */
        __u64   stx_size;       /* File size */
        __u64   stx_blocks;     /* Number of 512-byte blocks allocated */
        __u64   stx_attributes_mask; /* Mask to show what's supported in stx_attributes */
        /* 0x40 */
        struct statx_timestamp  stx_atime;      /* Last access time */
        struct statx_timestamp  stx_btime;      /* File creation time */
        struct statx_timestamp  stx_ctime;      /* Last attribute change time */
        struct statx_timestamp  stx_mtime;      /* Last data modification time */
        /* 0x80 */
        __u32   stx_rdev_major; /* Device ID of special file [if bdev/cdev] */
        __u32   stx_rdev_minor;
        __u32   stx_dev_major;  /* ID of device containing file [uncond] */
        __u32   stx_dev_minor;
        /* 0x90 */
        __u64   __spare2[14];   /* Spare space for future expansion */
        /* 0x100 */
};
#endif

    /*
     * HERE
     */

    /*
     * If a flat file, we're done; if a directory, recurse and descend.
     */

    if (S_ISDIR(status.st_mode)) {

        dp = opendir(path);
        if (dp == (DIR *)0) {
            perror(path);
            return -3;
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
                fc = -4;
                break;
            }

            if (strcmp(ep->d_name, "..") == 0) {
                /* Do ntohing. */
            } else if (strcmp(ep->d_name, ".") == 0) {
                /* Do ntohing. */
            } else if ((rc = walk(db, ep->d_name, path, total, depth)) == 0) {
                /* Do ntohing. */
            } else {
                fc = rc;
                break;
            }

        }

        if (closedir(dp) < 0) {
            perror(path);
            return -5;
        }

    }

    path[prior] = '\0';

    return fc;
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

    while ((opt = getopt(argc, argv, "?Ddv")) >= 0) {
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
        sqlerror(rc);
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
            rc = walk(db, argv[optind], path, 0, 0);
            if (xc == 0) {
                xc = rc;
            }
        }
    }

    if (db == (sqlite3 *)0) {
        /* Do nothing. */
    } else if ((rc = sqlite3_close(db)) != SQLITE_OK) {
        sqlerror(rc);
        return 2;
    } else {
        db = (sqlite3 *)0;
    }

    return (xc < 0) ? -xc : xc;
}
