/*
SpaceNavigatorEmulator - 3dconnexion-compatible siappdll.dll replacement
Copyright (C) 2015  Luke Nuttall <lukenuttall@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#define ELPP_THREAD_SAFE
#define ELPP_FORCE_USE_STD_THREAD

#include "easylogging++.h"

class Log
{
public:

	static Log& getInstance()
	{
		static Log instance;
		return instance;
	}

private:
	Log();
	
	Log(Log const&) = delete;
	void operator=(Log const&) = delete;
};
