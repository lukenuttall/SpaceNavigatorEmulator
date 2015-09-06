#pragma once
#include "IMode.h"
#include "ButtonTester.h"

namespace SpaceNavigatorEmulator
{
	class AxisProcessor : public IMode
	{
	public:
		AxisProcessor();
		~AxisProcessor();

		virtual bool Process(JoyRecord& record, DIJOYSTATE* state);

	private:
		ButtonTester buttonTester;
	};

}

