#pragma once
#include "IMode.h"

namespace SpaceNavigatorEmulator
{
	class KeySender :
		public IMode
	{
	public:
		KeySender();
		~KeySender();
	};
}
