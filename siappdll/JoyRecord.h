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
#include "src\sidef.h"

namespace SpaceNavigatorEmulator
{
	class JoyRecord
	{
	public:

		long x = 0;
		long y = 0;
		long z = 0;
		long rx = 0;
		long ry = 0;
		long rz = 0;

		SiButtonData buttonData;
		
		bool operator == (const JoyRecord& other)
		{
			return x == other.x &&
				y == other.y &&
				z == other.z &&
				rx == other.rx &&
				ry == other.ry &&
				rz == other.rz &&
				buttonData.current == other.buttonData.current;
		}

		bool operator != (const JoyRecord& other)
		{
			return !(*this == other);
		}

		bool IsCentered()
		{
			return x == 0 &&
				y == 0 &&
				z == 0 &&
				rx == 0 &&
				ry == 0 &&
				rz == 0;
		}
		
		bool IsRotation()
		{
			return rx != 0 || ry != 0 || rz != 0;
		}

		void log();

		JoyRecord();
		~JoyRecord();
	};
}

