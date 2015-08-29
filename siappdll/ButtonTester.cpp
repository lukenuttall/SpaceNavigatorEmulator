#include "ButtonTester.h"
#include "JoystickConfigurator.h"

#include <boost\foreach.hpp>

namespace SpaceNavigatorEmulator
{

	ButtonTester::ButtonTester()
	{
		JoystickConfigurator configurator;
		auto configuratorActions = configurator.GetActions();
		BOOST_FOREACH(ActionMap const& map, configuratorActions)
		{
			std::shared_ptr<ActionDetails> details(new ActionDetails(map.getMappingOffset(), map.IsInverted()));

			actions[map.getAction()] = details;
		}
	}


	ButtonTester::~ButtonTester()
	{
	}

	bool ButtonTester::IsButtonDown(SpaceNavigatorAction::Action action, DIJOYSTATE* state)
	{
		auto actionIter = actions.find(action);
		if (actionIter != actions.end())
		{
			unsigned int mappingOffset = actionIter->second->mappingOffset;
			auto pointer = ((BYTE *)state + mappingOffset);
			return (*pointer & 0x80) != 0;
		}
		return 0;
	}

	LONG ButtonTester::GetAxis(SpaceNavigatorAction::Action action, DIJOYSTATE* state, bool invertRequired)
	{
		auto actionIter = actions.find(action);
		if (actionIter != actions.end())
		{
			auto map = actionIter->second;
			unsigned int mappingOffset = map->mappingOffset;
			auto pointer = (LONG*)((BYTE *)state + mappingOffset);
			auto axis = *pointer;
			if (map->axisInverted || invertRequired)
			{
				axis = -axis;
			}
			return axis;
		}
		return 0;
	}
}