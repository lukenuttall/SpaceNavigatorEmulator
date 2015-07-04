siappdll replacement library
----------------------------
This subproject implements the client library which is loaded dynamically by
all programs using the 3Dconnexion API (siappdll.dll). It implements all the
functions provided by the original DLL by passing requests over TCP/IP to
the 3dxemu_win32 driver/server.

Copyright (C) 2014  John Tsiombikas <nuclear@member.fsf.org>

This program is free software, feel free to use, modify, copy, and redistribute
it with or without modifications, under the terms of the GNU General Public
License version 3 (or at your option any later version published by the Free
Software Foundation). See COPYING.txt for details.
