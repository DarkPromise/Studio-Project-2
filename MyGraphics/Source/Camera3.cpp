#include "Camera3.h"
#include "Application.h"

/******************************************************************************/
/*!
\brief
Camera3's constructor.
*/
/******************************************************************************/
Camera3::Camera3()
{
}

/******************************************************************************/
/*!
\brief
Camera3's destructor.
*/
/******************************************************************************/
Camera3::~Camera3()
{
}

/******************************************************************************/
/*!
\brief
Initialize the various properties (position, target, up, view) and the rotation of Camera3.
\param pos
the location of the camera.
\param target
the location at which the camera is looking at.
\param up
the vertically up vector that is perpendicular to the camera.
*/
/******************************************************************************/
void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	defaultView = view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();

	maxWorld.Set(690, 690, 690);
	minWorld.Set(-690, -90, -690);

	viewAngle = 0.0f;

	rotation.SetToIdentity();
}

/******************************************************************************/
/*!
\brief
Update the Camera's position, target, up and view location based on the time passed since the last update.
\param dt - 
the time passed since the last update.
*/
/******************************************************************************/
void Camera3::Update(double dt)
{
	static const float CAMERA_SPEED = 100.f;

	if(Application::IsKeyPressed('W'))
	{
		Vector3 Ytarget;
		Vector3 tempPos, tempTar;
		tempPos = position;
		tempTar = target;
		Ytarget = target;
		Ytarget.y = position.y;
		float yaw = (float)(CAMERA_SPEED * dt);
		view = (Ytarget - position).Normalize();
		position += (view * yaw);
		target += (view * yaw);
		if (position.x > maxWorld.x || position.x < minWorld.x || position.y > maxWorld.y || position.y < minWorld.y || position.z > maxWorld.z || position.z < minWorld.z)
		{
			position = tempPos;
			target = tempTar;
		}	

		position.y = tempPos.y;
		target.y = tempTar.y;
	}

	if(Application::IsKeyPressed('S'))
	{
		Vector3 Ytarget;
		Vector3 tempPos, tempTar;
		tempPos = position;
		tempTar = target;
		Ytarget = target;
		Ytarget.y = position.y;
		float yaw = (float)(-CAMERA_SPEED * dt);
		view = (Ytarget - position).Normalize();
		position += (view * yaw);
		target += (view * yaw);
		if (position.x > maxWorld.x || position.x < minWorld.x || position.y > maxWorld.y || position.y < minWorld.y || position.z > maxWorld.z || position.z < minWorld.z)
		{
			position = tempPos;
			target = tempTar;
		}

		//position.y = tempPos.y;
		//target.y = tempTar.y;
	}

	if(Application::IsKeyPressed('A'))
	{
		Vector3 Ytarget;
		Vector3 tempPos, tempTar;
		tempPos = position;
		tempTar = target;
		Ytarget = target;
		Ytarget.y = position.y;
		float yaw = (float)(-CAMERA_SPEED * dt);
		view = (Ytarget - position).Normalize();
		position += (view.Cross(up) * yaw);
		target += (view.Cross(up) * yaw);
		if (position.x > maxWorld.x || position.x < minWorld.x || position.y > maxWorld.y || position.y < minWorld.y || position.z > maxWorld.z || position.z < minWorld.z)
		{
			position = tempPos;
			target = tempTar;
		}
	}

	if(Application::IsKeyPressed('D'))
	{
		Vector3 Ytarget;
		Vector3 tempPos, tempTar;
		tempPos = position;
		tempTar = target;
		Ytarget = target;
		Ytarget.y = position.y;
		float yaw = (float)(CAMERA_SPEED * dt);
		view = (Ytarget - position).Normalize();
		position += (view.Cross(up) * yaw);
		target += (view.Cross(up) * yaw);
		if (position.x > maxWorld.x || position.x < minWorld.x || position.y > maxWorld.y || position.y < minWorld.y || position.z > maxWorld.z || position.z < minWorld.z)
		{
			position = tempPos;
			target = tempTar;
		}
	}

	if(Application::IsKeyPressed('Q'))
	{
		Vector3 tempPos, tempTar;
		tempPos = position;
		tempTar = target;
		float yaw = (float)(-CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position += (view.Cross(right) * yaw);
		target += (view.Cross(right) * yaw);
		if (position.x > maxWorld.x || position.x < minWorld.x || position.y > maxWorld.y || position.y < minWorld.y || position.z > maxWorld.z || position.z < minWorld.z)
		{
			position = tempPos;
			target = tempTar;
		}
	}

	if(Application::IsKeyPressed('E'))
	{
		Vector3 tempPos, tempTar;
		tempPos = position;
		tempTar = target;
		float yaw = (float)(CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position += (view.Cross(right) * yaw);
		target += (view.Cross(right) * yaw);
		if (position.x > maxWorld.x || position.x < minWorld.x || position.y > maxWorld.y || position.y < minWorld.y || position.z > maxWorld.z || position.z < minWorld.z)
		{
			position = tempPos;
			target = tempTar;
		}
	}

	// tilt up
	if(Application::IsKeyPressed(VK_UP))
	{
		float pitch = (float)(CAMERA_SPEED * dt);
		view = (target - position).Normalize();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalize();
		//viewAngle += 10.0f * dt;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = (position + view);
	}

	// tilt down
	if(Application::IsKeyPressed(VK_DOWN))
	{
		float pitch = (float)(-CAMERA_SPEED * dt);
		view = (target - position).Normalize();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalize();
		//viewAngle -= 10.0f * dt;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = (position + view);
	}

	// tilt left
	if(Application::IsKeyPressed(VK_LEFT))
	{
		float pitch = (float)(CAMERA_SPEED * dt);
		view = (target - position).Normalize();
		rotation.SetToRotation(pitch, 0, 1, 0);
		view = rotation * view;
		target = (position + view);
		up = rotation * up;
	}

	// tilt right
	if(Application::IsKeyPressed(VK_RIGHT))
	{
		float pitch = (float)(-CAMERA_SPEED * dt);
		view = (target - position).Normalize();
		rotation.SetToRotation(pitch, 0, 1, 0);
		view = rotation * view;
		target = (position + view);
		up = rotation * up;
	}

	if(Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

/******************************************************************************/
/*!
\brief
Reset the position, target, up and view coordinates and rotation to the default values.
*/
/******************************************************************************/
void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
	view = defaultView;
	rotation.SetToIdentity();
	viewAngle = 0.0f;
}