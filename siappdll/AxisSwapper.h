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
		void GetAxis(SpaceNavigatorAction::Action action, DIJOYSTATE* state, LONG& record);

		std::vector<ModeReplacement> axisSwaps;

		ButtonTester buttonTester;
	};
}

