#include "Item.h"

Item::Item(Vector3 boundMax, Vector3 boundMin, Vector3 translate, Vector3 scale, float rotateY, int mesh, int uimesh)
{
	position.SetZero();
	this->boundMax = storeBoundMax = boundMax;
	this->boundMin = storeBoundMin= boundMin;

	position += translate;
	this->boundMax += translate;
	this->boundMin += translate;

	this->scale = scale;
	this->rotateY = rotateY;
	this->mesh = mesh;
	this->uimesh = uimesh;
}


Item::~Item(void)
{
}

void Item::takeItem(Vector3 newPos)
{
	position = newPos;
	boundMax = newPos + storeBoundMax;
	boundMin = newPos + storeBoundMin;
}

void Item::placeItem(Vector3 newPos)
{
	position = newPos;
	boundMax = newPos + storeBoundMax;
	boundMin = newPos + storeBoundMin;
}

void Item::updateRotate(float rotate)
{
	rotateY = rotate;
}