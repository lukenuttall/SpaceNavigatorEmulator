#include "KeyMapper.h"
#include <boost\foreach.hpp>

namespace SpaceNavigatorEmulator
{
	KeyMapper::KeyMapper()
	{
		LoadVirtualKeys();
	}

	void KeyMapper::LoadVirtualKeys()
	{
		// From https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
		struct hhb_vkey{
			char* name;
			unsigned int vkey;
			char* description;
		};
		const hhb_vkey hhb_vkey_list[] = {
			{ "VK_CANCEL", 0x03, "Control-break processing" },
			{ "-", 0x07, "Undefined" },
			{ "VK_BACK", 0x08, "BACKSPACE" },
			{ "VK_TAB", 0x09, "TAB" },
			{ "VK_CLEAR", 0x0C, "CLEAR" },
			{ "VK_RETURN", 0x0D, "ENTER" },
			{ "VK_SHIFT", 0x10, "SHIFT" },
			{ "VK_CONTROL", 0x11, "CTRL" },
			{ "VK_MENU", 0x12, "ALT" },
			{ "VK_PAUSE", 0x13, "PAUSE" },
			{ "VK_CAPITAL", 0x14, "CAPS LOCK" },
			{ "VK_ESCAPE", 0x1B, "ESC" },
			{ "VK_SPACE", 0x20, "SPACEBAR" },
			{ "VK_PRIOR", 0x21, "PAGE UP" },
			{ "VK_NEXT", 0x22, "PAGE DOWN" },
			{ "VK_END", 0x23, "END" },
			{ "VK_HOME", 0x24, "HOME" },
			{ "VK_LEFT", 0x25, "LEFT ARROW" },
			{ "VK_UP", 0x26, "UP ARROW" },
			{ "VK_RIGHT", 0x27, "RIGHT ARROW" },
			{ "VK_DOWN", 0x28, "DOWN ARROW" },
			{ "VK_SELECT", 0x29, "SELECT" },
			{ "VK_PRINT", 0x2A, "PRINT" },
			{ "VK_EXECUTE", 0x2B, "EXECUTE" },
			{ "VK_SNAPSHOT", 0x2C, "PRINT SCREEN" },
			{ "VK_INSERT", 0x2D, "INS" },
			{ "VK_DELETE", 0x2E, "DEL" },
			{ "VK_HELP", 0x2F, "HELP" },
			{ "0", 0x30, "0" },
			{ "1", 0x31, "1" },
			{ "2", 0x32, "2" },
			{ "3", 0x33, "3" },
			{ "4", 0x34, "4" },
			{ "5", 0x35, "5" },
			{ "6", 0x36, "6" },
			{ "7", 0x37, "7" },
			{ "8", 0x38, "8" },
			{ "9", 0x39, "9" },
			//{ "undefined", 0x3A - 40, "undefined" },
			{ "A", 0x41, "A" },
			{ "B", 0x42, "B" },
			{ "C", 0x43, "C" },
			{ "D", 0x44, "D" },
			{ "E", 0x45, "E" },
			{ "F", 0x46, "F" },
			{ "G", 0x47, "G" },
			{ "H", 0x48, "H" },
			{ "I", 0x49, "I" },
			{ "J", 0x4A, "J" },
			{ "K", 0x4B, "K" },
			{ "L", 0x4C, "L" },
			{ "M", 0x4D, "M" },
			{ "N", 0x4E, "N" },
			{ "O", 0x4F, "O" },
			{ "P", 0x50, "P" },
			{ "Q", 0x51, "Q" },
			{ "R", 0x52, "R" },
			{ "S", 0x53, "S" },
			{ "T", 0x54, "T" },
			{ "U", 0x55, "U" },
			{ "V", 0x56, "V" },
			{ "W", 0x57, "W" },
			{ "X", 0x58, "X" },
			{ "Y", 0x59, "Y" },
			{ "Z", 0x5A, "Z" },
			{ "VK_LWIN", 0x5B, "Left Windows (Naturalboard)" },
			{ "VK_RWIN", 0x5C, "Right Windows (Naturalboard)" },
			{ "VK_APPS", 0x5D, "Applications (Naturalboard)" },
			//{ "-", 0x5E, "Reserved" },
			{ "VK_SLEEP", 0x5F, "Computer Sleep" },
			{ "VK_NUMPAD0", 0x60, "Numericpad 0" },
			{ "VK_NUMPAD1", 0x61, "Numericpad 1" },
			{ "VK_NUMPAD2", 0x62, "Numericpad 2" },
			{ "VK_NUMPAD3", 0x63, "Numericpad 3" },
			{ "VK_NUMPAD4", 0x64, "Numericpad 4" },
			{ "VK_NUMPAD5", 0x65, "Numericpad 5" },
			{ "VK_NUMPAD6", 0x66, "Numericpad 6" },
			{ "VK_NUMPAD7", 0x67, "Numericpad 7" },
			{ "VK_NUMPAD8", 0x68, "Numericpad 8" },
			{ "VK_NUMPAD9", 0x69, "Numericpad 9" },
			{ "VK_MULTIPLY", 0x6A, "Multiply" },
			{ "VK_ADD", 0x6B, "Add" },
			{ "VK_SEPARATOR", 0x6C, "Separator" },
			{ "VK_SUBTRACT", 0x6D, "Subtract" },
			{ "VK_DECIMAL", 0x6E, "Decimal" },
			{ "VK_DIVIDE", 0x6F, "Divide" },
			{ "VK_F1", 0x70, "F1" },
			{ "VK_F2", 0x71, "F2" },
			{ "VK_F3", 0x72, "F3" },
			{ "VK_F4", 0x73, "F4" },
			{ "VK_F5", 0x74, "F5" },
			{ "VK_F6", 0x75, "F6" },
			{ "VK_F7", 0x76, "F7" },
			{ "VK_F8", 0x77, "F8" },
			{ "VK_F9", 0x78, "F9" },
			{ "VK_F10", 0x79, "F10" },
			{ "VK_F11", 0x7A, "F11" },
			{ "VK_F12", 0x7B, "F12" },
			{ "VK_F13", 0x7C, "F13" },
			{ "VK_F14", 0x7D, "F14" },
			{ "VK_F15", 0x7E, "F15" },
			{ "VK_F16", 0x7F, "F16" },
			{ "VK_F17", 0x80, "F17" },
			{ "VK_F18", 0x81, "F18" },
			{ "VK_F19", 0x82, "F19" },
			{ "VK_F20", 0x83, "F20" },
			{ "VK_F21", 0x84, "F21" },
			{ "VK_F22", 0x85, "F22" },
			{ "VK_F23", 0x86, "F23" },
			{ "VK_F24", 0x87, "F24" },
			//{ "-", 0x88 - 8F, "Unassigned" },
			{ "VK_NUMLOCK", 0x90, "NUM LOCK" },
			{ "VK_SCROLL", 0x91, "SCROLL LOCK" },
			//{"-", 0x92-96,"OEM specific"},
			//{ "-", 0x97 - 9F, "Unassigned" },
			{ "VK_LSHIFT", 0xA0, "Left SHIFT" },
			{ "VK_RSHIFT", 0xA1, "Right SHIFT" },
			{ "VK_LCONTROL", 0xA2, "Left CONTROL" },
			{ "VK_RCONTROL", 0xA3, "Right CONTROL" },
			{ "VK_LMENU", 0xA4, "Left MENU" },
			{ "VK_RMENU", 0xA5, "Right MENU" },
			{ "VK_BROWSER_BACK", 0xA6, "Browser Back" },
			{ "VK_BROWSER_FORWARD", 0xA7, "Browser Forward" },
			{ "VK_BROWSER_REFRESH", 0xA8, "Browser Refresh" },
			{ "VK_BROWSER_STOP", 0xA9, "Browser Stop" },
			{ "VK_BROWSER_SEARCH", 0xAA, "Browser Search" },
			{ "VK_BROWSER_FAVORITES", 0xAB, "Browser Favorites" },
			{ "VK_BROWSER_HOME", 0xAC, "Browser Start and Home" },
			{ "VK_VOLUME_MUTE", 0xAD, "Volume Mute" },
			{ "VK_VOLUME_DOWN", 0xAE, "Volume Down" },
			{ "VK_VOLUME_UP", 0xAF, "Volume Up" },
			{ "VK_MEDIA_NEXT_TRACK", 0xB0, "Next Track" },
			{ "VK_MEDIA_PREV_TRACK", 0xB1, "Previous Track" },
			{ "VK_MEDIA_STOP", 0xB2, "Stop Media" },
			{ "VK_MEDIA_PLAY_PAUSE", 0xB3, "Play/Pause Media" },
			{ "VK_LAUNCH_MAIL", 0xB4, "Start Mail" },
			{ "VK_LAUNCH_MEDIA_SELECT", 0xB5, "Select Media" },
			{ "VK_LAUNCH_APP1", 0xB6, "Start Application 1" },
			{ "VK_LAUNCH_APP2", 0xB7, "Start Application 2" },
			//{ "-", 0xB8 - B9, "Reserved" },
			{ "VK_PLAY", 0xFA, "Play" },
			{ "VK_ZOOM", 0xFB, "Zoom" },
		};

		BOOST_FOREACH(auto entry, hhb_vkey_list)
		{
			descritionToVK[entry.description] = entry.vkey;
		}
	}

	int KeyMapper::getVK(std::string keyName)
	{
		auto iter = descritionToVK.find(keyName);
		if (iter != descritionToVK.end())
		{
			return iter->second;
		}
		return 0;
	}
}