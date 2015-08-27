#include "MouseDragOnStartOfRotation.h"
#include "JoystickConfigurator.h"

namespace SpaceNavigatorEmulator
{
	MouseDragOnStartOfRotation::MouseDragOnStartOfRotation(HWND window) :
		dragWindow(window)
	{
		JoystickConfigurator configurer;
		configurer.openConfiguration();
		enabled = configurer.GetFakeMouseMoveWhenRotating();
	}


	MouseDragOnStartOfRotation::~MouseDragOnStartOfRotation()
	{
	}

	bool MouseDragOnStartOfRotation::Process(JoyRecord& record)
	{
		if (enabled && previousRecord.IsCentered() && record.IsRotation())
		{
			// We are going to fake a middle mouse movement
			POINT p;
			if (GetCursorPos(&p) && ScreenToClient(dragWindow, &p))
			{
				DWORD position = MAKELPARAM(p.x, p.y);
				::PostMessage(dragWindow, WM_MBUTTONDOWN, MK_MBUTTON, position);

				DWORD move = MAKELPARAM(p.x + 10, p.y);
				::PostMessage(dragWindow, WM_MOUSEMOVE, MK_MBUTTON, move);
				::PostMessage(dragWindow, WM_MOUSEMOVE, MK_MBUTTON, position);

				::PostMessage(dragWindow, WM_MBUTTONUP, 0, position);
			}
		}

		return true;
	}
}