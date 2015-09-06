#include "ActionDetails.h"

namespace SpaceNavigatorEmulator
{
	ActionDetails::ActionDetails(unsigned int mappingOffsetA, bool axisInvertedA) :
		mappingOffset(mappingOffsetA),
		axisInverted(axisInvertedA)
	{
	}


	ActionDetails::~ActionDetails()
	{
	}
}