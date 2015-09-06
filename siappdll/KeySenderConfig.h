#pragma once
#include "SpaceNavigatorAction.h"
#include <string>
#include <boost/property_tree/ptree.hpp>

namespace SpaceNavigatorEmulator
{
	class KeySenderConfig
	{
	public:

		bool operator<(const KeySenderConfig& rhs) const;

		std::string keys;

		SpaceNavigatorAction::Action drivenBy;

		int threshold;

		static KeySenderConfig load(const boost::property_tree::ptree& tree);
	};
}

