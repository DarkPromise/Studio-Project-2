#include "Shelfslot.h"


Shelfslot::Shelfslot(Vector3 position, Vector3 boundMax, Vector3 boundMin, int itemid, bool isempty)
{
	this->position = position;
	this->boundMax = position + boundMax;
	this->boundMin = position +boundMin;
	this->itemid = itemid;
	this->isempty = isempty;
}


Shelfslot::~Shelfslot(void)
{
}
