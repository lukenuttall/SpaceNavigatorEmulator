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
#include <Windows.h>
#include "Timer.h"
#include "Joystick.h"
#include "IMode.h"
#include <mutex>

namespace SpaceNavigatorEmulator
{
	class SpaceNavConnector
	{
	public:
		SpaceNavConnector(UINT spaceNavigatorWindowsMessage);
		~SpaceNavConnector();

		void Open(HWND window);

		void Close();

		SpaceNavConnector() = delete;
		SpaceNavConnector(SpaceNavConnector const&) = delete;
		SpaceNavConnector& operator=(SpaceNavConnector const&) = delete;

		const JoyRecord & GetRecord();

		long getPeriodMs() const
		{
			return lastPeriodMs != 0 ? lastPeriodMs : periodMs;
		}

	private:
		void pollStick();

		void fireCallback();

		std::mutex mutex;
		
		UINT space_nav_message;

		HWND windowHandle;

		// configured timeout
		int periodMs = 16;

		long lastPeriodMs;

		// timekeeping
		LARGE_INTEGER lastMove;
		LARGE_INTEGER Frequency;			

		long GetInterval();

		std::unique_ptr<SpaceNavigatorEmulator::Joystick> stick;
		
		std::unique_ptr<SpaceNavigatorEmulator::Timer> timer;

		JoyRecord record;

		std::unique_ptr<SpaceNavigatorEmulator::IMode> mouseDragOnStartOfRotationFilter;
	};

}