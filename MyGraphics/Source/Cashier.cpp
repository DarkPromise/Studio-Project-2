#include "Cashier.h"


Cashier::Cashier(void)
{
}


Cashier::~Cashier(void)
{
}

void Cashier::updateAI(void)
{
	for ( int i = 0; i < this->AICurrent; i++ )
	{
		if ( this->turnDelay[i] > 50 )
		{
			int delayTime = rand() % 2;
			
			if ( delayTime == 0 )
			{
				if ( i == 0 || i == 2 )
				{
					if ( this->rotateY[i] == cashierRight )
						this->rotateY[i] = cashierForward;
					else
						this->rotateY[i] = cashierRight;
				}
				else
				{
					if ( this->rotateY[i] == cashierLeft )
						this->rotateY[i] = cashierForward;
					else
						this->rotateY[i] = cashierLeft;
				}

				this->turnDelay[i] -= 50;
			}
			else
			{
				if ( i == 0 || i == 2 )
				{
					if ( this->rotateY[i] == cashierRight )
						this->rotateY[i] = cashierForward;
					else
						this->rotateY[i] = cashierRight;
				}
				else
				{
					if ( this->rotateY[i] == cashierLeft )
						this->rotateY[i] = cashierForward;
					else
						this->rotateY[i] = cashierLeft;
				}

				this->turnDelay[i] -= 25;
			}
		}
		else
		{
			this->turnDelay[i] += 0.2f;
		}
	}
}

void Cashier::spawnAI(void)
{
	if ( this->AICurrent == 0 )
	{
		this->Coordinates.push_back(Cashier1Spawnpoint);
		this->rotateY.push_back(cashierRight);
		this->turnDelay.push_back(0);

		this->Coordinates.push_back(Cashier2Spawnpoint);
		this->rotateY.push_back(cashierLeft);
		this->turnDelay.push_back(0);

		this->Coordinates.push_back(Cashier3Spawnpoint);
		this->rotateY.push_back(cashierRight);
		this->turnDelay.push_back(0);

		this->Coordinates.push_back(Cashier4Spawnpoint);
		this->rotateY.push_back(cashierLeft);
		this->turnDelay.push_back(0);

		this->AICurrent = 4;
	}
}

void Cashier::interactWith(void)
{
}