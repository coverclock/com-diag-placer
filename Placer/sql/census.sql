/* vi: set ts=4 expandtab shiftwidth=4: */
/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in LICENSE.txt<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 */
CREATE TABLE census (
    path TEXT PRIMARY KEY,
    type TEXT,
    nlink INTEGER,
    uid INTEGER,
    gid INTEGER,
    mode INTEGER,
    ino INTEGER,
    size INTEGER,
    blocks INTEGER,
    rdevmajor INTEGER,
    rdevminor INTEGER,
    devmajor INTEGER,
    devminor INTEGER,
    ctime BLOB,
    mark INTEGER
);
