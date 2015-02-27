#include "Inhand.h"
Inhand::Inhand(void)
{
}


Inhand::Inhand(int maxAmount)
{
	this->maxAmount = maxAmount;
	reachMax = false;
}

Inhand::~Inhand(void)
{
}

void Inhand::recive(int a)
{
	holding.push_back(a);

	if(holding.size() >= maxAmount)
	{
		reachMax = true;
	}
}

int Inhand::remove()
{
	int a = holding.back();
	holding.pop_back();

	reachMax = false;

	return a;
}