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
#include <string>

namespace SpaceNavigatorEmulator
{
	class SpaceNavigatorAction
	{
	public:

		enum Action
		{
			PAN_X,
			PAN_Y,
			PAN_Z,
			ROTATE_AROUND_X,
			ROTATE_AROUND_Y,
			ROTATE_AROUND_Z,
			BUTTON_MODE_SWAP,
			BUTTON_1,
			BUTTON_2,
			BUTTON_3,
			BUTTON_4
		};

		static Action parseAction(std::string actionName);
	};
}