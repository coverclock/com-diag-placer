/* vi: set ts=4 expandtab shiftwidth=4: */

/**
 * @file
 *
 * Copyright 2020 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * https://github.com/coverclock/com-diag-placer<BR>
 * This header file undefines the X macros for a schema.
 * N.B. THIS HEADER FILE CAN BE INCLUDED MORE THAN ONCE.
 */

#if defined(PLACER_SCHEMA)
#undef PLACER_SCHEMA
#endif

#if defined(PLACER_BLOB)
#undef PLACER_BLOB
#endif

#if defined(PLACER_FLOAT)
#undef PLACER_FLOAT
#endif

#if defined(PLACER_INTEGER)
#undef PLACER_INTEGER
#endif

#if defined(PLACER_INTEGER64)
#undef PLACER_INTEGER64
#endif

#if defined(PLACER_TEXT)
#undef PLACER_TEXT
#endif

#if defined(PLACER_TEXT16)
#undef PLACER_TEXT16
#endif

#if defined(PLACER_FIELD)
#undef PLACER_FIELD
#endif

#if defined(PLACER_FINAL)
#undef PLACER_FINAL
#endif

#if defined(PLACER_END)
#undef PLACER_END
#endif
