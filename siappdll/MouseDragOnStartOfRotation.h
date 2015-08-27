#pragma once
#include "IMode.h"

namespace SpaceNavigatorEmulator
{
	class MouseDragOnStartOfRotation : public IMode
	{
	public:
		MouseDragOnStartOfRotation(HWND window);
		~MouseDragOnStartOfRotation();

		virtual bool Process(JoyRecord& record);

	private:
		JoyRecord previousRecord;
		HWND dragWindow;
		bool enabled;
	};
}
