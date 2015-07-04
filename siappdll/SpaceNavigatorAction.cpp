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
#include "SpaceNavigatorAction.h"

namespace SpaceNavigatorEmulator
{
	SpaceNavigatorAction::Action SpaceNavigatorAction::parseAction(std::string actionName)
	{
		if (actionName == "PAN_X"){
			return PAN_X;
		} else if (actionName == "PAN_Y") {
			return PAN_Y;
		} else if (actionName == "PAN_Z"){
			return PAN_Z;
		} else if (actionName == "ROTATE_AROUND_X"){
			return ROTATE_AROUND_X;
		} else if (actionName == "ROTATE_AROUND_Y"){
			return ROTATE_AROUND_Y;
		} else if (actionName == "ROTATE_AROUND_Z"){
			return ROTATE_AROUND_Z;
		} else if (actionName == "BUTTON_MODE_SWAP"){
			return BUTTON_MODE_SWAP;
		} else if (actionName == "BUTTON_1"){
			return BUTTON_1;
		} else if (actionName == "BUTTON_2"){
			return BUTTON_2;
		}else if (actionName == "BUTTON_3"){
			return BUTTON_3;
		}else if (actionName == "BUTTON_4"){
			return BUTTON_4;
		}
		throw std::exception((std::string("Could not map exception ") + actionName).c_str());
	}	
}