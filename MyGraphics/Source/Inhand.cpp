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

int Inhand::remove(int currentlyHolding)
{
	int a = holding[currentlyHolding];
	holding.erase(holding.begin()+currentlyHolding);

	reachMax = false;

	return a;
}

void Inhand::dropAll()
{
	holding.clear();
	reachMax = false;
}