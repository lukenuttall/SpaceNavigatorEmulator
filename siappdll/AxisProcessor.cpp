#include "AxisProcessor.h"

namespace SpaceNavigatorEmulator
{

	AxisProcessor::AxisProcessor()
	{
	}


	AxisProcessor::~AxisProcessor()
	{
	}

	bool AxisProcessor::Process(JoyRecord& record, DIJOYSTATE* state)
	{
		record.x = buttonTester.GetAxis(SpaceNavigatorAction::PAN_X, state);
		record.y = buttonTester.GetAxis(SpaceNavigatorAction::PAN_Y, state);
		record.z = buttonTester.GetAxis(SpaceNavigatorAction::PAN_Z, state);
		record.rx = buttonTester.GetAxis(SpaceNavigatorAction::ROTATE_AROUND_X, state);
		record.ry = buttonTester.GetAxis(SpaceNavigatorAction::ROTATE_AROUND_Y, state);
		record.rz = buttonTester.GetAxis(SpaceNavigatorAction::ROTATE_AROUND_Z, state);
		return true;
	}
}
