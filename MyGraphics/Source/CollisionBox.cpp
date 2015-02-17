#include "CollisionBox.h"
#include "MyMath.h"

int CollisionBox::boundCollide(const Plane& plane,const Vector3& position, const Vector3& direction, float &Distance,Vector3& normal)
{
	float dot = direction.Dot(plane.normal);
	float collisionDistance;

	if((dot < Math::EPSILON) && (dot >-Math::EPSILON))
	{
		return 0;
	}

	collisionDistance = (plane.normal.Dot(plane.position-position))/dot;

	if(collisionDistance<-Math::EPSILON)
	{
		return 0;
	}

	normal = plane.normal;
	Distance = collisionDistance;
	return 1;
}