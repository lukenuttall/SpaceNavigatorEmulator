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
#include "syncapi.h"
#include "..\Log.h"

SpwRetVal SPWAPI SiSyncSendQuery(struct SiHdl *si)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncGetVersion(struct SiHdl *si, SPWuint32 *major, SPWuint32 *minor)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncGetNumberOfFunctions(struct SiHdl *si, SPWuint32 *num_func)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncGetFunction(struct SiHdl *hdl, SPWuint32 index, SPWint32 *abs_func_num, wchar_t *name, SPWuint32 *name_len)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncGetButtonAssignment(struct SiHdl *hdl, SPWuint32 bn_num, SPWint32 *assigned_func_index)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncSetButtonAssignment(struct SiHdl *hdl, SPWuint32 bn_num, SPWint32 func_index)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncSetButtonAssignmentAbsolute(struct SiHdl *hdl, SPWuint32 bn_num, SPWint32 abs_func_num)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncSetButtonName(struct SiHdl *hdl, SPWuint32 bn_num, const wchar_t *name)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncGetAxisLabel(struct SiHdl *hdl, SPWuint32 axis_num, wchar_t *name, SPWuint32 *name_len)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncSetAxisLabel(struct SiHdl *hdl, SPWuint32 axis_num, const wchar_t *name)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncGetOrientation(struct SiHdl *hdl, SPWint32 axes[6])
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncSetOrientation(struct SiHdl *hdl, const SPWint32 axes[6])
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncGetFilter(struct SiHdl *hdl, SiSyncFilter i, SiSyncFilterValue *pv)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncSetFilter(struct SiHdl *hdl, SiSyncFilter i, SiSyncFilterValue v)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncGetAxesState(struct SiHdl *hdl, SiSyncAxesState *pa)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncSetAxesState(struct SiHdl *hdl, SiSyncAxesState a)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncSetInfoLine(struct SiHdl *hdl, SPWint32 duration, const wchar_t *text)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncGetScaleOverall(struct SiHdl *hdl, SPWfloat32 *pv)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncSetScaleOverall(struct SiHdl *hdl, SPWfloat32 v)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncGetScaleTx(struct SiHdl *hdl, SPWfloat32 *pv)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncSetScaleTx(struct SiHdl *hdl, SPWfloat32 v)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncGetScaleTy(struct SiHdl *hdl, SPWfloat32 *pv)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncSetScaleTy(struct SiHdl *hdl, SPWfloat32 v)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncGetScaleTz(struct SiHdl *hdl, SPWfloat32 *pv)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncSetScaleTz(struct SiHdl *hdl, SPWfloat32 v)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncGetScaleRx(struct SiHdl *hdl, SPWfloat32 *pv)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncSetScaleRx(struct SiHdl *hdl, SPWfloat32 v)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncGetScaleRy(struct SiHdl *hdl, SPWfloat32 *pv)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncSetScaleRy(struct SiHdl *hdl, SPWfloat32 v)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncGetScaleRz(struct SiHdl *hdl, SPWfloat32 *pv)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncSetScaleRz(struct SiHdl *hdl, SPWfloat32 v)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncInvokeAbsoluteFunction(struct SiHdl *hdl, SiSyncAbsFunctionNumber i)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	
	return SPW_NO_ERROR;
}

enum SpwRetVal SPWAPI SiSyncSetButtonState(struct SiHdl *hdl, SPWuint32 bn_num, SiSyncButtonState state)
{
	LOG(TRACE) << "Stub called: " << __FUNCTION__;
	
	return SPW_NO_ERROR;
}