#include "KeySender.h"
#include "JoystickConfigurator.h"
#include <boost\foreach.hpp>
#include "Log.h"

namespace SpaceNavigatorEmulator
{
	KeySender::KeySender()
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
					LOG(TRACE) << "Pretending to send keys " << sender.keys;
				}
			}
		}
		return true;
	}
}