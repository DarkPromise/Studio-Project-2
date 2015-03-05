/******************************************************************************/
/*!
\file	Camera2.h
\author Group 7
\brief
Header file for Camera2
*/
/******************************************************************************/
#ifndef CAMERA_2_H
#define CAMERA_2_H

#include "Camera.h"
#include "Mtx44.h"
#include <vector>

class Camera2 : public Camera
{
public:
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
	Vector3 defaultView;

	float mouseSpeed;
	float lookSpeed;
	float yaw;
	float pitch;

	float aspect;

	bool firstInit;
	Vector3 direction;
	Vector3 right;
	Vector3 rotate;
	double delay;

	int facingDirection;

	Vector3 Maximum;
	Vector3 Minimum;

	Camera2();
	~Camera2();

	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt, bool move, double xpos, double ypos);
	virtual void Change(double &dx, double &dy);
	virtual void Reset();
};

#endif