#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

//Josh Code

#include "Vector3.h"

typedef struct BoundingBox
{
	BoundingBox() //Inside = BoundingBox for indoors
		: isInside(true),
	isObj(false),
	canPhase(false),
	isPlayer(false)
	{
		//Default Constructor With Nothing :D
	}

	bool isInside;
	bool isObj;
	bool canPhase;
	bool isPlayer;

	Vector3 Max;
	Vector3 Min;

}BoundingBox;

BoundingBox createBox(Vector3 Max, Vector3 Min);

#endif