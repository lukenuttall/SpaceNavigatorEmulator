#pragma once
#include "IMode.h"
#include "ButtonTester.h"
#include <vector>
#include "KeySenderConfig.h"

namespace SpaceNavigatorEmulator
{
	class KeySender :
		public IMode
	{
	public:
		KeySender();
		~KeySender();

		virtual bool Process(JoyRecord& record, DIJOYSTATE* state);

	private:
		std::vector<KeySenderConfig> keySenders;

		ButtonTester buttonTester;
	};
}
