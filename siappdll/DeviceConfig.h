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

#include <set>
#include <string>
#include "ActionMap.h"
#include "ModeReplacement.h"

namespace SpaceNavigatorEmulator
{
	class DeviceConfig
	{
	public:

		std::string deviceName;
		std::set<ActionMap> actions;
		std::set<ModeReplacement> modeReplacements;

		void load(std::string filename);
		void save(std::string filename);

		long GetDeadZone() const
		{
			return deadzone;
		}

		bool GetFakeMouseMoveWhenRotating() const
		{
			return fakeMouseMoveWhenRotating;
		}

		long GetAxisRange() const
		{
			return axisRange;
		}

	private:

		int axisRange = 350;

		bool fakeMouseMoveWhenRotating;

		long deadzone;
	};
}