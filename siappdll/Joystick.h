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

#define DIRECTINPUT_VERSION 0x0800
//#define NTDDI_VERSION NTDDI_WIN7

#include <functional>
#include <vector>
#include "PreferredJoystick.h"
#include <wrl.h>
#include <map>
#include "JoyRecord.h"
#include "SpaceNavigatorAction.h"
#include "ActionMap.h"
#include "IMode.h"
#include "ModeReplacement.h"
#include "JoystickConfigurator.h"
#include "ActionDetails.h"
#include "ButtonTester.h"

namespace SpaceNavigatorEmulator
{
	class Joystick
	{
	public:
		Joystick(HWND window, std::function<void()>&& callback);
		~Joystick();

		void PollStick();
		
		Joystick(const Joystick&) = delete;
		Joystick& operator=(const Joystick&) = delete;

		JoyRecord const & GetRecord() const;

		bool IsReady()
		{
			return ready;
		}

	private:
		// Initialises a direct input object an acquires the joystickpte
		bool Initialise();

		// Releases direct input
		void Release();

		// Build the list of joysticks that are also xbox controllers
		void FindXInputDevices();

		// Checks whether stick is really an xbox controller
		bool IsXInputDevice(const GUID* pGuidProductFromDirectInput);

		// When calling the call the function that issues this callback, we supply a this pointer as the context;
		static BOOL __stdcall StaticEnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, void* pContext);

		bool EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance);

		// When calling the call the function that issues this callback, we supply a this pointer as the context;
		static BOOL __stdcall StaticEnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, void* pContext);

		bool EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi);

		std::function<void()> changedCallback;

		LONG getAxis(SpaceNavigatorAction::Action action, DIJOYSTATE* state, bool swapsEnabled = false);

		SpaceNavigatorAction::Action Joystick::GetBufferedAction(DWORD offset);
		
		long GetButtonState(std::set<SpaceNavigatorAction::Action> actions);

		long GetCurrentButtonState(DIJOYSTATE* state);

		Microsoft::WRL::ComPtr<IDirectInput8> directInput;
		Microsoft::WRL::ComPtr<IDirectInputDevice8> stick;
		HWND window;

		std::vector<DWORD> xinputDevices;

		std::map<SpaceNavigatorAction::Action, std::shared_ptr<ActionDetails>> actions;

		PreferredJoystick preferredStick;

		JoyRecord record;

		JoystickConfigurator configurator;

		bool ready = false;

		bool filterXInputDevicesOut = false;

		std::unique_ptr<SpaceNavigatorEmulator::IMode> axisProcessor;

		std::unique_ptr<SpaceNavigatorEmulator::IMode> axisSwapper;

		std::unique_ptr<SpaceNavigatorEmulator::IMode> keySender;

		ButtonTester buttonTester;
	};
}