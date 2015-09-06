#pragma once

namespace SpaceNavigatorEmulator
{
	class ActionDetails
	{
	public:
		ActionDetails(unsigned int mappingOffset, bool axisInverted);
		~ActionDetails();

		unsigned int mappingOffset;
		bool axisInverted;
	};
}

