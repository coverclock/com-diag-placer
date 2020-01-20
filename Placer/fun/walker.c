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

static int walk(FILE * fp, const char * path, int depth)
{
    int rc = 0;
    DIR * dp = (DIR *)0;
    struct dirent * ep = (struct dirent *)0;;
    int ii = 0;

    do {

        dp = opendir(path);
        if (dp == (DIR *)0) {
            perror(path);
            rc = -1;
            break;
        }

        while ((ep = readdir(dp)) != (struct dirent *)0) {

            if (ep->d_type != DT_DIR) {
                for (ii = 0; ii < depth; ++ii) {
                    fputc(' ', fp);
                }
                fprintf(fp, "%s\n", ep->d_name);

            } else if (strcmp(ep->d_name, "..") == 0) {
                /* Do ntohing. */
            } else if ((depth > 0) && (strcmp(ep->d_name, ".") == 0)) {
                /* Do nothing. */
            } else {
                char buffer[PATH_MAX] = { '\0', };

                for (ii = 0; ii < depth; ++ii) {
                    fputc(' ', fp);
                }
                fprintf(fp, "%s\n", ep->d_name);

                strncpy(buffer, path, sizeof(buffer));
                if (strcmp(path, "/") != 0) {
                    strncat(buffer, "/", sizeof(buffer) - strnlen(buffer, sizeof(buffer)));
                }
                strncat(buffer, ep->d_name, sizeof(buffer) - strnlen(buffer, sizeof(buffer)));

                if (buffer[sizeof(buffer) - 1] != '\0') {
                    buffer[sizeof(buffer) - 1] = '\0';
                    errno = E2BIG;
                    perror(buffer);
                    rc = -2;
                } else {
                    rc = walk(fp, buffer, depth + 1);
                }

            }

        }

    } while (0);

    if (dp == (DIR *)0) {
        /* Do nothing. */
    } else if (closedir(dp) >= 0) {
        /* Do nothing. */
    } else if (rc != 0) {
        perror(path);
    } else {
        perror(path);
        rc = -4;
    }

    return rc;
}

int main(int argc, char * argv[])
{
    int rc = 0;
    int ic = 0;
    const char * root = "/";
    int ii = 0;

    if (argc <= 1) {
        rc = walk(stdout, "/", 0);
    } else {
        for (ii = 1; ii < argc; ++ii) {
            ic = walk(stdout, argv[ii], 0);
            if (rc == 0) {
                rc = ic;
            }
        }
    }

    return (rc < 0) ? -rc : rc;
}
