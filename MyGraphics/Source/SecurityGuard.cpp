#include "SecurityGuard.h"


SecurityGuard::SecurityGuard(void)
{
}


SecurityGuard::~SecurityGuard(void)
{
}

void SecurityGuard::updateAI(void)
{
	for ( int i = 0; i < this->AICurrent; i++ )
	{
		if ( this->State[i] == Sitting )
		{
		}
		else if ( this->State[i] == Standing )
		{
		}
	}
}

void SecurityGuard::spawnAI(void)
{
	if ( this->AICurrent == 0 )
	{
		this->Coordinates.push_back(SittingGuard);
		this->rotateY.push_back(GuardDirection);
		this->State.push_back(Sitting);

		this->AICurrent = 1;
	}
}

void SecurityGuard::interactWith(void)
{
}