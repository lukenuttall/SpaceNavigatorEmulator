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
			record.x = GetAxis(SpaceNavigatorAction::PAN_X, state, swapAxes);
			record.y = GetAxis(SpaceNavigatorAction::PAN_Y, state, swapAxes);
			record.z = GetAxis(SpaceNavigatorAction::PAN_Z, state, swapAxes);
			record.rx = GetAxis(SpaceNavigatorAction::ROTATE_AROUND_X, state, swapAxes);
			record.ry = GetAxis(SpaceNavigatorAction::ROTATE_AROUND_Y, state, swapAxes);
			record.rz = GetAxis(SpaceNavigatorAction::ROTATE_AROUND_Z, state, swapAxes);
		}
		return true;
	}

	LONG AxisSwapper::GetAxis(SpaceNavigatorAction::Action action, DIJOYSTATE* state, bool swapAxis)
	{
		bool invertRequiredBySwap = false;

		// Check if we need to swap this axis
		if (swapAxis)
		{
			BOOST_FOREACH(ModeReplacement& mode, axisSwaps)
			{
				// If we are swapping and the axis normally driving this action is swapped out return 0
				if (mode.drivenBy == action)
				{
					return 0;
				}
				if (mode.action == action)
				{
					LOG(TRACE) << "Using " << mode.drivenBy << " to provide " << action;
					action = mode.drivenBy;
					invertRequiredBySwap = mode.invertDrivingAxis;
				}
			}
		}

		return buttonTester.GetAxis(action, state, invertRequiredBySwap);
	}
}