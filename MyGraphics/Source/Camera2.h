#ifndef CAMERA_2_H
#define CAMERA_2_H

#include "Camera.h"
#include "Location.h"
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

	Vector3 Maximum;
	Vector3 Minimum;

	Camera2();
	~Camera2();

	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt,std::vector<CLocation*>List,float &_rotation,Mesh* cookie);
	virtual void Reset();
};

#endif