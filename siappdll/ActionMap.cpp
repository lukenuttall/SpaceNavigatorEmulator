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

#include "ActionMap.h"
#include <iostream>
#include <boost/foreach.hpp>
#include <dinput.h>
#include <map>
#include <string>
#include "Log.h"

namespace SpaceNavigatorEmulator
{
	using namespace std;
	using namespace boost;
	using namespace boost::property_tree;

	ActionMap::ActionMap()
	{
		offsets["lX"] = offsetof(DIJOYSTATE, lX);
		offsets["lY"] = offsetof(DIJOYSTATE, lY);
		offsets["lZ"] = offsetof(DIJOYSTATE, lZ);
		offsets["lRx"] = offsetof(DIJOYSTATE, lRx);
		offsets["lRy"] = offsetof(DIJOYSTATE, lRy);
		offsets["lRz"] = offsetof(DIJOYSTATE, lRz);
		for (int i = 0; i < 2; i++)
			offsets[string("rglSlider").append(to_string(i))] = offsetof(DIJOYSTATE, rglSlider) + i*sizeof(LONG);

		for (int i = 0; i < 4; i++)
			offsets[string("rgdwPOW").append(to_string(i))] = offsetof(DIJOYSTATE, rgdwPOV) + i*sizeof(DWORD);

		for (int i = 0; i < 32; i++)
			offsets[string("rgbButtons").append(to_string(i))] = offsetof(DIJOYSTATE, rgbButtons) + i*sizeof(BYTE);
	}
	
	ActionMap ActionMap::load(const ptree& tree)
	{
		ActionMap map;

		map.actionName = tree.get<std::string>("actionName");
		map.action = SpaceNavigatorAction::parseAction(map.actionName);

		map.joystickObject = tree.get<std::string>("joystickObject"); 
		map.invertAxis = tree.get<bool>("invertAxis", false);
		
		map.SetMappingOffset();

		return map;
	}

	bool ActionMap::operator<(const ActionMap& rhs) const
	{
		return this->actionName < rhs.actionName;
	}


	void ActionMap::SetMappingOffset()
	{
		auto iter = offsets.find(joystickObject);
		if (iter != offsets.end())
		{
			mappingOffset = iter->second;
		}
		else
		{
			LOG(ERROR) << "Could not map " << joystickObject << " to DIJOYSTATE";
		}
	}
}