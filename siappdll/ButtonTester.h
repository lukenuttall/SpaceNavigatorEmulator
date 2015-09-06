#pragma once
#include "SpaceNavigatorAction.h"
#include "ActionDetails.h"

#include <dinput.h>
#include <map>
#include <memory>

namespace SpaceNavigatorEmulator
{
	class ButtonTester
	{
	public:
		ButtonTester();
		~ButtonTester();

		bool IsButtonDown(SpaceNavigatorAction::Action action, DIJOYSTATE* state);		

		LONG GetAxis(SpaceNavigatorAction::Action action, DIJOYSTATE* state, bool invertRequired = false);

	private:
		std::map<SpaceNavigatorAction::Action, std::shared_ptr<ActionDetails>> actions;
	};
}

