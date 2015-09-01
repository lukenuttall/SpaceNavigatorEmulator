#include "KeySenderConfig.h"

namespace SpaceNavigatorEmulator
{

	using namespace boost::property_tree;

	KeySenderConfig KeySenderConfig::load(const ptree& tree)
	{
		KeySenderConfig sender;

		auto drivenBy = tree.get<std::string>("drivenBy");

		sender.drivenBy = SpaceNavigatorAction::parseAction(drivenBy);
		sender.threshold = tree.get<int>("threshold");
		sender.keys = tree.get<std::string>("keys");

		return sender;
	}

	bool KeySenderConfig::operator<(const KeySenderConfig& rhs) const
	{
		// nasty - This should not be a string compare.
		return this->drivenBy + " " + this->keys < rhs.drivenBy + " " + rhs.keys;
	}
}