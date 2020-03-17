/* vi: set ts=4 expandtab shiftwidth=4: */
/*
 * Node schema describes a DB row that is indexed by the inode
 * number. Because inode numbers are unique only within a file
 * system , storing data from across file systems is not supported.
 */

PLACER_SCHEMA(Node)
PLACER_INTEGER64(ino)       PLACER_FIELD(PRIMARY KEY)
PLACER_INTEGER64(nlink)     PLACER_FIELD()
PLACER_INTEGER(uid)         PLACER_FIELD()
PLACER_INTEGER(gid)         PLACER_FIELD()
PLACER_INTEGER(mode)        PLACER_FIELD()
PLACER_INTEGER64(blocks)    PLACER_FIELD()
PLACER_INTEGER64(size)      PLACER_FIELD()
PLACER_INTEGER(devmajor)    PLACER_FIELD()
PLACER_INTEGER(devminor)    PLACER_FIELD()
PLACER_INTEGER64(ctime)     PLACER_FINAL()
PLACER_END()
