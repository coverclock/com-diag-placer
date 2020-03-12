/*
 * Node schema describes a DB row that is indexed by the synthetic
 * node identifier, which is a combination of the device major and
 * minor numbers and the inode number (which is only unique within
 * the context of a specific device.
 */

PLACER_SCHEMA(Node)
PLACER_TEXT(node, ((sizeof(dev_t) + sizeof(ino_t)) * 2) + 1) PLACER_FIELD(PRIMARY KEY)
PLACER_TEXT16(type, sizeof("'")) PLACER_FIELD()
PLACER_INTEGER64(nlink) PLACER_FIELD()
PLACER_INTEGER(uid) PLACER_FIELD()
PLACER_INTEGER(gid) PLACER_FIELD()
PLACER_INTEGER(mode) PLACER_FIELD()
PLACER_INTEGER64(ino) PLACER_FIELD()
PLACER_INTEGER64(blocks) PLACER_FIELD()
PLACER_INTEGER64(size) PLACER_FIELD()
PLACER_INTEGER(rdevmajor) PLACER_FIELD()
PLACER_INTEGER(rdevminor) PLACER_FIELD()
PLACER_INTEGER(devmajor) PLACER_FIELD()
PLACER_INTEGER(devminor) PLACER_FIELD()
PLACER_INTEGER64(ctime) PLACER_FIELD()
PLACER_BLOB(status, sizeof(struct stat)) PLACER_FIELD()
PLACER_END()
