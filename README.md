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

Placer is my excuse to re-learn SQLite after having been away from it
for a few years. SQLite is a server-less database system that is featured
in Android and Apple phones and tablets, Windows 10 (so it is rumored),
and countless embedded system including some on which I have worked. I
am not a database person, but I very much appreciate the persistent data
management model offered by SQLite that is usable both from applications
and the command line.

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
in the usual locations.  But thes Makefile rules worked for me in either
case, since on Nickel the build found the necessary components in the
standard locations. Your mileage may vary.

ARMv8 valgrind(1) seems to be broken in the Debian package that installs
on Rhodium. I've been successfully running it instead on Nickel, even
though most of my development remains on Rhodium.

# References

Grant Allen, Mike Owens, *The Definitive Guide to SQLite*, 2nd ed.,
Apress, 2010

Jay A. Kreibich, *Using SQLite*, O'Reilly, 2010

# Links

SQLite Documentation, <https://www.sqlite.org/docs.html>
