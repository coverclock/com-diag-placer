#include <limits.h>

PLACER_SCHEMA(Schema)
PLACER_TEXT(path, PATH_MAX) PLACER_FIELD(PRIMARY KEY)
PLACER_TEXT(type, sizeof("'")) PLACER_FIELD()
PLACER_INTEGER(nlink) PLACER_FIELD()
PLACER_INTEGER(uid) PLACER_FIELD()
PLACER_INTEGER(gid) PLACER_FIELD()
PLACER_INTEGER(mode) PLACER_FIELD()
PLACER_INTEGER(ino) PLACER_FIELD()
PLACER_INTEGER64(size) PLACER_FIELD()
PLACER_INTEGER64(blocks) PLACER_FIELD()
PLACER_INTEGER(rdevmajor) PLACER_FIELD()
PLACER_INTEGER(rdevminor) PLACER_FIELD()
PLACER_INTEGER(devmajor) PLACER_FIELD()
PLACER_INTEGER(devminor) PLACER_FIELD()
PLACER_INTEGER64(ctime) PLACER_FIELD()
PLACER_BLOB(status, sizeof(struct stat)) PLACER_FIELD()
PLACER_INTEGER(mark) PLACER_FINAL()
PLACER_END()
