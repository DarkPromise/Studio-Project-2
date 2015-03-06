/******************************************************************************/
/*!
\file	StudioProject2.h
\author Group 7
\brief
Header file for StudioProject2
*/
/******************************************************************************/
#ifndef STUDIO_PROJECT_2_H
#define STUDIO_PROJECT_2_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera2.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <vector>
#include <string>
#include "Item.h"
#include "Shelfslot.h"
#include "Inhand.h"
#include "BoundingBox.h"
#include "Octree.h"
#include "Child.h"
#include "Passerby.h"
#include "Promoter.h"
#include "Customer.h"
#include "Vehicle.h"
#include "Cashier.h"
#include "SecurityGuard.h"
#include "AI.h"
#include "Shopping.h"

using std::vector;

class StudioProject2 : public Scene
{
public:
	StudioProject2();
	~StudioProject2();

	virtual void Init();
	virtual void Update(double dt, double xpos, double ypos);
	virtual void Render();
	virtual void Exit();

	float rotateAngle;
	float modelrotatey;

	float LSPEED;
	float rotateSpeed;
	float moveSpeed;
	bool canMove;
	bool doRenderBounds;
	std::string currView;

	float menuX;
    bool stoptime;
	bool showCursor;

	int checkingOut;
	int takeItem;
	float itemDelay;

	float animateFlying;
	float animateDoor;

	float playerDistance;

	void AItakeItem(int Shelf);

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
		GEO_LEFTGATE,
		GEO_RIGHTGATE,

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

		GEO_CUSTOMERHEAD,
		GEO_CUSTOMERBODY,
		GEO_CUSTOMERLEFTARM,
		GEO_CUSTOMERRIGHTARM,
		GEO_CUSTOMERLEFTLEG,
		GEO_CUSTOMERRIGHTLEG,

		GEO_PROMOTERHEAD,
		GEO_PROMOTERBODY,
		GEO_PROMOTERLEFTARM,
		GEO_PROMOTERRIGHTARM,
		GEO_PROMOTERLEFTLEG,
		GEO_PROMOTERRIGHTLEG,
		GEO_PROMOTERTABLE,

		GEO_OWNERHEAD,
		GEO_OWNERBODY,
		GEO_OWNERLEFTARM,
		GEO_OWNERRIGHTARM,
		GEO_OWNERLEFTLEG,
		GEO_OWNERRIGHTLEG,
		GEO_OWNERTABLE,

		GEO_PASSERBYHEAD,
		GEO_PASSERBYBODY,
		GEO_PASSERBYLEFTARM,
		GEO_PASSERBYRIGHTARM,
		GEO_PASSERBYLEFTLEG,
		GEO_PASSERBYRIGHTLEG,

		GEO_CUSTOMERSERVICE,
		GEO_CASHIERTABLE,
		GEO_CASHIERTABLE2,
		GEO_SECURITYCOUNTER,
		GEO_SHELF,
		GEO_CHILLER,
		GEO_FREEZER,

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
		GEO_MARKETWALL1,
		GEO_MARKETWALL2,
		GEO_MARKETWALL3,
		GEO_MARKETWALL4,
		GEO_SHELFBOUNDS1,
		GEO_SHELFBOUNDS2,
		GEO_SHELFBOUNDS3,
		GEO_SHELFBOUNDS4,
		GEO_SHELFBOUNDS5,
		GEO_DOORBOUNDS,
		GEO_PLAYERBOUNDS,
		GEO_PASSERBYBOUNDS,
		GEO_CUSTOMERBOUNDS,
		GEO_PROMOTERBOUNDS,
		GEO_SECURITYGUARDBOUNDS,
		GEO_CASHIERBOUNDS,
		GEO_FREEZERBOUNDS,
		GEO_CHILLERBOUNDS,
		GEO_GATEBOUNDS1,
		GEO_GATEBOUNDS2,

		GEO_CASHIERRIGHTBOUNDS,
		GEO_MIDDLECASHIERBOUNDS,
		GEO_LEFTCASHIERBOUNDS,
		GEO_SECURITYBOUNDS,
		GEO_CUSTOMERSERVICEBOUNDS,
		GEO_LSHAPELEFTBOUNDS,
		GEO_LSHAPEBACKBOUNDS,
		/************************************************/

		/*************************************************
		USER INTERFACE
		*************************************************/
		GEO_SIDEBAR,
		GEO_CURRENTBOX,
		GEO_CROSSHAIR,
		GEO_CACTUSJUICEUI,
		GEO_CADBURYUI,
		GEO_CANVEGEUI,
		GEO_CATFOODUI,
		GEO_CEREALUI,
		GEO_DETERGENTUI,
		GEO_FERERROUI,
		GEO_MACCHZUI,
		GEO_MILOCANUI,
		GEO_POTATOCHIPSUI,
		GEO_SARDINESUI,
		GEO_TEXTBOX,
		GEO_SHOPPINGLIST,
		GEO_CORRECT,
		GEO_WRONG,
		/************************************************/

		GEO_MENU,
		GEO_TEXT,
		GEO_LSHAPEWALL,

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

	Octree *tree;

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
	void renderCustomer();
	void renderPromoter();
	void renderPasserby();
	void renderVehicle();
	void renderOwner();

	void renderSkybox();
	void renderSupermarket();
	void renderOutside();
	void renderBounds();
	void renderItems();
	void renderUI();
	void MainMenu();
	void CollisionCheck(double);

	int Sounds();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderQuadOnScreen(Mesh* mesh, float scalex, float scaley, float x, float y);

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
	Vector3 testPos;

	vector<Item*> itemVector;
	vector<Shelfslot*> shelfVector;

	Inhand* inhand;
	int currentlyHolding;
	Shopping shopping;
	vector<string> shoppingList;
	/****************************************
	BOUNDS CODE GO HERE
	****************************************/
	BoundingBox boundingBox;
	Vector3 tempStorage;
	Vector3 tempTarget;
	Vector3 tempUp;

	bool canPhase;
	bool sideMove;
	bool frontMove;
	/***************************************/
	Light light[1];

	void RenderMesh(Mesh * mesh, bool enableLight, bool transparent);
};

enum BOUNDTYPE
	{
		MARKETWALL1 = 0,
		MARKETWALL2,
		MARKETWALL3,
		MARKETWALL4,
		SHELF1,
		SHELF2,
		SHELF3,
		SHELF4,
		SHELF5,
		DOOR,
		PLAYER,
		PASSERBY,
		CUSTOMER,
		PROMOTER,
		SECURITYGUARD,
		CASHIER,
		FREEZER,
		CHILLER,
		GATE1,
		GATE2,
		RIGHTCASHIER,
		MIDDLECASHIER,
		LEFTCASHIER,
		SECURITY,
		CUSTOMERSERVICE,
		LSHAPELEFT,
		LSHAPEBACK,
	};
#endif