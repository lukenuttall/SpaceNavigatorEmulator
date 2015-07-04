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
#include "DeviceConfig.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "Log.h"
#include <boost/foreach.hpp>
#include <exception>

namespace SpaceNavigatorEmulator
{
	using boost::property_tree::ptree;

	void DeviceConfig::load(std::string filename)
	{
		// Create an empty property tree object
		ptree pt;
		read_json(filename, pt);
		
		try
		{
			deviceName = pt.get<std::string>("device.deviceName");

			deadzone = pt.get<long>("device.deadzone", 2000);
			
			axisRange = pt.get<long>("device.axisRange", 350);

			fakeMouseMoveWhenRotating = pt.get<bool>("device.fakeMouseMoveWhenRotating");

			BOOST_FOREACH(ptree::value_type &v, pt.get_child("device.actionMap"))
				actions.insert(ActionMap::load(v.second));
			
			BOOST_FOREACH(ptree::value_type &v, pt.get_child("device.modeReplacement"))
				modeReplacements.insert(ModeReplacement::load(v.second));
		}
		catch (std::exception e)
		{
			LOG(ERROR) << "Could not load action mapping file" << e.what();
		}
	}

}