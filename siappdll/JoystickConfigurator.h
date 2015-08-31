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

#include "DeviceConfig.h"
#include <dinput.h>
#include <set>

namespace SpaceNavigatorEmulator
{
	class JoystickConfigurator
	{
	public:
		JoystickConfigurator();
		~JoystickConfigurator();

		void openConfiguration();
		
		JoystickConfigurator(const JoystickConfigurator&) = delete;
		JoystickConfigurator& operator=(const JoystickConfigurator&) = delete;

		std::set<ModeReplacement> GetModeReplacements() const
		{
			return config.modeReplacements;
		}

		std::set<ActionMap> GetActions() const
		{
			return config.actions;
		}

		std::set<KeySenderConfig> GetKeySenders() const
		{
			return config.keySenders;
		}

		long GetDeadZone() const
		{
			return config.GetDeadZone();
		}

		long GetAxisRange() const
		{
			return config.GetAxisRange();
		}

		bool GetFakeMouseMoveWhenRotating() const
		{
			return config.GetFakeMouseMoveWhenRotating();
		}

	private:
		DeviceConfig config;
	};
}