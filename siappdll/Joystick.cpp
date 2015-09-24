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

#include "Joystick.h"

//#pragma warning(push)
//#pragma warning(disable:6000 28251)
#include <dinput.h>
//#pragma warning(pop)

#include <dinputd.h>


#include <windows.h>
#include <commctrl.h>
#include <basetsd.h>
#include <dinputd.h>
#include <assert.h>
#include <oleauto.h>
#include <shellapi.h>

#include <iostream>

// Stuff to filter out XInput devices
#include <wbemidl.h>
#include "Log.h"
#include <boost\foreach.hpp>

#include "AxisProcessor.h"
#include "AxisSwapper.h"
#include "KeySender.h"

#include "JoystickConfigurator.h"

//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=nullptr; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=nullptr; } }

using namespace std;
using namespace Microsoft::WRL;

namespace SpaceNavigatorEmulator
{

	Joystick::Joystick(HWND window, std::function<void()>&& callback) :
		window(window),
		changedCallback(std::move(callback))
	{
		bool stuck = Initialise();
		ready = stuck;

		axisProcessor.reset(new AxisProcessor());
		axisSwapper.reset(new AxisSwapper());
		keySender.reset(new KeySender(window));
		LOG(INFO) << "Joystick " << (!stuck ? "" : "not ") << "initialised";
	}


	Joystick::~Joystick()
	{
		LOG(INFO) << "Releasing Joystick";
		Release();
	}

	JoyRecord const & Joystick::GetRecord() const
	{
		return record;
	}

	void Joystick::PollStick()
	{
		LOG(TRACE) << "polling stick";
		HRESULT hr;
		DIJOYSTATE js;           // DInput joystick state 

		try{
			if (!stick)
				return;
		}
		catch (...)
		{
			LOG(ERROR) << "Could not read joystick pointer";
			return;
		}
		// Poll the device to read the current state
		hr = stick->Poll();

		if (FAILED(hr))
		{
			if (hr == DIERR_INPUTLOST)
			{
				LOG(ERROR) << "Input lost";
			}
			else if (hr == DIERR_NOTACQUIRED)
			{
				LOG(ERROR) << "Not acquired";
			}
			else if (hr == DIERR_NOTINITIALIZED)
			{
				LOG(ERROR) << "Not initialised";
			}

			// DInput is telling us that the input stream has been
			// interrupted. We aren't tracking any state between polls, so
			// we don't have any special reset that needs to be done. We
			// just re-acquire and try again.
			hr = stick->Acquire();
			if (hr == DIERR_NOTACQUIRED)
			{
				LOG(ERROR) << "Aquiring stick failed";
			}
			while (hr == DIERR_INPUTLOST)
				hr = stick->Acquire();

			// hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
			// may occur when the app is minimized or in the process of 
			// switching, so just try again later 
			//return;
			LOG(ERROR) << "Stick not acquired: " << hr;
		}

		JoyRecord old(record);

		
		set<SpaceNavigatorAction::Action> pressed;
		set<SpaceNavigatorAction::Action> released;

		DIDEVICEOBJECTDATA didod[128];
		DWORD dwObjCount = 128;

		if (FAILED(hr = stick->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), didod, &dwObjCount, 0)))
		{
			LOG(ERROR) << "Failed getting device data: " << hr;
		}
		else
		{
			LOG(TRACE) << "Read " << dwObjCount << "buffered entries";
			for (unsigned int i = 0; i < dwObjCount; i++)
			{
				auto data = didod[i];
				auto action = GetBufferedAction(data.dwOfs);
				switch (action)
				{
				case SpaceNavigatorAction::BUTTON_1:
				case SpaceNavigatorAction::BUTTON_2:
				case SpaceNavigatorAction::BUTTON_3:
				case SpaceNavigatorAction::BUTTON_4:
				{
					bool down = (data.dwData & 0x80) != 0;
					LOG(TRACE) << action << " is " << down;
					if (down)
					{
						if (pressed.find(action) == pressed.end())
							pressed.insert(action);
					}
					else
					{
						if (released.find(action) == released.end())
							released.insert(action);
					}
					break;
				}
				default:
					break;
				}				
			}
		}

		// Get the input's device state
		if (FAILED(hr = stick->GetDeviceState(sizeof(DIJOYSTATE), &js)))
		{
			LOG(ERROR) << "Stick not acquired";
			return; // The device should have been acquired during the Poll()
		}

		record.buttonData.last = old.buttonData.current;
		record.buttonData.current = GetCurrentButtonState(&js);
		record.buttonData.release = GetButtonState(released);
		record.buttonData.pressed = GetButtonState(pressed);

		axisProcessor->Process(record, &js);
		axisSwapper->Process(record, &js);
		bool sentKeys = keySender->Process(record, &js);

		if (sentKeys)
		{
			record = old;
		}
		
		changedCallback();
	}

	long Joystick::GetButtonState(set<SpaceNavigatorAction::Action> actions)
	{
		long state = 0;
		BOOST_FOREACH(auto action, actions)
		{
			switch (action)
			{
			case SpaceNavigatorAction::BUTTON_1:
				state |= 2;
				break;
			case SpaceNavigatorAction::BUTTON_2:
				state |= 4;
				break;
			case SpaceNavigatorAction::BUTTON_3:
				state |= 8;
				break;
			case SpaceNavigatorAction::BUTTON_4:
				state |= 16;
				break;
			}
		}
		return state;
	}

	long Joystick::GetCurrentButtonState(DIJOYSTATE* stickState)
	{
		long state = 0;
		if (buttonTester.IsButtonDown(SpaceNavigatorAction::BUTTON_1, stickState))
			state |= 2;
		if (buttonTester.IsButtonDown(SpaceNavigatorAction::BUTTON_2, stickState))
			state |= 4;
		if (buttonTester.IsButtonDown(SpaceNavigatorAction::BUTTON_3, stickState))
			state |= 8;
		if (buttonTester.IsButtonDown(SpaceNavigatorAction::BUTTON_4, stickState))
			state |= 16;

		return state;
	}

	SpaceNavigatorAction::Action Joystick::GetBufferedAction(DWORD offset)
	{
		BOOST_FOREACH(auto action, actions)
		{
			if (action.second->mappingOffset == offset)
			{
				return action.first;
			}
		}
		// fixme - add a nop action to the enum
		return SpaceNavigatorAction::BUTTON_SWAP_AXES;
	}

	bool Joystick::Initialise()
	{
		LOG(TRACE) << "Initialising Joystick";

		HRESULT hr;

		// Register with the DirectInput subsystem and get a pointer
		// to a IDirectInput interface we can use.
		// Create a DInput object
		if (FAILED(hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)directInput.ReleaseAndGetAddressOf(), nullptr)))
		{
			LOG(ERROR) << "Could not create directinput8 interface: " << hr;
			return false;
		}
		LOG(TRACE) << "Created directinput";
		
		configurator.openConfiguration();

		if (filterXInputDevicesOut)
			FindXInputDevices();

		preferredStick.Initialise(directInput);
		LOG(TRACE) << "Created initialised preferred stick";

		// Look for a simple joystick we can use for this sample program.
		if (FAILED(hr = directInput->EnumDevices(DI8DEVCLASS_GAMECTRL, StaticEnumJoysticksCallback, this, DIEDFL_ATTACHEDONLY)))			
		{
			LOG(ERROR) << "Could not enumerate joysticks: " << hr;
			return false;
		}
		
		LOG(TRACE) << "Enumerated joysticks";

		// Make sure we got a joystick
		if (!stick)
		{
			LOG(ERROR) << "Did not find a joystick";
			return false;			
		}
		
		auto configuratorActions = configurator.GetActions();
		BOOST_FOREACH(ActionMap const& map, configuratorActions)
		{
			std::shared_ptr<ActionDetails> details(new ActionDetails(map.getMappingOffset(), map.IsInverted()));

			actions[map.getAction()] = details;
		}



		// Set the data format to "simple joystick" - a predefined data format 
		//
		// A data format specifies which controls on a device we are interested in,
		// and how they should be reported. This tells DInput that we will be
		// passing a DIJOYSTATE structure to IDirectInputDevice::GetDeviceState().
		if (FAILED(hr = stick->SetDataFormat(&c_dfDIJoystick)))
		{
			LOG(ERROR) << "Could not set joystick format:" << hr;
			return false;
		}

		DIPROPDWORD dipdw;
		dipdw.diph.dwSize = sizeof(dipdw);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwObj = DIPH_DEVICE;
		dipdw.diph.dwHow = 0;
		dipdw.dwData = 128;

		if (FAILED(hr = stick->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
		{
			LOG(ERROR) << "Could not set stick buffer size";
			if (hr == DIERR_INVALIDPARAM){
				LOG(ERROR) << "Invalid parameter";
			}
			else if (hr == DIERR_NOTINITIALIZED){
				LOG(ERROR) << "Not initialised";
			}
			else if (hr == DIERR_OBJECTNOTFOUND){
				LOG(ERROR) << "Object not found";
			}
			else if (hr == DIERR_UNSUPPORTED){
				LOG(ERROR) << "Unsupported";
			};
		}
		else
		{
			LOG(TRACE) << "Set buffer to: " << dipdw.dwData;
		}


		// Set the cooperative level to let DInput know how this device should
		// interact with the system and with other DInput applications.
		if (FAILED(hr = stick->SetCooperativeLevel(window, DISCL_EXCLUSIVE | DISCL_BACKGROUND)))
		{
			LOG(ERROR) << "Could not set joystick cooperative level: " << hr;
			switch (hr)
			{
				case DIERR_INVALIDPARAM:
					LOG(ERROR) << "An invalid parameter was passed to the returning function, or the object was not in a state that permitted the function to be called.This value is equal to the E_INVALIDARG standard Component Object Model(COM) return value.";
					break;
				case DIERR_NOTINITIALIZED:	
					LOG(ERROR) << "The object has not been initialized.";
					break;
				case E_HANDLE:	
					LOG(ERROR) << "The HWND parameter is not a valid top - level window that belongs to the process.";
					break;
				default:
					LOG(ERROR) << "Unknown error";
					break;
			}
			return false;
		}

		if (FAILED(hr = stick->Acquire()))
		{
			LOG(ERROR) << "Could not acquire stick initially: " << hr;
		}

		// Enumerate the joystick objects. The callback function enabled user
		// interface elements for objects that are found, and sets the min/max
		// values property for discovered axes.
		if (FAILED(hr = stick->EnumObjects(StaticEnumObjectsCallback, this, DIDFT_ALL)))
		{
			LOG(ERROR) << "Could not enumerate joystick objects: " << hr;
			return false;
		}

		return S_OK;
	}

	void Joystick::Release()
	{
		ready = false;
		// Unacquire the device one last time just in case 
		// the app tried to exit while the device is still acquired.
		if (stick)
			stick->Unacquire();
	}

	//-----------------------------------------------------------------------------
	// Enum each PNP device using WMI and check each device ID to see if it contains 
	// "IG_" (ex. "VID_045E&PID_028E&IG_00").  If it does, then it’s an XInput device
	// Unfortunately this information can not be found by just using DirectInput.
	// Checking against a VID/PID of 0x028E/0x045E won't find 3rd party or future 
	// XInput devices.
	//
	// This function stores the list of xinput devices in a linked list 
	// at g_pXInputDeviceList, and IsXInputDevice() searchs that linked list
	//-----------------------------------------------------------------------------
	void Joystick::FindXInputDevices()
	{
		LOG(TRACE) << "Finding xinput devices";
		xinputDevices.clear();

		IWbemServices* pIWbemServices = nullptr;
		IEnumWbemClassObject* pEnumDevices = nullptr;
		IWbemLocator* pIWbemLocator = nullptr;
		IWbemClassObject* pDevices[20] = { 0 };
		BSTR bstrDeviceID = nullptr;
		BSTR bstrClassName = nullptr;
		BSTR bstrNamespace = nullptr;
		DWORD uReturned = 0;
		bool bCleanupCOM = false;
		UINT iDevice = 0;
		VARIANT var;
		HRESULT hr;

		// CoInit if needed
		hr = CoInitialize(nullptr);
		bCleanupCOM = SUCCEEDED(hr);

		// Create WMI
		hr = CoCreateInstance(__uuidof(WbemLocator),
			nullptr,
			CLSCTX_INPROC_SERVER,
			__uuidof(IWbemLocator),
			(LPVOID*)&pIWbemLocator);
		if (FAILED(hr) || pIWbemLocator == nullptr)
			goto LCleanup;

		// Create BSTRs for WMI
		bstrNamespace = SysAllocString(L"\\\\.\\root\\cimv2"); if (bstrNamespace == nullptr) goto LCleanup;
		bstrDeviceID = SysAllocString(L"DeviceID");           if (bstrDeviceID == nullptr)  goto LCleanup;
		bstrClassName = SysAllocString(L"Win32_PNPEntity");    if (bstrClassName == nullptr) goto LCleanup;

		// Connect to WMI 
		hr = pIWbemLocator->ConnectServer(bstrNamespace, nullptr, nullptr, 0L,
			0L, nullptr, nullptr, &pIWbemServices);
		if (FAILED(hr) || pIWbemServices == nullptr)
			goto LCleanup;

		// Switch security level to IMPERSONATE
		(void)CoSetProxyBlanket(pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr,
			RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, 0);

		// Get list of Win32_PNPEntity devices
		hr = pIWbemServices->CreateInstanceEnum(bstrClassName, 0, nullptr, &pEnumDevices);
		if (FAILED(hr) || pEnumDevices == nullptr)
			goto LCleanup;

		// Loop over all devices
		for (;;)
		{
			// Get 20 at a time
			hr = pEnumDevices->Next(10000, 20, pDevices, &uReturned);
			if (FAILED(hr))
				goto LCleanup;
			if (uReturned == 0)
				break;

			for (iDevice = 0; iDevice < uReturned; iDevice++)
			{
				if (!pDevices[iDevice])
					continue;

				// For each device, get its device ID
				hr = pDevices[iDevice]->Get(bstrDeviceID, 0L, &var, nullptr, nullptr);
				if (SUCCEEDED(hr) && var.vt == VT_BSTR && var.bstrVal != nullptr)
				{
					// Check if the device ID contains "IG_".  If it does, then it’s an XInput device
					// Unfortunately this information can not be found by just using DirectInput 
					if (wcsstr(var.bstrVal, L"IG_"))
					{
						// If it does, then get the VID/PID from var.bstrVal
						DWORD dwPid = 0, dwVid = 0;
						WCHAR* strVid = wcsstr(var.bstrVal, L"VID_");
						if (strVid && swscanf(strVid, L"VID_%4X", &dwVid) != 1)
							dwVid = 0;
						WCHAR* strPid = wcsstr(var.bstrVal, L"PID_");
						if (strPid && swscanf(strPid, L"PID_%4X", &dwPid) != 1)
							dwPid = 0;

						DWORD dwVidPid = MAKELONG(dwVid, dwPid);

						xinputDevices.push_back(dwVidPid);
					}
				}
				SAFE_RELEASE(pDevices[iDevice]);
			}
		}

	LCleanup:
		if (bstrNamespace)
			SysFreeString(bstrNamespace);
		if (bstrDeviceID)
			SysFreeString(bstrDeviceID);
		if (bstrClassName)
			SysFreeString(bstrClassName);
		for (iDevice = 0; iDevice < 20; iDevice++)
			SAFE_RELEASE(pDevices[iDevice]);
		SAFE_RELEASE(pEnumDevices);
		SAFE_RELEASE(pIWbemLocator);
		SAFE_RELEASE(pIWbemServices);
	}

	// Checks whether stick is really an xbox controller
	bool Joystick::IsXInputDevice(const GUID* pGuidProductFromDirectInput)
	{
		return std::find(xinputDevices.begin(), xinputDevices.end(), pGuidProductFromDirectInput->Data1) != xinputDevices.end();
	}

	// When call the call the function that issues this callback, we supply a this pointer as the context;
	BOOL __stdcall Joystick::StaticEnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, void* pContext)
	{
		auto joystick = reinterpret_cast<Joystick*>(pContext);
		return joystick->EnumJoysticksCallback(pdidInstance) ? DIENUM_CONTINUE : DIENUM_STOP;
	}

	//-----------------------------------------------------------------------------
	// Name: EnumJoysticksCallback()
	// Desc: Called once for each enumerated joystick. If we find one, create a
	//       device interface on it so we can play with it.
	//-----------------------------------------------------------------------------
	bool Joystick::EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance)
	{
		LOG(TRACE) << "Enumerating joystick: " << pdidInstance->tszInstanceName;
		HRESULT hr;

		if (filterXInputDevicesOut && IsXInputDevice(&pdidInstance->guidProduct))
		{
			LOG(TRACE) << "Found and filtering out xInput device";
			return true;
		}

		// Skip anything other than the perferred joystick device as defined by the control panel.  
		// Instead you could store all the enumerated joysticks and let the user pick.
		if (preferredStick.PreferredJoystickExists() && !preferredStick.IsPreferredJoystick(pdidInstance->guidInstance))
		{
			LOG(TRACE) << "This is not the stick we are looking for";
			return true;
		}

		// Obtain an interface to the enumerated joystick.
		if (FAILED(hr = directInput->CreateDevice(pdidInstance->guidInstance, stick.ReleaseAndGetAddressOf(), nullptr)))
		{
			LOG(TRACE) << "Failed to create joystick direct x device";
			return true;
		}

		// We set the deadzone on the preferred stick, but continue on if that fails.
		if (preferredStick.IsPreferredJoystick(pdidInstance->guidInstance))
		{
			LOG(TRACE) << "Found preferred stick: " << pdidInstance->tszInstanceName;
			
			// Set deadzone.
			DIPROPDWORD dipdw;
			dipdw.diph.dwSize = sizeof(dipdw);
			dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
			dipdw.diph.dwObj = DIPH_DEVICE;
			dipdw.diph.dwHow = 0;
			dipdw.dwData = configurator.GetDeadZone();

			HRESULT hr;
			if (FAILED(hr = stick->SetProperty(DIPROP_DEADZONE, &dipdw.diph)))
			{
				LOG(ERROR) << "Could not set deadzone for: " << pdidInstance->tszInstanceName;
				if (hr == DIERR_INVALIDPARAM){
					LOG(ERROR) << "Invalid parameter";
				}
				else if (hr == DIERR_NOTINITIALIZED){
					LOG(ERROR) << "Not initialised";
				}
				else if (hr == DIERR_OBJECTNOTFOUND){
					LOG(ERROR) << "Object not found";
				}
				else if (hr == DIERR_UNSUPPORTED){
					LOG(ERROR) << "Unsupported";
				};
			}
			else
			{
				LOG(TRACE) << "Set deadzone to: " << dipdw.dwData;
			}
		}

		// Stop enumeration. Note: we're just taking the first joystick we get. You
		// could store all the enumerated joysticks and let the user pick.
		return false;
	}

	BOOL __stdcall Joystick::StaticEnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, void* pContext)
	{
		auto stick = reinterpret_cast<Joystick*>(pContext);
		return stick->EnumObjectsCallback(pdidoi) ? DIENUM_CONTINUE : DIENUM_STOP;
	}

	//-----------------------------------------------------------------------------
	// Name: EnumObjectsCallback()
	// Desc: Callback function for enumerating objects (axes, buttons, POVs) on a 
	//       joystick. This function enables user interface elements for objects
	//       that are found to exist, and scales axes min/max values.
	//-----------------------------------------------------------------------------
	bool Joystick::EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi)
	{
		LOG(TRACE) << "Enumerating: " << pdidoi->tszName;
		static int nSliderCount = 0;  // Number of returned slider controls
		static int nPOVCount = 0;     // Number of returned POV controls

		// For axes that are returned, set the DIPROP_RANGE property for the
		// enumerated axis in order to scale min/max values.
		if (pdidoi->dwType & DIDFT_AXIS)
		{
			// set the range
			DIPROPRANGE diprg;
			diprg.diph.dwSize = sizeof(DIPROPRANGE);
			diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			diprg.diph.dwHow = DIPH_BYID;
			diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
			diprg.lMin = -configurator.GetAxisRange();
			diprg.lMax = configurator.GetAxisRange();
		
			// Set the range for the axis
			if (FAILED(stick->SetProperty(DIPROP_RANGE, &diprg.diph)))
			{
				LOG(ERROR) << "Could not set space navigator axis range for: " << pdidoi->tszName;
				return false;
			}
		}


		// Set the UI to reflect what objects the joystick supports
		if (pdidoi->guidType == GUID_XAxis)
		{
			LOG(TRACE) << "IDC_X_AXIS";
		}
		if (pdidoi->guidType == GUID_YAxis)
		{
			LOG(TRACE) << "IDC_Y_AXIS";
		}
		if (pdidoi->guidType == GUID_ZAxis)
		{
			LOG(TRACE) << "IDC_Z_AXIS";
		}
		if (pdidoi->guidType == GUID_RxAxis)
		{
			LOG(TRACE) << "IDC_X_ROT";
		}
		if (pdidoi->guidType == GUID_RyAxis)
		{
			LOG(TRACE) << "IDC_Y_ROT";
		}
		if (pdidoi->guidType == GUID_RzAxis)
		{
			LOG(TRACE) << "IDC_Z_ROT";
		}
		if (pdidoi->guidType == GUID_Slider)
		{
			switch (nSliderCount++)
			{
			case 0:
				LOG(TRACE) << "IDC_SLIDER0";
				break;

			case 1:
				LOG(TRACE) << "IDC_SLIDER1";
				break;
			}
		}
		if (pdidoi->guidType == GUID_POV)
		{
			switch (nPOVCount++)
			{
			case 0:
				LOG(TRACE) << "IDC_POV0";
				break;

			case 1:
				LOG(TRACE) << "IDC_POV1";
				break;

			case 2:
				LOG(TRACE) << "IDC_POV2";
				break;

			case 3:
				LOG(TRACE) << "IDC_POV3";
				break;
			}
		}

		return true;
	}
}







