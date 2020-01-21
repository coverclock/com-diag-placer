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
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <errno.h>
#include <assert.h>

static const int DEBUG = 0;

static int walk(FILE * fp, const char * name, char * path, size_t total, size_t depth)
{
    int fc = 0;
    DIR * dp = (DIR *)0;
    struct dirent * ep = (struct dirent *)0;;
    int rc = 0;
    size_t dd = 0;
    size_t prior = 0;
    size_t length = 0;

    if (depth == 0) {
        path[0] = '\0';
        path[PATH_MAX - 1] = '\0';
        total = 0;
    }

    length = strnlen(name, PATH_MAX);
    if ((total + 1 /* '/' */ + length + 1 /* '\0' */) > PATH_MAX) {
        errno = E2BIG;
        perror(path);
        return -1;
    }

    if (DEBUG) { fprintf(stderr, "%s@%d: \"%s\" [%zu] \"%s\" [%zu]\n", __FILE__, __LINE__, path, total, name, length); }

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

    if (DEBUG) { fprintf(stderr, "%s@%d: \"%s\" [%zu]\n", __FILE__, __LINE__, path, total); }

    for (dd = 0; dd < depth; ++dd) {
        fputc(' ', fp);
    }
    fputs(name, fp);
    
    dp = opendir(path);
    if (dp == (DIR *)0) {

        fputc('\n', fp);

    } else {

        fputs("/\n", fp);

        depth += 1;

        while (!0) {

            errno = 0;
            if ((ep = readdir(dp)) != (struct dirent *)0) {
                /* Do nothing. */
            } else if (errno == 0) {
                break;
            } else {
                perror("readdir");
                break;
            }

            if (strcmp(ep->d_name, "..") == 0) {
                /* Do ntohing. */
            } else if (strcmp(ep->d_name, ".") == 0) {
                /* Do ntohing. */
            } else if ((rc = walk(fp, ep->d_name, path, total, depth)) == 0) {
                /* Do ntohing. */
            } else {
                fc = rc;
            }

        }

        if (closedir(dp) < 0) {
            perror("closedir");
            fc = -4;
        }

    }

    path[prior] = '\0';
    if (DEBUG) { fprintf(stderr, "%s@%d: \"%s\" [%zu]\n", __FILE__, __LINE__, path, prior); }

    return fc;
}

int main(int argc, char * argv[])
{
    int xc = 0;
    int rc = 0;
    int ii = 0;
    char path[PATH_MAX] = { '\0', };

    if (argc <= 1) {
        xc = walk(stdout, ".", path, 0, 0);
    } else {
        for (ii = 1; ii < argc; ++ii) {
            rc = walk(stdout, argv[ii], path, 0, 0);
            if (xc == 0) {
                xc = rc;
            }
        }
    }

    return (xc < 0) ? -xc : xc;
}
