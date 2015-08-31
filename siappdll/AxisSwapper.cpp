#include "AxisSwapper.h"
#include "JoystickConfigurator.h"
#include "Log.h"

#include <boost\foreach.hpp>

namespace SpaceNavigatorEmulator
{

	AxisSwapper::AxisSwapper()
	{
		JoystickConfigurator configurer;
		configurer.openConfiguration();
		BOOST_FOREACH(const ModeReplacement& swap, configurer.GetModeReplacements())
		{
			axisSwaps.push_back(swap);
		}
	}


	AxisSwapper::~AxisSwapper()
	{
	}

	bool AxisSwapper::Process(JoyRecord& record, DIJOYSTATE* state)
	{
		bool swapAxes = buttonTester.IsButtonDown(SpaceNavigatorAction::BUTTON_MODE_SWAP, state);
		if (swapAxes)
		{
			LOG(TRACE) << "Axis wap enabled - swapping";
			GetAxis(SpaceNavigatorAction::PAN_X, state, record.x);
			GetAxis(SpaceNavigatorAction::PAN_Y, state, record.y);
			GetAxis(SpaceNavigatorAction::PAN_Z, state, record.z);
			GetAxis(SpaceNavigatorAction::ROTATE_AROUND_X, state, record.rx);
			GetAxis(SpaceNavigatorAction::ROTATE_AROUND_Y, state, record.ry);
			GetAxis(SpaceNavigatorAction::ROTATE_AROUND_Z, state, record.rz);
		}
		return true;
	}

	void AxisSwapper::GetAxis(SpaceNavigatorAction::Action action, DIJOYSTATE* state, LONG& value)
	{
		BOOST_FOREACH(ModeReplacement& mode, axisSwaps)
		{
			// If we are swapping and the axis normally driving this action is swapped out return 0
			if (mode.drivenBy == action)
			{
				value = 0;
			}
			if (mode.action == action)
			{
				LOG(TRACE) << "Using " << mode.drivenBy << " to provide " << action;
				value = buttonTester.GetAxis(mode.drivenBy, state, mode.invertDrivingAxis);
			}
		}
	}
}