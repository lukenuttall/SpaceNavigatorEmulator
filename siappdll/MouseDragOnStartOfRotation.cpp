#include "MouseDragOnStartOfRotation.h"
#include "JoystickConfigurator.h"
#include "Log.h"

namespace SpaceNavigatorEmulator
{
	MouseDragOnStartOfRotation::MouseDragOnStartOfRotation(HWND window)
	{
		dragWindow = window;
		JoystickConfigurator configurer;
		configurer.openConfiguration();
		enabled = configurer.GetFakeMouseMoveWhenRotating();
		LOG(TRACE) << "Fake mouse move: " << enabled;
	}


	MouseDragOnStartOfRotation::~MouseDragOnStartOfRotation()
	{
	}

	bool MouseDragOnStartOfRotation::Process(JoyRecord& record, DIJOYSTATE* state)
	{
		if (enabled)
		{
			if (record.IsRotation() && previousRecord.IsCentered())
			{
				LOG(TRACE) << "Sending fake mouse move on drag";
				// We are going to fake a middle mouse movement
				POINT p;
				if (GetCursorPos(&p) && ScreenToClient(dragWindow, &p))
				{
					LOG(TRACE) << "x: " << p.x << " y: " << p.y;
					DWORD position = MAKELPARAM(p.x, p.y);
					::PostMessage(dragWindow, WM_MBUTTONDOWN, MK_MBUTTON, position);

					DWORD move = MAKELPARAM(p.x + 10, p.y);
					::PostMessage(dragWindow, WM_MOUSEMOVE, MK_MBUTTON, move);
					::PostMessage(dragWindow, WM_MOUSEMOVE, MK_MBUTTON, position);

					::PostMessage(dragWindow, WM_MBUTTONUP, MK_MBUTTON, position);
				}
			}
			previousRecord = record;
		}
		return true;
	}
}