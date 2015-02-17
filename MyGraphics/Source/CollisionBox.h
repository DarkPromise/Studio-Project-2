#ifndef COLLISION_BOX_H
#define COLLISION_BOX_H

#include "Vector3.h"

struct Plane
{
	Vector3 position;
	Vector3 normal;
};

class CollisionBox
{
public:
	int boundCollide(const Plane& plane,const Vector3& position, const Vector3& direction, float &Distance,Vector3& normal);

private:
};

#endif