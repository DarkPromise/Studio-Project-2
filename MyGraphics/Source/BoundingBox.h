#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Vector3.h"

typedef struct
{
	Vector3 Max;
	Vector3 Min;

}BoundingBox;

BoundingBox createBox(Vector3 Max, Vector3 Min);

#endif