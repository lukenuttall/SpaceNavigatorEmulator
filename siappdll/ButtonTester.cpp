#include "ButtonTester.h"
#include "JoystickConfigurator.h"

#include <boost\foreach.hpp>
#include "Log.h"

namespace SpaceNavigatorEmulator
{

	ButtonTester::ButtonTester()
	{
		LOG(TRACE) << "Creating ButtonTester";
		JoystickConfigurator configurator;
		configurator.openConfiguration();
		auto configuratorActions = configurator.GetActions();
		BOOST_FOREACH(ActionMap const& map, configuratorActions)
		{
			LOG(TRACE) << "Creating map for " << map.actionName << " id " << map.getAction();
			actions[map.getAction()] = std::make_shared<ActionDetails>(ActionDetails(map.getMappingOffset(), map.IsInverted()));
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
		LOG(TRACE) << "Testing axis for " << action;
		BOOST_FOREACH(auto act, actions)
		{
			LOG(TRACE) << act.first;
			LOG(TRACE) << "Offset" << act.second->mappingOffset;
			LOG(TRACE) << "Axis" << act.second->axisInverted;
		}
		auto actionIter = actions.find(action);
		if (actionIter != actions.end())
		{
			unsigned int mappingOffset = actionIter->second->mappingOffset;
			auto pointer = (LONG*)((BYTE *)state + mappingOffset);
			auto axis = *pointer;
			if (actionIter->second->axisInverted || invertRequired)
			{
				axis = -axis;
			}
			return axis;
		}
		LOG(TRACE) << "Returning default axis value = 0";
		return 0;
	}
}