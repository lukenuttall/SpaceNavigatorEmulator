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
#include "JoyRecord.h"
#include "Log.h"

namespace SpaceNavigatorEmulator
{
	JoyRecord::JoyRecord() 		
	{
		buttonData.current = 0;
		buttonData.last = 0;
		buttonData.pressed = 0;
		buttonData.release = 0;
	}

	JoyRecord::~JoyRecord()
	{
	}

	void JoyRecord::log()
	{
		LOG(TRACE) << "x: " << x << std::endl
			<< "y: " << y << std::endl
			<< "z: " << z << std::endl
			<< "rx: " << rx << std::endl
			<< "ry: " << ry << std::endl
			<< "rz: " << rx << std::endl; 
	}
}
