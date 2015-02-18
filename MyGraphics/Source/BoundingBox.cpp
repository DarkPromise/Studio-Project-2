#include "BoundingBox.h"

BoundingBox createBox(Vector3 Max, Vector3 Min)
{
	BoundingBox box;
	box.Max = Max;
	box.Min = Min;

	return box;
}