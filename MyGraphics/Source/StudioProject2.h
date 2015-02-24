#ifndef STUDIO_PROJECT_2_H
#define STUDIO_PROJECT_2_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera2.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"

#define insideBounds Vector3(2202,274,1826)
#define outsideBounds Vector3(2252,274,1876)
#define shelfBounds1Max Vector3(-2005,70,-340)
#define shelfBounds1Min Vector3(-2202,-274,-1826)
#define shelfBounds2Max Vector3(-975,70,-573)
#define shelfBounds2Min Vector3(-1445,-274,-1066)
#define shelfBounds3Max Vector3(-544,70,-573)
#define shelfBounds3Min Vector3(-75,-274,-1066)

class StudioProject2 : public Scene
{
public:
	StudioProject2();
	~StudioProject2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	float rotateAngle;

	float LSPEED;
	float rotateSpeed;
	float moveSpeed;

	float SGTranslate;
	float SGLegTranslate;
	bool SGState;

	float animateFlying;
	float animateDoor;

	float playerDistance;

	int openDoor;

	void setRotate(int i)
	{
		rotateDirection = i;
	}

	void setTranslate(int i2)
	{
		flyingDirection = i2;
	}

	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_LIGHTBALL,

		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_GLASSDOOR,

		GEO_CARPARK,
		GEO_CAR,
		GEO_GREYBRICK,
		GEO_GRASS,
		GEO_ROAD,
		GEO_ROAD2,
		GEO_WHITE,

		GEO_PLAYERHEAD,
		GEO_PLAYERBODY,
		GEO_PLAYERLEFTARM,
		GEO_PLAYERRIGHTARM,
		GEO_PLAYERLEFTLEG,
		GEO_PLAYERRIGHTLEG,

		GEO_CASHIERHEAD,
		GEO_CASHIERBODY,
		GEO_CASHIERLEFTARM,
		GEO_CASHIERRIGHTARM,
		GEO_CASHIERLEFTLEG,
		GEO_CASHIERRIGHTLEG,

		GEO_SGHEAD,
		GEO_SGBODY,
		GEO_SGLEFTARM,
		GEO_SGRIGHTARM,
		GEO_SGLEFTLEG,
		GEO_SGRIGHTLEG,

		GEO_CUSTOMERSERVICE,
		GEO_CASHIERTABLE,
		GEO_CASHIERTABLE2,
		GEO_SECURITYCOUNTER,
		GEO_SHELF,

		GEO_CACTUSJUICE,
		GEO_CADBURY,
		GEO_CANVEGE,
		GEO_CATFOOD,
		GEO_CEREAL,
		GEO_DETERGENT,
		GEO_DUREX,
		GEO_FERERRO,
		GEO_MACCHZ,
		GEO_MILK,
		GEO_MILOCAN,
		GEO_MOUNTAINDEW,
		GEO_PIZZAHUT,
		GEO_POTATOCHIPS,
		GEO_SARDINES,
		GEO_SPAM,

		GEO_OUTSIDELEFT,
		GEO_OUTSIDERIGHT,
		GEO_OUTSIDETOP,
		GEO_OUTSIDEBOTTOM,
		GEO_OUTSIDEFRONT,
		GEO_OUTSIDEBACK,

		GEO_BOUNDHELPER,
		GEO_TRANSPARENT,

		GEO_TEST,

		/*************************************************
		ANYTHING UNDER THIS SECTION IS FOR BOUNDS USE ONLY
		*************************************************/
		GEO_INSIDEMARKETBOUNDS,
		GEO_OUTSIDEMARKETBOUNDS,
		GEO_SHELFBOUNDS1,
		GEO_SHELFBOUNDS2,
		GEO_SHELFBOUNDS3,
		GEO_SHELFBOUNDS4,
		GEO_SHELFBOUNDS5,

		GEO_DOORBOUNDS,
		/************************************************/

		GEO_TEXT,

		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,

		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_TOTAL,
		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,

	};

	void setBoundingBox(BoundingBox box)
	{
		boundingBox = box;
	}

private:
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	int rotateDirection;
	int flyingDirection;
	double matrixSpeed;

	void renderPlayer();
	void renderCashier();
	void renderSecurityGuard();

	void renderSkybox();
	void renderSupermarket();
	void renderOutside();
	void renderBounds();

	void renderItems();

	bool CollisionCheck();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);

	Mtx44 translate, rotate, scale;
	Mtx44 model;
	Mtx44 view;
	Mtx44 projection;
	Mtx44 MVP;
	
	MS modelStack, viewStack, projectionStack;

	Camera2 camera;

	Vector3 playerDir;
	Vector3 playerPos;
	Vector3 playerRotation;

	/****************************************
	BOUNDS CODE GO HERE
	****************************************/
	BoundingBox boundingBox;
	/***************************************/
	Light light[1];

	void RenderMesh(Mesh * mesh, bool enableLight, bool transparent);
};

#endif