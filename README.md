# com-diag-placer

Musings with SQLite (WORK IN PROGRESS).

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
in the use of SQLite in the C programming language.  It is also my excuse
to re-learn SQLite after having been away from it for a few years.

SQLite is a server-less relational database system that is featured in
Android and Apple phones and tablets, Windows 10 (so it is rumored),
and countless embedded system including some of which I have helped
develop. I am not a database person, but I very much appreciate the
persistent data management model offered by SQLite that is usable both
from within applications and the command line.

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
though most of my development remains on Rhodium.

# Dependencies

Placer depends on SQLite3, and on Diminuto, my C-based systems programming library.

# Repositories

<https://github.com/coverclock/com-diag-placer>

<https://github.com/coverclock/com-diag-diminuto>

# Build

Install SQLite.

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

Run the unit tests

    cd ~/src/com-diag-placer/Placer
    . out/host/bin/setup
    unittest-expand
    unittest-format
    unittest-message

Run the functional test.

    cd ~/src/com-diag-placer/Placer
    make out/host/sql/census.db
    . out/host/bin/setup
    census -D out/host/sql/census.db -v -0 /

# References

Grant Allen, Mike Owens, *The Definitive Guide to SQLite*, 2nd ed.,
Apress, 2010

Jay A. Kreibich, *Using SQLite*, O'Reilly, 2010

Anthony Molinaro, *SQL Cookbook*, O'Reilly, 2006

# Links

SQLite Documentation, <https://www.sqlite.org/docs.html>

SQLite Tutorial, <https://www.sqlitetutorial.net>

X Macro, <https://en.wikipedia.org/wiki/X_Macro>
