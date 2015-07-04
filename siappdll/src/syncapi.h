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

extern "C" {

	typedef struct SiSyncAxesState {
		int state;
	} SiSyncAxesState;

	typedef enum SiSyncAbsFunctionNumber {
		SI_SYNC_FUNCTION_MENU_TOGGLE = 12,
		SI_SYNC_FUNCTION_TRANS_TOGGLE = 13,
		SI_SYNC_FUNCTION_ROT_TOGGLE = 14,
		SI_SYNC_FUNCTION_HPV_TOGGLE = 15,
		SI_SYNC_FUNCTION_DEC_SENS = 16,
		SI_SYNC_FUNCTION_INC_SENS = 17,
		SI_SYNC_FUNCTION_RESTORE_DEF = 18,
		SI_SYNC_FUNCTION_PAN = 19,
		SI_SYNC_FUNCTION_ZOOM = 20,
		SI_SYNC_FUNCTION_TX = 21,
		SI_SYNC_FUNCTION_TY = 22,
		SI_SYNC_FUNCTION_TZ = 23,
		SI_SYNC_FUNCTION_RX = 24,
		SI_SYNC_FUNCTION_RY = 25,
		SI_SYNC_FUNCTION_RZ = 26,
		SI_SYNC_FUNCTION_REZERO_DEVICE = 27,
		SI_SYNC_FUNCTION_SAVE = 33,
		SI_SYNC_FUNCTION_RELOAD = 57,
		SI_SYNC_FUNCTION_SHIFT_KEY = 60,
		SI_SYNC_FUNCTION_CTRL_KEY = 61,
		SI_SYNC_FUNCTION_ALT_KEY = 62,
		SI_SYNC_FUNCTION_RESTORE_SENS = 63,
		SI_SYNC_FUNCTION_SPACE_KEY = 64,
		SI_SYNC_FUNCTION_CTRL_SHIFT_KEY = 65,
		SI_SYNC_FUNCTION_CTRL_ALT_KEY = 66,
		SI_SYNC_FUNCTION_SHIFT_ALT_KEY = 67,
		SI_SYNC_FUNCTION_TAB_KEY = 68,
		SI_SYNC_FUNCTION_RETURN_KEY = 69,
		SI_SYNC_FUNCTION_DEC_TRANS_SENS = 70,
		SI_SYNC_FUNCTION_INC_TRANS_SENS = 71,
		SI_SYNC_FUNCTION_DEC_ROT_SENS = 72,
		SI_SYNC_FUNCTION_INC_ROT_SENS = 73,
		SI_SYNC_FUNCTION_DEC_PAN_SENS = 74,
		SI_SYNC_FUNCTION_INC_PAN_SENS = 75,
		SI_SYNC_FUNCTION_DEC_ZOOM_SENS = 76,
		SI_SYNC_FUNCTION_INC_ZOOM_SENS = 77,
		SI_SYNC_FUNCTION_ESC_KEY = 78,
		SI_SYNC_FUNCTION_3DX_HELP = 94,
		SI_SYNC_FUNCTION_APP_HELP = 95,
		SI_SYNC_FUNCTION_DIALOG_TOGGLE_FN = 96,
		SI_SYNC_FUNCTION_FIT_FN = 97,
		SI_SYNC_FUNCTION_TOGGLE_3DXLCD_FN = 198,
		SI_SYNC_FUNCTION_TOGGLE_3DXNUMPAD_FN = 199
	} SiSyncAbsFunctionNumber;


	typedef enum SiSyncFilter {
		SI_SYNC_FILTER_TRANSLATIONS = 1,
		SI_SYNC_FILTER_ROTATIONS = 2,
		SI_SYNC_FILTER_DOMINANT = 3
	} SiSyncFilter;

	typedef enum SiSyncFilterValue {
		SI_SYNC_FILTER_OFF = 0,
		SI_SYNC_FILTER_ON = 1,
		SI_SYNC_FILTER_IN_BETWEEN = 2
	} SiSyncFilterValue;

	typedef enum SiSyncAxesStateBits {
		SI_SYNC_AXES_STATE_TX = (1 << 0),
		SI_SYNC_AXES_STATE_TY = (1 << 1),
		SI_SYNC_AXES_STATE_TZ = (1 << 2),
		SI_SYNC_AXES_STATE_RX = (1 << 3),
		SI_SYNC_AXES_STATE_RY = (1 << 4),
		SI_SYNC_AXES_STATE_RZ = (1 << 5)
	} SiSyncAxesStateBits;

	typedef enum SiSyncButtonState {
		SI_SYNC_BUTTON_STATE_OFF = 0,
		SI_SYNC_BUTTON_STATE_ON = 1,
		SI_SYNC_BUTTON_STATE_DISABLED = 2
	} SiSyncButtonState;

	SpwRetVal SPWAPI SiSyncSendQuery(struct SiHdl *si);
	SpwRetVal SPWAPI SiSyncGetVersion(struct SiHdl *si, SPWuint32 *major, SPWuint32 *minor);
	SpwRetVal SPWAPI SiSyncGetNumberOfFunctions(struct SiHdl *si, SPWuint32 *num_func);
	SpwRetVal SPWAPI SiSyncGetFunction(struct SiHdl *hdl, SPWuint32 index, SPWint32 *abs_func_num, wchar_t *name, SPWuint32 *name_len);
	SpwRetVal SPWAPI SiSyncGetButtonAssignment(struct SiHdl *hdl, SPWuint32 bn_num, SPWint32 *assigned_func_index);
	SpwRetVal SPWAPI SiSyncSetButtonAssignment(struct SiHdl *hdl, SPWuint32 bn_num, SPWint32 func_index);
	SpwRetVal SPWAPI SiSyncSetButtonAssignmentAbsolute(struct SiHdl *hdl, SPWuint32 bn_num, SPWint32 abs_func_num);
	SpwRetVal SPWAPI SiSyncSetButtonName(struct SiHdl *hdl, SPWuint32 bn_num, const wchar_t *name);
	SpwRetVal SPWAPI SiSyncGetAxisLabel(struct SiHdl *hdl, SPWuint32 axis_num, wchar_t *name, SPWuint32 *name_len);
	SpwRetVal SPWAPI SiSyncSetAxisLabel(struct SiHdl *hdl, SPWuint32 axis_num, const wchar_t *name);
	SpwRetVal SPWAPI SiSyncGetOrientation(struct SiHdl *hdl, SPWint32 axes[6]);
	SpwRetVal SPWAPI SiSyncSetOrientation(struct SiHdl *hdl, const SPWint32 axes[6]);
	SpwRetVal SPWAPI SiSyncGetFilter(struct SiHdl *hdl, SiSyncFilter i, SiSyncFilterValue *pv);
	SpwRetVal SPWAPI SiSyncSetFilter(struct SiHdl *hdl, SiSyncFilter i, SiSyncFilterValue v);
	SpwRetVal SPWAPI SiSyncGetAxesState(struct SiHdl *hdl, SiSyncAxesState *pa);
	SpwRetVal SPWAPI SiSyncSetAxesState(struct SiHdl *hdl, SiSyncAxesState a);
	SpwRetVal SPWAPI SiSyncSetInfoLine(struct SiHdl *hdl, SPWint32 duration, const wchar_t *text);
	SpwRetVal SPWAPI SiSyncGetScaleOverall(struct SiHdl *hdl, SPWfloat32 *pv);
	SpwRetVal SPWAPI SiSyncSetScaleOverall(struct SiHdl *hdl, SPWfloat32 v);
	SpwRetVal SPWAPI SiSyncGetScaleTx(struct SiHdl *hdl, SPWfloat32 *pv);
	SpwRetVal SPWAPI SiSyncSetScaleTx(struct SiHdl *hdl, SPWfloat32 v);
	SpwRetVal SPWAPI SiSyncGetScaleTy(struct SiHdl *hdl, SPWfloat32 *pv);
	SpwRetVal SPWAPI SiSyncSetScaleTy(struct SiHdl *hdl, SPWfloat32 v);
	SpwRetVal SPWAPI SiSyncGetScaleTz(struct SiHdl *hdl, SPWfloat32 *pv);
	SpwRetVal SPWAPI SiSyncSetScaleTz(struct SiHdl *hdl, SPWfloat32 v);
	SpwRetVal SPWAPI SiSyncGetScaleRx(struct SiHdl *hdl, SPWfloat32 *pv);
	SpwRetVal SPWAPI SiSyncSetScaleRx(struct SiHdl *hdl, SPWfloat32 v);
	SpwRetVal SPWAPI SiSyncGetScaleRy(struct SiHdl *hdl, SPWfloat32 *pv);
	SpwRetVal SPWAPI SiSyncSetScaleRy(struct SiHdl *hdl, SPWfloat32 v);
	SpwRetVal SPWAPI SiSyncGetScaleRz(struct SiHdl *hdl, SPWfloat32 *pv);
	SpwRetVal SPWAPI SiSyncSetScaleRz(struct SiHdl *hdl, SPWfloat32 v);
	SpwRetVal SPWAPI SiSyncInvokeAbsoluteFunction(struct SiHdl *hdl, SiSyncAbsFunctionNumber i);
	SpwRetVal SPWAPI SiSyncSetButtonState(struct SiHdl *hdl, SPWuint32 bn_num, SiSyncButtonState state);

}