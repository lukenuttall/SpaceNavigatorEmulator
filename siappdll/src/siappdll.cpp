/*
SpaceNavigatorEmulator - 3dconnexion-compatible siappdll.dll replacement
Copyright (C) 2014  John Tsiombikas <nuclear@member.fsf.org>
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
#include "config.h"
#include <stdio.h>
#include <malloc.h>
#include <windows.h>
#include "siappdll.h"
#include "siappdll_impl.h"
#include <iostream>
#include <fstream>
#include <string>
#include "..\Log.h"
#include "..\resource.h"

#include "MinHook.h"

extern "C"{
	typedef UINT(__stdcall * GETRAWINPUTDEVICEINFOW)(HANDLE, UINT, LPVOID, PUINT);

	GETRAWINPUTDEVICEINFOW fpOldGetRawInput = NULL;

	typedef UINT(WINAPI *GetDeviceList)(PRAWINPUTDEVICELIST pRawInputDeviceList, PUINT puiNumDevices, UINT cbSize); 

	GetDeviceList fpOldGetDeviceList = NULL;	
}


bool hookNext = false;
UINT WINAPI DetourGetRawInputDeviceList(PRAWINPUTDEVICELIST pRawInputDeviceList, PUINT puiNumDevices, UINT cbSize)
{
	if (hookNext)
	{
		LOG(TRACE) << "Faking GetRawInputDeviceList";
		hookNext = false;
		return 1;
	}
	else
	{
		return fpOldGetDeviceList(pRawInputDeviceList, puiNumDevices, cbSize);
	}
}


UINT WINAPI DetourGetRawInputDeviceInfoW(HANDLE hDevice, UINT uiCommand, LPVOID pData, PUINT pcbSize)
{
	if (hookNext)
	{
		LOG(TRACE) << "Faking GetRawInputDeviceInfoW";
		hookNext = false;
		return 1;
	}
	else
	{
		return fpOldGetRawInput(hDevice, uiCommand, pData, pcbSize);
	}
}


int startHook()
{
	// Initialize MinHook.
	if (MH_Initialize() != MH_OK)
	{
		return 1;
	}

	// Create a hook in disabled state.
	if (MH_CreateHook(&GetRawInputDeviceInfoW, &DetourGetRawInputDeviceInfoW,
		reinterpret_cast<LPVOID*>(&fpOldGetRawInput)) != MH_OK)
	{
		return 1;
	}		
	if (MH_CreateHook(&GetRawInputDeviceList, &DetourGetRawInputDeviceList,
		reinterpret_cast<LPVOID*>(&fpOldGetDeviceList)) != MH_OK)
	{
		return 1;
	}

	// Enable the hook
	if (MH_EnableHook(&GetRawInputDeviceInfoW) != MH_OK)
	{
		LOG(ERROR) << "Could not hook GetRawInputDeviceInfoW";
		return 1;
	}

	// Enable the hook 
	if (MH_EnableHook(&GetRawInputDeviceList) != MH_OK)
	{
		LOG(ERROR) << "Could not hook GetRawInputDeviceList";
		return 1;
	}
	return -1;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		// Make sure we've initialised the logger
		Log::getInstance();
		LOG(INFO) << "Starting siappdll replacement";

		auto startedHook = startHook();
		
		LOG(INFO) << "Siappdll loaded: ";
		char buffer[MAX_PATH];
		if (!FAILED(::GetModuleFileName(::GetModuleHandle("siappdll.dll"), buffer, MAX_PATH)))
		{
			LOG(INFO) << "Loaded from: "<< buffer;
		}
		
		// We could set this byte as an alternative to hooking the device input functions
		// but it's more liable to blow up with different versions of the static libraries included
		// in apps using the space navigator sdk
		//byte* actualbRawInputFound3DxDevice = (byte *)0x413489;
		//*actualbRawInputFound3DxDevice = 1;		
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

enum SpwRetVal SPWAPI SiInitialize(void)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	
	if(!(space_ware_message = RegisterWindowMessage("SpaceWareMessage00"))) {
		LOG(ERROR) << "failed to register window message";
		return SPW_ERROR;
	}

	initialised = true;
	return SPW_NO_ERROR;
}

SPWbool SiIsInitialized()
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return initialised;
}

void SPWAPI SiTerminate(void)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	initialised = false;
}


void SPWAPI SiGetLibraryInfo(SiVerInfo *info)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	info->major = VER_MAJOR;
	info->minor = VER_MINOR;
	info->build = VER_BUILD;
	strcpy(info->version, VER_STR);
	info->data[0] = 0;
}

enum SpwRetVal SPWAPI SiGetDriverInfo(SiVerInfo *info)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	info->major = VER_MAJOR;
	info->minor = VER_MINOR;
	info->build = VER_BUILD;
	strcpy(info->version, VER_STR);
	info->data[0] = 0;

	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiGetDevicePort(int devid, SiDevPort *port)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	port->devID = 0;
	port->devClass = 
	port->devType = SI_SPACENAVIGATOR;
	strcpy(port->devName, "SpaceNavigatorEmulator");
	strcpy(port->portName, "Pretend Port");
	return SPW_NO_ERROR;
}

int SPWAPI SiGetNumDevices(void)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return 1;
}

int SPWAPI SiDeviceIndex(int idx)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return 0;
}


void SPWAPI SiOpenWinInit(SiOpenData *opendata, void *win)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	opendata->win = win;
}

struct SiHdl *SPWAPI SiOpen(const char *appname, int devid, SiTypeMask *tmask, int mode, SiOpenData *opendata)
{
	LOG(TRACE) << "SiOpen called for app: " << appname;
	struct SiHdl *si = new SiHdl();

	if (!si)
	{
		SpwErrorVal = SPW_ERROR;
		return 0;
	}

	si->opendata = *opendata;
	si->opendata.lib_flag = 1;
	si->opendata.trans_ctl = nullptr;
	// FIXME - get an actual process id.
	si->opendata.pid = 1;
	si->connector = new SpaceNavigatorEmulator::SpaceNavConnector(space_ware_message);

	si->connector->Open((HWND)opendata->win);

	SpwErrorVal = SPW_NO_ERROR;
	return si;
}

struct SiHdl* SPWAPI SiOpenPort(const char *pAppName, const SiDevPort *pPort, int mode, const SiOpenData *pData)
{
	LOG(TRACE) << __FUNCTION__ << " passing to SiOpen";
	// FIXME - copy data instead on const cast
	return SiOpen(pAppName, 0, nullptr, mode, const_cast<SiOpenData*>(pData));
}

enum SpwRetVal SPWAPI SiClose(struct SiHdl *si)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	si->connector->Close();
	delete si->connector;
	delete si;
	return SPW_NO_ERROR;
}


int SPWAPI SiGetDeviceID(struct SiHdl *si)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	SpwErrorVal = SPW_NO_ERROR;
	return 0;	/* TODO */
}

enum SpwRetVal SPWAPI SiGetDeviceImageFileName(struct SiHdl *si, char *name, unsigned long *maxlen)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	name[0] = 0;
	*maxlen = 0;
	return SPW_NO_ERROR;	/* TODO */
}

SpwRetVal SPWAPI SiGetDeviceInfo(struct SiHdl *si, SiDevInfo *info)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	info->dev_type = SI_SPACENAVIGATOR;
	info->num_buttons = 2;
	info->num_degrees = 6;
	info->can_beep = 0;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiGetDeviceName(struct SiHdl *si, SiDeviceName *name)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	strcpy(name->name, "SpaceNavigatorEmulator");
	return SPW_NO_ERROR;
}


enum SpwRetVal SPWAPI SiGrabDevice(struct SiHdl *si, SPWbool exclusive)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiReleaseDevice(struct SiHdl *si)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiRezero(struct SiHdl *si)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiBeep(struct SiHdl *si, char *str)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSetLEDs(struct SiHdl *si, unsigned long mask)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSetUiMode(struct SiHdl *si, unsigned long mode)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

#define CALL_CB(cb, od, evdata, ev)	cb.func(od, evdata, ev, cb.cls)

int SPWAPI SiDispatch(struct SiHdl *si, SiGetEventData *data, SiSpwEvent *ev, SiSpwHandlers *handlers)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__ << " - event: " << ev->type;

	if (handlers != nullptr)
	{
		switch (ev->type) {
		case SI_BUTTON_EVENT:
			return CALL_CB(handlers->button, &si->opendata, data, ev);
		case SI_MOTION_EVENT:
			if (handlers->motion.func != nullptr)
				return CALL_CB(handlers->motion, &si->opendata, data, ev);
		case SI_COMBO_EVENT:
			return CALL_CB(handlers->combo, &si->opendata, data, ev);
		case SI_ZERO_EVENT:
			return CALL_CB(handlers->zero, &si->opendata, data, ev);
		default:
			break;
		}
	}
	return 0;
}


void SPWAPI SiGetEventWinInit(SiGetEventData *evdata, unsigned int msg, unsigned int wparam, long lparam)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	if (evdata == nullptr)
	{
		LOG(ERROR) << __FUNCTION__ << " called with null pointer for event storage";
	}
	evdata->msg = msg;
	evdata->lparam = lparam;
	evdata->wparam = wparam;
}

SpwRetVal SPWAPI SiGetEvent(struct SiHdl *si, int flags, const SiGetEventData *data, SiSpwEvent *ev)
{
	LOG(TRACE) << "called: " << __FUNCTION__;

	// If we weren't given a valid pointer, we return bad value.
	if (!ev || !si || !data)
		return SI_BAD_VALUE;
	
	if (data->msg == space_ware_message)
	{
		auto record = si->connector->GetRecord();

		// completely fake an event
		if (data->wparam == SI_MOTION_EVENT)
		{
			ev->type = data->wparam;
			ev->u.spw_data.button.last = 0;
			ev->u.spw_data.button.current = 0;
			ev->u.spw_data.button.release = 0;
			ev->u.spw_data.button.pressed = 0;
			ev->u.spw_data.motion[0] = record.x; 
			ev->u.spw_data.motion[1] = record.y; 
			ev->u.spw_data.motion[2] = record.z; 
			ev->u.spw_data.motion[3] = record.rx; 
			ev->u.spw_data.motion[4] = record.ry; 
			ev->u.spw_data.motion[5] = record.rz; 
			// FIXME - use elasped period and not timeout
			ev->u.spw_data.period = si->connector->getPeriodMs();

			if (data->wparam == SI_MOTION_EVENT)
				hookNext = true;
		}
		else if (data->wparam == SI_BUTTON_EVENT)
		{
			ev->type = data->wparam;
			ev->u.spw_data.button.last = record.buttonData.last;
			ev->u.spw_data.button.current = record.buttonData.current;
			ev->u.spw_data.button.release = record.buttonData.release;
			ev->u.spw_data.button.pressed = record.buttonData.pressed;
			ev->u.spw_data.motion[0] = 0;
			ev->u.spw_data.motion[1] = 0;
			ev->u.spw_data.motion[2] = 0;
			ev->u.spw_data.motion[3] = 0;
			ev->u.spw_data.motion[4] = 0;
			ev->u.spw_data.motion[5] = 0;
			ev->u.spw_data.period = 0;
		}
		else if (data->wparam == SI_BUTTON_PRESS_EVENT || data->wparam == SI_BUTTON_RELEASE_EVENT)
		{
			ev->type = data->wparam;
			ev->u.hwButtonEvent.buttonNumber = data->lparam;
		}

		return SI_IS_EVENT;
	}
	else
	{
		ev = 0;
		return SI_NOT_EVENT;
	}
}

SpwRetVal SPWAPI SiPeekEvent(struct SiHdl *si, int flags, const SiGetEventData *data, SiSpwEvent *ev)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;

	// FIXME - implement
	return SPW_NO_ERROR;
}

SPWbool SPWAPI SiIsSpaceWareEvent(const SiGetEventData *data, struct SiHdl *si)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return data->msg == space_ware_message;
}

int SPWAPI SiButtonPressed(SiSpwEvent *ev)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;

	long pressed = ev->u.spw_data.button.pressed;

	for (unsigned int i = 0; i < 32; i++)
	{
		if ((1 << i) & pressed)
		{
			return i;
		}
	}
	return 0;
}

int SPWAPI SiButtonReleased(SiSpwEvent *ev)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;

	long released = ev->u.spw_data.button.release;

	for (unsigned int i = 0; i < 32; i++)
	{
		if ((1 << i) & released)
		{
			return i;
		}
	}
	return 0;
}

enum SpwRetVal SPWAPI SiGetButtonName(struct SiHdl *si, unsigned long bnum, SiButtonName *name)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	strcpy(name->name, "Smoooth buttony text");
	return SPW_NO_ERROR;
}

void *SPWAPI SiGetCompanyIcon(void)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;

	return ::LoadImage(::GetModuleHandle("siappdll.dll"), MAKEINTRESOURCE(WING_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);	
}

enum SpwRetVal SPWAPI SiGetCompanyLogoFileName(char *name, unsigned long *maxlen)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;


	char buffer[MAX_PATH];
	if (FAILED(::GetModuleFileName(::GetModuleHandle("siappdll.dll"), buffer, MAX_PATH)))
	{
		return SPW_ERROR;
	}

	unsigned int length = strlen(buffer);
	char* path = "\\ws.bmp";
	unsigned int maxLength = length + strlen(path);

	if (maxLength > *maxlen)
	{
		*maxlen = maxLength;
		return SI_BAD_VALUE;
	}

	strncat(buffer, path, maxLength);
	*maxlen = maxLength;

	strncpy(name, buffer, maxLength);
	return SPW_NO_ERROR;
}


static const char *errnames[] = {
	"no error",
	"error -- function failed",
	"invalid 3DxWare handle",
	"invalid device ID",
	"invalid argument value",
	"event is a 3DxWare event",
	"skip this 3DxWare event",
	"event is not a 3DxWare event",
	"3DxWare driver is not running",
	"3DxWare driver is not responding",
	"the function is unsupported by this version",
	"3DxWare Input Library is uninitialized",
	"incorrect driver for this 3DxWare version",
	"internal 3DxWare error"
};

const char *SPWAPI SpwErrorString(enum SpwRetVal err)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return errnames[(int)err];
}

/* deprecated */
enum SpwRetVal SiSetTypeMask(SiTypeMask *mask, int type1, ...)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}
