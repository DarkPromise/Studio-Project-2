#include "Camera2.h"
#include "Application.h"

/***********************************************************/
/*!
\brief
	Camera2's constructor
*/
/***********************************************************/
Camera2::Camera2()
{
}

/***********************************************************/
/*!
\brief
	Camera2's destruction
*/
/***********************************************************/
Camera2::~Camera2()
{
}

/***********************************************************/
/*
\brief
	Initialize the Camera variables values
\param pos - position of the camera

\param target - position of the target

\param up - the up direction of the camera's position
*/
/***********************************************************/
void Camera2::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	defaultView = view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();

	/*distanceFromChar = 50;
	angleView = 0;
	pitch = 20;
	yaw = 0;*/

	Maximum.Set(10000,10000,10000);
	Minimum.Set(-10000,-10000,-10000);
}

/***********************************************************/
/*!
\brief
	Update the camera position based on key pressed
\param dt - the delta time since the last time the function was called
*/
/***********************************************************/
void Camera2::Update(double dt,std::vector<CLocation*>List,float &_rotation,Mesh* cookie)
{
	static const float CAMERA_SPEED = 300.f;
	if(Application::IsKeyPressed('W'))
	{
		Vector3 temptarget = target;
		Vector3 tempposition = position;
		float yaw = (float)(CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		position += (view * yaw);
		target += (view * yaw);
		CLocation* ptr;
		for(int i = 0;i<List.size();i++)
		{
			ptr = List[i];
			if(target.x > Maximum.x || target.x < Minimum.x || target.y > Maximum.y || target.y < Minimum.y || target.z > Maximum.z || target.z < Minimum.z||(position.x<ptr->Maxbound.x && position.x>ptr->Minbound.x && position.y<ptr->Maxbound.y && position.y>ptr->Minbound.y && position.z<ptr->Maxbound.z && position.z>ptr->Minbound.z))
			{
				target = temptarget;
				position = tempposition;
			}
		}

		//target.y = temptarget.y;
		//position.y = tempposition.y;
	}

	if(Application::IsKeyPressed('S'))
	{
		Vector3 temptarget = target;
		Vector3 tempposition = position;
		float yaw = (float)(-CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		position += (view * yaw);
		target += (view * yaw);
		CLocation* ptr;
		for(int i = 0;i<List.size();i++)
		{
			ptr = List[i];
			if(target.x > Maximum.x || target.x < Minimum.x || target.y > Maximum.y || target.y < Minimum.y || target.z > Maximum.z || target.z < Minimum.z||(position.x<ptr->Maxbound.x && position.x>ptr->Minbound.x && position.y<ptr->Maxbound.y && position.y>ptr->Minbound.y && position.z<ptr->Maxbound.z && position.z>ptr->Minbound.z))
			{
				target = temptarget;
				position = tempposition;
			}
		}

		//target.y = temptarget.y;
		//position.y = tempposition.y;
	}

	if(Application::IsKeyPressed('A'))
	{
		Vector3 temptarget = target;
		Vector3 tempposition = position;
		float yaw = (float)(-CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		position += (view.Cross(up) * yaw);
		target += (view.Cross(up) * yaw);
		CLocation* ptr;
		for(int i = 0;i<List.size();i++)
		{
			ptr = List[i];
			if(target.x > Maximum.x || target.x < Minimum.x || target.y > Maximum.y || target.y < Minimum.y || target.z > Maximum.z || target.z < Minimum.z||(position.x<ptr->Maxbound.x && position.x>ptr->Minbound.x && position.y<ptr->Maxbound.y && position.y>ptr->Minbound.y && position.z<ptr->Maxbound.z && position.z>ptr->Minbound.z))
			{
				target = temptarget;
				position = tempposition;
			}
		}
		
		//target.y = temptarget.y;
		//position.y = tempposition.y;
	}

	if(Application::IsKeyPressed('D'))
	{
		Vector3 temptarget = target;
		Vector3 tempposition = position;
		float yaw = (float)(CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		position += (view.Cross(up) * yaw);
		target += (view.Cross(up) * yaw);
		CLocation* ptr;
		for(int i = 0;i<List.size();i++)
		{
			ptr = List[i];
			if(target.x > Maximum.x || target.x < Minimum.x || target.y > Maximum.y || target.y < Minimum.y || target.z > Maximum.z || target.z < Minimum.z||(position.x<ptr->Maxbound.x && position.x>ptr->Minbound.x && position.y<ptr->Maxbound.y && position.y>ptr->Minbound.y && position.z<ptr->Maxbound.z && position.z>ptr->Minbound.z))
			{
				target = temptarget;
				position = tempposition;
			}
		}

		//target.y = temptarget.y;
		//position.y = tempposition.y;
	}

	if(Application::IsKeyPressed('Q'))
	{
		Vector3 tempposition = position;
		Vector3 temptarget = target;
		float yaw = (float)(-CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position += (view.Cross(right) * yaw);
		target += (view.Cross(right) * yaw);
		CLocation* ptr;
		for(int i = 0;i<List.size();i++)
		{
			ptr = List[i];
			if(target.x > Maximum.x || target.x < Minimum.x || target.y > Maximum.y || target.y < Minimum.y || target.z > Maximum.z || target.z < Minimum.z||(position.x<ptr->Maxbound.x && position.x>ptr->Minbound.x && position.y<ptr->Maxbound.y && position.y>ptr->Minbound.y && position.z<ptr->Maxbound.z && position.z>ptr->Minbound.z))
			{
				target = temptarget;
				position = tempposition;
			}
		}
	}

	if(Application::IsKeyPressed('E'))
	{
		Vector3 temptarget = target;
		Vector3 tempposition = position;
		float yaw = (float)(CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position += (view.Cross(right) * yaw);
		target += (view.Cross(right) * yaw);
		CLocation* ptr;
		for(int i = 0;i<List.size();i++)
		{
			ptr = List[i];
			if(target.x > Maximum.x || target.x < Minimum.x || target.y > Maximum.y || target.y < Minimum.y || target.z > Maximum.z || target.z < Minimum.z||(position.x<ptr->Maxbound.x && position.x>ptr->Minbound.x && position.y<ptr->Maxbound.y && position.y>ptr->Minbound.y && position.z<ptr->Maxbound.z && position.z>ptr->Minbound.z))
			{
				target = temptarget;
				position = tempposition;
			}
		}
	}

	// tilt up
	if(Application::IsKeyPressed(VK_UP))
	{
		Mtx44 rotation;
		float pitch = (float)(CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		//target = (position + view);
		target -= position;
		target = rotation*target;
		target+=position;
	}

	// tilt down
	if(Application::IsKeyPressed(VK_DOWN))
	{
		Mtx44 rotation;
		float pitch = (float)(-CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		//target = (position + view);
		target -= position;
		target = rotation*target;
		target+=position;
	}

	// tilt left
	if(Application::IsKeyPressed(VK_LEFT))
	{
		Mtx44 rotation;
		float pitch = (float)(CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		rotation.SetToRotation(pitch, 0, 1, 0);
		view = rotation * view;
		//target = (position + view);
		target -= position;
		target = rotation* target;
		target+=position;
		up = rotation * up;
	}

	// tilt right
	if(Application::IsKeyPressed(VK_RIGHT))
	{
		Mtx44 rotation;
		float pitch = (float)(-CAMERA_SPEED * dt);
		view = (target - position).Normalized();
		rotation.SetToRotation(pitch, 0, 1, 0);
		view = rotation * view;
		//target = (position + view);
		target -= position;
		target = rotation* target;
		target+=position;
		up = rotation * up;
	}
	if(Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

/***********************************************************/
/*!
\brief
	Reset's the camera's variables to default
*/
/***********************************************************/
void Camera2::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
	view = defaultView;
}