/*
SpaceNavigatorEmulator - 3dconnexion-compatible siappdll.dll replacement
Copyright (C) 2014  John Tsiombikas <nuclear@member.fsf.org>
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
#ifndef __SIAPPDLL_IMPL_H__
#define __SIAPPDLL_IMPL_H__

#include <stdio.h>
#include "..\SpaceNavConnector.h"

extern "C" {

	struct SiHdl {
		SiOpenData opendata;
		SpaceNavigatorEmulator::SpaceNavConnector* connector;
	};

	unsigned int space_ware_message;

	bool initialised = false;
}
#endif
