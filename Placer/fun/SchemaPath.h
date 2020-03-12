/*
 * Path schema describes a DB row that is indexed by the unique path
 * name of the entry in the file system. Multiple paths may reference
 * the same inode, and hence the same physical file, But the absolute
 * fully qualified path name is itself unique.
 */

#include <limits.h>

PLACER_SCHEMA(Path)
PLACER_TEXT(path, PATH_MAX) PLACER_FIELD(PRIMARY KEY)
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
PLACER_INTEGER(mark) PLACER_FINAL()
PLACER_END()
