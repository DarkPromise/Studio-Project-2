#pragma once
#include "Vector3.h"
#include "Mesh.h"

class CLocation
{
public:
	CLocation(void);
	~CLocation(void);

	Vector3 Maxbound;
	Vector3 Minbound;
	Vector3 Translate;
	Vector3 Scale;
	Vector3 Size;
	Vector3 Rotate;
	Vector3 InteractionMaxbound;
	Vector3 InteractionMinbound;

	CLocation(Vector3 _Translate, Vector3 _Scale, Vector3 _Size,Mesh* _obj);
	void CalcLocation();

	Mesh* obj;
};

