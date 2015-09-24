#include "KeySender.h"
#include "JoystickConfigurator.h"
#include <boost\foreach.hpp>
#include "Log.h"

#include <boost/algorithm/string/classification.hpp> 
#include <boost/algorithm/string/split.hpp>

namespace SpaceNavigatorEmulator
{
	KeySender::KeySender(HWND window) :
		keyWindow(window)
	{
		JoystickConfigurator configurer;
		configurer.openConfiguration();
		BOOST_FOREACH(const KeySenderConfig& keySender, configurer.GetKeySenders())
		{
			keySenders.push_back(keySender);
		}
	}


	KeySender::~KeySender()
	{
	}

	bool KeySender::Process(JoyRecord& record, DIJOYSTATE* state)
	{
		bool sendKeys = buttonTester.IsButtonDown(SpaceNavigatorAction::BUTTON_SEND_KEYS, state);
		if (sendKeys)
		{
			LOG(TRACE) << "key sending enabled";
			BOOST_FOREACH(KeySenderConfig& sender, keySenders)
			{
				auto value = buttonTester.GetAxis(sender.drivenBy, state);
				LOG(TRACE) << "Axis: " << sender.drivenBy << " value:" << value << " threshold: " << sender.threshold;
				if ((sender.threshold > 0 && value > sender.threshold) ||
					(sender.threshold < 0 && value < sender.threshold))
				{
					LOG(TRACE) << "sending keys " << sender.keys;

					// Split the keys into a vector
					std::vector<std::string> keys;
					boost::split(keys, sender.keys, boost::is_any_of("+"), boost::token_compress_on);

					// Press each key in sequence
					BOOST_FOREACH(auto key, keys)
					{
						int vk = mapper.getVK(key);
						if (vk != 0)
							SendKey(vk);
					}

					// Release keys in reverse order
					BOOST_REVERSE_FOREACH(auto key, keys)
					{
						int vk = mapper.getVK(key);
						if (vk != 0)
							SendKey(vk, true);
					}
				}
			}
		}
		return sendKeys;
	}

	void KeySender::SendKey(int virtualKey, bool up)
	{
		INPUT input;
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = virtualKey;
		input.ki.wScan = 0;
		input.ki.time = 0;
		input.ki.dwFlags = up ? KEYEVENTF_KEYUP : 0;

		::SendInput(1, &input, sizeof(input));
	}
}