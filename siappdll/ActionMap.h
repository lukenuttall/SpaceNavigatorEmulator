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
#include <string>
#include <boost/property_tree/ptree.hpp>

#include <set>
#include <map>

#include "SpaceNavigatorAction.h"

namespace SpaceNavigatorEmulator
{
	class ActionMap
	{
	public:
		ActionMap();

		bool operator<(const ActionMap& rhs) const;

		std::string actionName;

		std::string joystickObject;

		unsigned int getMappingOffset() const
		{
			return mappingOffset;
		}

		SpaceNavigatorAction::Action getAction() const
		{
			return action;
		}

		bool IsInverted() const
		{
			return invertAxis;
		}

		static ActionMap load(const boost::property_tree::ptree& tree);

	private:

		bool invertAxis;

		unsigned int mappingOffset;

		SpaceNavigatorAction::Action action;

		void SetMappingOffset();

		std::map<std::string, size_t> offsets;		
	};
}

