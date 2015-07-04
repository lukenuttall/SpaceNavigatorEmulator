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

#include "SpaceNavConnector.h"
#include <iostream>
#include <thread>
#include "Log.h"
#include <windowsx.h>

using namespace std;

namespace SpaceNavigatorEmulator
{
	SpaceNavConnector::SpaceNavConnector(UINT spaceNavigatorWindowsMessage)
		: space_nav_message(spaceNavigatorWindowsMessage)
	{
		QueryPerformanceFrequency(&Frequency);
	}

	SpaceNavConnector::~SpaceNavConnector()
	{
	}

	const JoyRecord & SpaceNavConnector::GetRecord()
	{
		return record;
	}

	void SpaceNavConnector::Open(HWND window)
	{
		LOG(TRACE) << "Opening space navigator";
		windowHandle = window;
		HWND topLevel = window;
		HWND current = window;
		do
		{
			current = topLevel;
			topLevel = ::GetParent(current);
		} while (topLevel != NULL);

		{
			std::lock_guard<std::mutex> lock(mutex);
			stick.reset(new Joystick(current, [this]
			{
				fireCallback();
			}));
		}

		timer.reset(new Timer(periodMs, [&] { pollStick(); }));

		timer->start();

		LOG(TRACE) << "Space Navigator opened";
	}

	void SpaceNavConnector::pollStick()
	{
		std::lock_guard<std::mutex> lock(mutex);
		if (stick)
			stick->PollStick();
	}

	void SpaceNavConnector::fireCallback()
	{
		LOG(TRACE) << "Joystick changed";
		lastPeriodMs = GetInterval();

		auto oldRecord = record;
		if (stick)
		{
			record = stick->GetRecord();
		};

		int type = SI_MOTION_EVENT;
		if (record.buttonData.current != record.buttonData.last)
		{
			type = SI_BUTTON_EVENT;
			DWORD down = record.buttonData.pressed;
			DWORD last = record.buttonData.last;
			DWORD difference = last ^ down;
			auto message = [&](DWORD buttons, int buttonMessage){
				for (unsigned int i = 0; i < 32; i++)
				{
					if ((1 << i) & buttons)
					{
						::PostMessage(windowHandle, space_nav_message, buttonMessage, i);
					}
				}
			};

			DWORD justPressed = difference & down;
			if (justPressed)
				message(justPressed, SI_BUTTON_PRESS_EVENT);

			DWORD justReleased = difference & last;
			if (justReleased)
				message(justReleased, SI_BUTTON_RELEASE_EVENT);
		}
		if (type == SI_MOTION_EVENT)
		{
			// FIXME - extract to strategy/helper class
			if (oldRecord.IsCentered() && record.IsRotation() && stick->GetFakeMouseMoveWhenRotating())
			{
				// We are going to fake a middle mouse movement
				POINT p;
				if (GetCursorPos(&p) && ScreenToClient(windowHandle, &p))
				{
					DWORD position = MAKELPARAM(p.x, p.y);
					::PostMessage(windowHandle, WM_MBUTTONDOWN, MK_MBUTTON, position);
					
					DWORD move = MAKELPARAM(p.x + 10, p.y);
					::PostMessage(windowHandle, WM_MOUSEMOVE, MK_MBUTTON, move);
					::PostMessage(windowHandle, WM_MOUSEMOVE, MK_MBUTTON, position);

					::PostMessage(windowHandle, WM_MBUTTONUP, 0, position);
				}
			}
		}
		::PostMessage(windowHandle, space_nav_message, type, 1);
	}

	long SpaceNavConnector::GetInterval()
	{
		LARGE_INTEGER current;		
		QueryPerformanceCounter(&current);

		long ellapsedMilliseconds = (long)(((current.QuadPart - lastMove.QuadPart) * 1000) / Frequency.QuadPart);

		lastMove = current;
		return ellapsedMilliseconds;
	}

	void SpaceNavConnector::Close()
	{
		LOG(TRACE) << "Closing space nav";
		std::lock_guard<std::mutex> lock(mutex);
		timer->stop();
		timer.release();
		stick.release();
	}
}