#pragma once
#include "IMode.h"
#include "ModeReplacement.h"
#include "ButtonTester.h"

#include <vector>

namespace SpaceNavigatorEmulator
{
	class AxisSwapper : public IMode
	{
	public:
		AxisSwapper();
		virtual ~AxisSwapper();

		virtual bool Process(JoyRecord& record, DIJOYSTATE* state);

	private:
		LONG GetAxis(SpaceNavigatorAction::Action action, DIJOYSTATE* state, bool swapAxis);

		std::vector<ModeReplacement> axisSwaps;

		ButtonTester buttonTester;
	};
}

