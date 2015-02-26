#include "Location.h"


CLocation::CLocation(void)
{

}

CLocation::CLocation(Vector3 _Translate, Vector3 _Scale, Vector3 _Size,Mesh* _obj)
{
	Translate = _Translate;
	Scale = _Scale;
	Size = _Size;
	obj = _obj;
}

void CLocation::CalcLocation()
{
	//Vector3 Origin(Translate);
	Minbound.x = Translate.x - ((Size.x * Scale.x)/2);
	Maxbound.x = Translate.x + ((Size.x * Scale.x)/2);
	Minbound.y = Translate.y - ((Size.y * Scale.y)/2);
	Maxbound.y = Translate.y + ((Size.y * Scale.y)/2);
	Minbound.z = Translate.z - ((Size.z * Scale.z)/2);
	Maxbound.z = Translate.z + ((Size.z * Scale.z)/2);

	InteractionMaxbound = Maxbound;
	InteractionMinbound = Minbound;
}

CLocation::~CLocation(void)
{
}
