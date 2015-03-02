#ifndef CAMERA_2_H
#define CAMERA_2_H

#include "Camera.h"
#include "Mtx44.h"
#include <vector>

class Camera2 : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
	Vector3 defaultView;
	Vector3 lookAt;

	float distanceFromChar;
	float angleView;
	float pitch;
	float yaw;
	float roll;

	int facingDirection;

	Vector3 Maximum;
	Vector3 Minimum;

	Camera2();
	~Camera2();

	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt, bool move);
	virtual void Reset();
};

#endif