#pragma once
#include "IMode.h"
#include "ButtonTester.h"
#include <vector>
#include "KeySenderConfig.h"
#include "KeyMapper.h"

namespace SpaceNavigatorEmulator
{
	class KeySender :
		public IMode
	{
	public:
		KeySender(HWND window);
		~KeySender();

		virtual bool Process(JoyRecord& record, DIJOYSTATE* state);

	private:
		std::vector<KeySenderConfig> keySenders;

		void SendKey(int virtualKey, bool up = false);

		HWND keyWindow;

		ButtonTester buttonTester;

		KeyMapper mapper;
	};
}
