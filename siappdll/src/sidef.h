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

#define SPWAPI WINAPI

#include <windows.h>

#ifndef _MSC_VER
#include <inttypes.h>
#else
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
#endif

typedef int32_t SPWint32;
typedef uint32_t SPWuint32;
typedef long SPWbool;
typedef float SPWfloat32;
typedef double SPWfloat64;

#define SI_RESET_BUTTON		0
#define SI_PICK_BUTTON		31
#define SI_DIALOG_BUTTON	30

#define SI_RESET_BIT		(1L << SI_RESET_BUTTON)
#define SI_PICK_BIT			(1L << SI_PICK_BUTTON)
#define SI_DIALOG_BIT		(1L << SI_DIALOG_BUTTON)

#define SI_STRSIZE		128
#define SI_MAXBUF		128
#define SI_MAXPORTNAME	128

#define SI_NO_MASK		0

struct SiHdl;

#pragma pack(push, 4)

typedef int SiDevID;

typedef struct SiButtonData {
	unsigned long last;		/* previous button bitmask */
	unsigned long current;	/* current button bitmask */
	unsigned long pressed;	/* buttons pressed */
	unsigned long release;	/* buttons released */
} SiButtonData;

typedef struct SiDevInfo {
	int dev_type;
	int num_buttons;
	int num_degrees;
	SPWbool can_beep;
	char firmware[SI_STRSIZE];
} SiDevInfo;

typedef struct SiGetEventData {
	unsigned int msg;
	unsigned int wparam;
	long lparam;
} SiGetEventData;

typedef struct SiOpenData {
	void *win;
	void *trans_ctl;
	unsigned long pid;
	char exe_file[MAX_PATH];
	int lib_flag;
} SiOpenData;

typedef struct SiTypeMask {
	unsigned char mask[8];
} SiTypeMask;

/* SiSpwData::motion[6] index names */
enum { SI_TX, SI_TY, SI_TZ, SI_RX, SI_RY, SI_RZ };

typedef struct SiSpwData {
	SiButtonData button;
	long motion[6];
	unsigned long period;
} SiSpwData;

typedef enum SiEventType {
	SI_BUTTON_EVENT = 1,
	SI_MOTION_EVENT,
	SI_COMBO_EVENT,
	SI_ZERO_EVENT,
	SI_EXCEPTION_EVENT,
	SI_OUT_OF_BAND,
	SI_ORIENTATION_EVENT,
	SI_KEYBOARD_EVENT,
	SI_LPFK_EVENT,
	SI_APP_EVENT,
	SI_SYNC_EVENT,
	SI_BUTTON_PRESS_EVENT,
	SI_BUTTON_RELEASE_EVENT,
	SI_DEVICE_CHANGE_EVENT,
	SI_MOUSE_EVENT,
	SI_JOYSTICK_EVENT
} SiEventType;

typedef enum SiOrientation { SI_LEFT, SI_RIGHT } SiOrientation;

typedef struct         /* Data for SI_BUTTON_PRESS/RELEASE_EVENT */
{
	SPWuint32 buttonNumber;   /* The button number that went down/up in a *
							  * SI_BUTTON_PRESS/RELEASE_EVENT event */
} SiHWButtonData;

typedef struct SiSpwEvent {
	int type;
	union {
		SiSpwData spw_data;
		SiOrientation spw_orient;
		char ex_data[SI_MAXBUF];	/* exception data */
		SiHWButtonData hwButtonEvent;
	} u;
} SiSpwEvent;

typedef struct {
	int (*func)(SiOpenData*, SiGetEventData*, SiSpwEvent*, void*);
	void *cls;
} SiEventHandler;

typedef struct SiSpwHandlers {
	SiEventHandler button, motion, combo, zero, exception;
} SiSpwHandlers;

typedef struct SiVerInfo {
	int major, minor, build;
	char version[SI_STRSIZE];
	char data[SI_STRSIZE];
} SiVerInfo;

typedef struct SiDevPort {
	SiDevID devID;             /* Device ID */
	int devType;               /* Device type */
	int devClass;              /* Device class */
	char devName[SI_STRSIZE];  /* Device name */
	char portName[SI_MAXPORTNAME];
} SiDevPort;

typedef struct SiDeviceName {
	char name[SI_STRSIZE];
} SiDeviceName;

typedef struct SiButtonName {
	char name[SI_STRSIZE];
} SiButtonName;

typedef struct SiPortName {
	char name[SI_STRSIZE];
} SiPortName;

typedef enum SiDevTypes {
	SI_ALL_TYPES                 = -1,
	SI_UNKNOWN_DEVICE            =  0,
	SI_SPACEBALL_2003            =  1,
	SI_SPACEBALL_3003            =  2,
	SI_SPACE_CONTROLLER          =  3,
	SI_SPACEEXPLORER             =  4,
	SI_SPACENAVIGATOR_FOR_NOTEBOOKS =  5,
	SI_SPACENAVIGATOR            =  6,
	SI_SPACEBALL_2003A           =  7,
	SI_SPACEBALL_2003B           =  8,
	SI_SPACEBALL_2003C           =  9,
	SI_SPACEBALL_3003A           =  10,
	SI_SPACEBALL_3003B           =  11,
	SI_SPACEBALL_3003C           =  12,
	SI_SPACEBALL_4000            =  13,
	SI_SPACEMOUSE_CLASSIC        =  14, 
	SI_SPACEMOUSE_PLUS           =  15,
	SI_SPACEMOUSE_XT             =  16,
	SI_CYBERMAN                  =  17,
	SI_CADMAN                    =  18,
	SI_SPACEMOUSE_CLASSIC_PROMO  =  19,
	SI_SERIAL_CADMAN             =  20,
	SI_SPACEBALL_5000            =  21,
	SI_TEST_NO_DEVICE            =  22,
	SI_3DX_KEYBOARD_BLACK        =  23,
	SI_3DX_KEYBOARD_WHITE        =  24,
	SI_TRAVELER                  =  25,
	SI_TRAVELER1                 =  26,
	SI_SPACEBALL_5000A           =  27,
	SI_SPACEDRAGON               =  28,
	SI_SPACEPILOT                =  29,
	SI_MB                        =  30,
	SI_SPACEPILOT_PRO            =  0xc629,
	SI_SPACEMOUSE_PRO            =  0xc62b
} SiDevType;



enum SpwRetVal {
	SPW_NO_ERROR ,		/* no error */ 
	SPW_ERROR,			/* error -- function failed */ 
	SI_BAD_HANDLE,		/* invalid 3DxWare handle */ 
	SI_BAD_ID,			/* invalid device ID */ 
	SI_BAD_VALUE,		/* invalid argument value */ 
	SI_IS_EVENT,		/* event is a 3DxWare event */ 
	SI_SKIP_EVENT,		/* skip this 3DxWare event */ 
	SI_NOT_EVENT,		/* event is not a 3DxWare event */ 
	SI_NO_DRIVER,		/* 3DxWare driver is not running */ 
	SI_NO_RESPONSE,		/* 3DxWare driver is not responding */ 
	SI_UNSUPPORTED,		/* the function is unsupported by this version */ 
	SI_UNINITIALIZED,	/* 3DxWare Input Library is uninitialized */ 
	SI_WRONG_DRIVER,	/* incorrect driver for this 3DxWare version */ 
	SI_INTERNAL_ERROR	/* internal 3DxWare error */  
};

#pragma pack(pop)	/* restore default packing */
