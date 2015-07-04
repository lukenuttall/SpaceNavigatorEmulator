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
#include "PreferredJoystick.h"
#include <winerror.h>
#include "Log.h"

// Associate the right guid with joy config interface
struct __declspec(uuid("EB0D7DFA-1990-4F27-B4D6-EDF2EEC4A44C")) IDirectInputJoyConfig8;

using namespace Microsoft::WRL;

namespace SpaceNavigatorEmulator
{

	PreferredJoystick::PreferredJoystick()		
	{
	}


	PreferredJoystick::~PreferredJoystick()
	{
	}

	bool PreferredJoystick::Initialise(const Microsoft::WRL::ComPtr<IDirectInput8>& directInput)
	{
		LOG(TRACE) << "Initialising preferred joystick";
		HRESULT hr;
		Microsoft::WRL::ComPtr<IDirectInputJoyConfig8> joyConfig;
		if (FAILED(hr = directInput.As(&joyConfig)))
			return false;
		
		preferred.dwSize = sizeof(DIJOYCONFIG);
		if (SUCCEEDED(joyConfig->GetConfig(0, &preferred, DIJC_GUIDINSTANCE))) // This function is expected to fail if no joystick is attached
			validConfig = true;

		return true;
	}

	bool PreferredJoystick::PreferredJoystickExists()
	{
		return validConfig;
	}

	bool PreferredJoystick::IsPreferredJoystick(GUID joystickInstance)
	{
		if (PreferredJoystickExists())
		{
			return IsEqualGUID(joystickInstance, preferred.guidInstance);
		}
		return false;
	}

}