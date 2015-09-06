#pragma once
#include <map>

namespace SpaceNavigatorEmulator
{
	class KeyMapper
	{
	public:
		KeyMapper();
		
		int getVK(std::string keyName);

	private:

		void LoadVirtualKeys();

		std::map<std::string, int> descritionToVK;
	};
}

