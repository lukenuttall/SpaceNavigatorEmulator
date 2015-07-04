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
#pragma once

#include "sidef.h"

enum SpwRetVal SpwErrorVal;

#ifdef __cplusplus
extern "C" {
#endif
	
int _afxForceUSRDLL;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved);

enum SpwRetVal SPWAPI SiInitialize(void);
SPWbool SiIsInitialized();
void SPWAPI SiTerminate(void);

void SPWAPI SiGetLibraryInfo(SiVerInfo *info);
enum SpwRetVal SPWAPI SiGetDriverInfo(SiVerInfo *info);
enum SpwRetVal SPWAPI SiGetDevicePort(int devid, SiDevPort *port);

int SPWAPI SiGetNumDevices(void);

void SPWAPI SiOpenWinInit(SiOpenData *opendata, void *win);
struct SiHdl *SPWAPI SiOpen(const char *appname, int devid, SiTypeMask *tmask, int mode, SiOpenData *opendata);
struct SiHdl* SPWAPI SiOpenPort(const char *pAppName, const SiDevPort *pPort, int mode, const SiOpenData *pData);
enum SpwRetVal SPWAPI SiClose(struct SiHdl *si);

int SPWAPI SiGetDeviceID(struct SiHdl *si);
enum SpwRetVal SPWAPI SiGetDeviceImageFileName(struct SiHdl *si, char *name, unsigned long *maxlen);
enum SpwRetVal SPWAPI SiGetDeviceInfo(struct SiHdl *si, SiDevInfo *info);
enum SpwRetVal SPWAPI SiGetDeviceName(struct SiHdl *si, SiDeviceName *name);

enum SpwRetVal SPWAPI SiGrabDevice(struct SiHdl *si, SPWbool exclusive);
enum SpwRetVal SPWAPI SiReleaseDevice(struct SiHdl *si);

enum SpwRetVal SPWAPI SiRezero(struct SiHdl *si);
enum SpwRetVal SPWAPI SiBeep(struct SiHdl *si, char *str);
enum SpwRetVal SPWAPI SiSetLEDs(struct SiHdl *si, unsigned long mask);
enum SpwRetVal SPWAPI SiSetUiMode(struct SiHdl *si, unsigned long mode);

int SPWAPI SiDispatch(struct SiHdl *si, SiGetEventData *data, SiSpwEvent *ev, SiSpwHandlers *handlers);

void SPWAPI SiGetEventWinInit(SiGetEventData *evdata, unsigned int msg, unsigned int wparam, long lparam);
SpwRetVal SPWAPI SiGetEvent(struct SiHdl *si, int flags, const SiGetEventData *data, SiSpwEvent *ev);
SpwRetVal SPWAPI SiPeekEvent(struct SiHdl *si, int flags, const SiGetEventData *data, SiSpwEvent *ev);

int SPWAPI SiButtonPressed(SiSpwEvent *ev);
int SPWAPI SiButtonReleased(SiSpwEvent *ev);
enum SpwRetVal SPWAPI SiGetButtonName(struct SiHdl *si, unsigned long bnum, SiButtonName *name);

void *SPWAPI SiGetCompanyIcon(void);
enum SpwRetVal SPWAPI SiGetCompanyLogoFileName(char *name, unsigned long *maxlen);

const char *SPWAPI SpwErrorString(enum SpwRetVal err);



/* deprecated */
enum SpwRetVal SiSetTypeMask(SiTypeMask *mask, int type1, ...);

#include "syncapi.h"

#ifdef __cplusplus
}
#endif

