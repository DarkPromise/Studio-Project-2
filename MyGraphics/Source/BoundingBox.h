#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

//Josh Code

#include "Vector3.h"

typedef struct BoundingBox
{
	BoundingBox() //Inside = BoundingBox for indoors
		: isInside(true),
	isObj(false)
	{
		//Default Constructor With Nothing :D
	}

	bool isInside;
	bool isObj;

	Vector3 Max;
	Vector3 Min;

}BoundingBox;

BoundingBox createBox(Vector3 Max, Vector3 Min);

#endif