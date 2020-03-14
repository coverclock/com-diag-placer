# com-diag-placer

Musings with SQLite,  X-Macros, and UTF-16 (A WORK IN PROGRESS)

# Copyright

Copyright 2020 by the Digital Aggregates Corporation, Colorado, USA.

# License

Licensed under the terms in LICENSE.txt. 

# Contact

Chip Overclock    
Digital Aggregates Corporation    
3440 Youngfield Street, Suite 209    
Wheat Ridge CO 80033 USA    
<http://www.diag.com>    
<mailto:coverclock@diag.com>    

# Abstract

Placer is a small collection of helper and convenience functions to assist
in the use of SQLite in the C programming language.  It is my excuse to
re-learn SQLite after having been away from it for a few years. It has
also given me the opportunity to revisit X-Macros which I've used in C
and C++ based projects in the past (where we referred to them - not
entirely incorrectly - as "magic macros"). I've applied them here to
generate C code to partially automate  a database schema.

SQLite is a server-less relational database system that is featured in
Android and Apple phones and tablets, Windows 10 (so it is rumored),
and countless embedded system including some of which I have helped
develop. I am not a database person, but I very much appreciate the
persistent data management model offered by SQLite that is usable both
from within applications and the command line.

# Remarks

This library and its applications support the following SQLite3 data
types: BLOB, INTEGER, INTEGER64, FLOAT, TEXT (UTF-8), TEXT16 (UTF-16).

SQLite3 provides two basic approaches to executing SQL statements in its
API: the simpler sqlite3_exec(), which is a purely text-based interface
(referred to here as "exec"), and sqlite3_prepare()/sqlite3_step(),
which is a more flexible, iterative interative (referred to here as
"steps"). Placer supports both, and I recommend the steps approach.

Placer uses so called "x-macros" that allow the developer to describe
a database schema: a row with fields, each with a specific data type,
and one of which is a primary key. The same schema description can then
be used to generate a C structure with appropriately typed fields to
hold the data from a database row, to generate C code describing SQL
commands, to generate C functions for the binding of structure fields
to SQL commands like INSERT and REPLACE, and C functions to import data
from SQL operations like SELECT.
    
# Targets

"Rhodium"    
Raspberry Pi 4 Model B    
ARMv8 64-bit    
Broadcom BCM2711 Cortex-A72 ARMv8 @ 1.5GHz x 4    
Raspbian 10 "Buster"    
Linux 4.19.93    
gcc 8.3.0    
sqlite 3.30.1    

"Nickel"    
Intel NUC5i7RYH    
Intel x86_64    
Intel Core i7-5557U @ 3.1GHz x 2 x 2    
Ubuntu 16.04.4 "xenial"    
Linux 4.13.0     
gcc 5.4.0    
sqlite 3.22.0    

# Notes

On Rhodium, I installed SQLite3 3.30 manually by downloading and
building it and installing the build artifacts in the default root
of /usr/local. On Nickel, I just installed the Debian sqlite3 and
libsqlite3-dev 3.22 packages which places the binary, header, and library
in the usual locations. But the same Makefile rules worked for me in
either case, since on Nickel the build found the necessary components
in the standard locations. Your mileage may vary.

ARMv8 valgrind(1) seems to be broken in the Debian package that installs
on Rhodium. I've been successfully running it instead on Nickel, even
though most of my development remains on Rhodium. This has caused me to
miss one pointer off by one bug while developing on the Raspberry Pi.

I've never had to personally deal that much with internationalization,
Unicode, and the mysteries of UTF-8 (more or less standard ASCII,
although multi-byte sequences are possible) versus UTF-16 (supported
directly in Java and used in many other database systems) versus UTF-32
(supported directly in the GNU C compiler and library as L"" literals
and the wchar_t type), although I've worked in lots of systems that had
to support it for business reasons. But here I am. SQLite3 supports both
UTF-8 and UTF-16. I've written some code for this project to do basic
TEXT16 stuff as implemented by SQLite3, but I haven't yet given up on
using existing libraries like libunistring which GNU provides.

# Dependencies

<https://github.com/coverclock/com-diag-placer>

<https://github.com/coverclock/com-diag-diminuto>

<https://www.sqlite.org/2020/sqlite-autoconf-3310100.tar.gz>

# Build

Install SQLite 3 (or build from source).

    sudo apt-get install sqlite3
    sudo apt-get install libsqlite3-dev

Clone and build Diminuto.

    cd ~
    mkdir -p src
    cd src
    git clone https://github.com/coverclock/com-diag-diminuto
    cd com-diag-diminuto/Diminuto
    make pristine
    make depend
    make all

Clone and build Placer. (If you don't build Diminuto where the Placer Makefile
expects it, some minor Makefile hacking might be required.)

    cd ~
    mkdir -p src
    cd src
    git clone https://github.com/coverclock/com-diag-placer
    cd com-diag-hazer/Placer
    make pristine
    make depend
    make all

# Unit Tests

Run the unit test.

    cd ~/src/com-diag-placer/Placer
    . out/host/bin/setup
    make sanity

# Functional Tests

There are two functional tests, census and survey, and each have a shell
script, census-suite and survey-suite, that runs a suite of tests using
them.  The census functional test program uses the simpler text-based
sqlite3_exec() approach. The survey functional test program uses the
more flexible sqlite3_steps() approach. Both functional tests implement
command line options that select a variety of subtests to run based on
one or more tables built by the functional test as it walks the file
system starting at a specified location.

    cd ~/src/com-diag-placer/Placer
    . out/host/bin/setup
    make functional

## census-suite

1. census -D CENSUSDATABASE -c -i -v ROOT # Create the database CENSUSDATABASE with table Census. Walk the file system starting at ROOT. Insert paths found into the database in table Census. Be verbose.
2. census -D CENSUSDATABASE -v -5 # Mark every entry in the database.
3. census -D CENSUSDATABASE -r -v ROOT # Walk the file system starting at ROOT. Replace or insert paths found into the database.
4. census -D CENSUSDATABASE -v -6 # Remove every marked entry in the database.
5. census -D CENSUSDATABASE -v -0 # Display every path entry in the database.

## survey-suite

1. survey -D SURVEYDATABASE -c -i -v ROOT # Create the database SURVEYDATABASE with tables Path and Node. Walk the file system starting at ROOT. Insert paths found into the database in table Path. Be verbose.
2. survey -D SURVEYDATABASE -v -5 # Mark every entry in the database.
3. survey -D SURVEYDATABASE -r -v ROOT # Walk the file system starting at ROOT. Replace or insert paths found into the database.
4. survey -D SURVEYDATABASE -v -6 # Remove every marked entry in the database.
5. survey -D SURVEYDATABASE -v -0 # Display every path entry in the database.
6. survey -D SURVEYDATABASE -v -9 # For every path entry in the database write a node entry in table Node.
7. survey -D SURVEYDATABASE -7 -P PATH # Display entry for PATH in the Path table.
8. survey -D SURVEYDATABASE -8 -I INODE # Display entry for INODE in the Path or Node tables.

# Schema Operators

* PLACER_SCHEMA(_STRUCTURE_) - The front matter for a schema named _STRUCTURE_.
* PLACER_BLOB(_NAME_, _ITEMS_) - A field of type BLOB, named _NAME_, of _ITEMS_ bytes in length.
* PLACER_FLOAT(_NAME_) - A field of type FLOAT, named _NAME_.
* PLACER_INTEGER(_NAME_) - A field of type INTEGER, named _NAME_.
* PLACER_INTEGER64(_NAME_) - A field of type INTEGER64, named _NAME_.
* PLACER_TEXT(_NAME_, _ITEMS_) - A field of type TEXT, named _NAME_, of _ITEMS_ UTF-8 characters in length.
* PLACER_TEXT16(_NAME_, _ITEMS_) - A field of type TEXT16, named _NAME_, of _ITEMS_ UTF-16 characters in length.
* PLACER_FIELD(_CONSTRAINTS_) - The ending of any field that is not the last field, adding _CONSTRAINTS_ constraints.
* PLACER_FINAL(_CONSTRAINTS_) - The ending of any field that is the last field, adding _CONSTRAINTS_ constraints.
* PLACER_END(_CONSTRAINTS_) - The end matter for the schema, adding _CONSTRAINTS_ contraints.

# Schema Example

    PLACER_SCHEMA(Schema)
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

# Code Generators

* placer_field_name - Generates an array of field names for a schema.
* placer_field_sizeof - Generates an array of field sizes for a schema.
* placer_field_type - Generates an array of field SQLite3 types for a schema.
* placer_exec_callback - Generates an exec callback function for a schema.
* placer_exec_prototype - Generates a prototype for an exec callback function.
* placer_null - Generates no code for a schema.
* placer_sql_create - Generates a CREATE SQL command for a schema.
* placer_sql_insert - Generates an INSERT SQL command for a schema.
* placer_sql_replace - Generates a REPLACE SQL command for a schema.
* placer_steps_callback - Generates a steps callback function for a schema.
* placer_steps_prototype - Generates a prototype for a steps callback function.
* placer_stmt_bind - Generates a statement bind function for a schema.
* placer_stmt_prototype - Generates a prototype for a statement bind function.
* placer_structure_definition - Generates a C structure definition for a schema.
* placer_structure_display - Generates a display function for a schema.
* placer_structure_initializer - Generates a C structure initializer for a schema.
* placer_structure_name- Generates a variable containing the schema name.
* placer_structure_prototype - Generates a prototype for a display function.

# Directories
 
* bin - utility source files.
* cfg - configuration makefiles.
* fun - functional test source files (may require special hardware).
* inc - public header files.
* out - build artifacts.
* sql - Structured Query Language scripts.
* src - feature implementation and private header source files.
* tst - unit test source files.

# Artifacts

* out/host/sym - binary executables with symbols intact.
* out/host/gen - generated source code of executable programs.
* out/host/fun - executable functional tests.
* out/host/bin - executable utilities.
* out/host/inc/com/diag/placer - generated header files.
* out/host/doc - generated documentation.
* out/host/doc/man/man3 - generated section 3 man pages.
* out/host/doc/html - generated HTML documentation.
* out/host/doc/pdf - generated PDF documentation.
* out/host/doc/latex - generated LaTEX files.
* out/host/tst - executable unit tests.
* out/host/lib - compiled shared object files.
* out/host/log - log files.
* out/host/obc/fun - compiled object files of functional tests.
* out/host/obc/tst - compiled object files of unit tests.
* out/host/obc/src - compiled object files of source components.
* out/host/arc - compiled archive files.
* out/host/sql - test databases.
* out/host/dep - header dependency make include files.

# References

Grant Allen, Mike Owens, *The Definitive Guide to SQLite*, 2nd ed.,
Apress, 2010

Richard Hipp at al., "Why SQLite succeeded as a database", *The Changelog*, 2016-04-30,
<https://changelog.com/podcast/201>

P. Hoffman, F. Yergeau, "UTF-16, an encoding of ISO 10646", *RFC 2781*, 2000-02,
<https://www.rfc-editor.org/rfc/pdfrfc/rfc2781.txt.pdf>

Jay A. Kreibich, *Using SQLite*, O'Reilly, 2010

Anthony Molinaro, *SQL Cookbook*, O'Reilly, 2006

SQLite Documentation, <https://www.sqlite.org/docs.html>

SQLite Tutorial, <https://www.sqlitetutorial.net>

sqlite3.h (embedded comments), <https://www.sqlite.org/cgi/src/artifact/572ea78b08ee9052>

Wikipedia, "X-Macro", <https://en.wikipedia.org/wiki/X_Macro>

Wikibooks, "C Programming/Preprocessor directives and macros",
<https://en.wikibooks.org/wiki/C_Programming/Preprocessor_directives_and_macros#X-Macros>

Wikibooks, "C Programming/Serialization",
<https://en.wikibooks.org/wiki/C_Programming/Serialization>

F. Yergeau, "UTF-8, a transformation format of ISO 10646", *RFC 3629*, 2003-11,
<https://www.rfc-editor.org/rfc/pdfrfc/rfc3629.txt.pdf>

# Articles

Chip Overclock, "When Learning By Doing Goes To Eleven", 2020-03,
<https://coverclock.blogspot.com/2020/03/when-learning-by-doing-goes-to-eleven.html>

# Soundtrack

<https://www.youtube.com/playlist?list=PLd7Yo1333iA8enrRacXyINR3oUbKBWDSH>
