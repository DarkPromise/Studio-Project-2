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

using std::vector;

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
	float modelrotatey;

	float LSPEED;
	float rotateSpeed;
	float moveSpeed;
	bool canMove;

	std::string currView;

	int checkingOut;

	float promoterRotateY;
	float itemRotateY;
	float cashier1RotateY;
	float cashier2RotateY;
	float cashier3RotateY;
	float cashier4RotateY;
	float rotateDelay;

	float SGTranslate;
	float SGLegTranslate;
	bool SGState;

	vector<float> CustomerX;
	vector<float> CustomerZ;
	vector<float> CustomerRotation;
	vector<int> CustomerState;
	vector<float> CustomerItemsHeld;
	int Customers;
	
	vector<float> PasserbyX;
	vector<float> PasserbyZ;
	vector<float> PasserbyRotation;
	int Passerby;

	vector<float> VehicleX;
	vector<float> VehicleZ;
	vector<float> OwnerX;
	vector<float> OwnerZ;
	vector<float> OwnerRotation;
	vector<float> OwnerItemsHeld;
	vector<float> VehicleRotation;
	vector<int> ParkingLocation;
	vector<int> VehicleState;
	vector<int> RenderOwner;
	int Vehicles;

	enum CustomerPath
	{
		CUSTOMER_PATHTOCARPARK1,
		CUSTOMER_PATHTOENTRANCE1,
		CUSTOMER_ENTRANCETOBASKET,
		CUSTOMER_BASKETTOENTRANCE,
		CUSTOMER_ENTRANCETOGATES,
		CUSTOMER_GATESTOTROLLEY,
		CUSTOMER_TROLLEYTOGATES,
		CUSTOMER_GATESTODRINKS,
		CUSTOMER_DRINKSTOGATES,
		CUSTOMER_GATESTOSHELF1,
		CUSTOMER_SHELF1TOSHELF2,
		CUSTOMER_SHELF1TOBACKSHELF,
		CUSTOMER_SHELF2TOBACKSHELF,
		CUSTOMER_BACKSHELVESTOLEFTSHELF,
		CUSTOMER_BACKSHELVESTOSHELF4,
		CUSTOMER_SHELF4TOCHECKOUT1,
		CUSTOMER_LEFTSHELFTOFRONTLEFTSHELF,
		CUSTOMER_LEFTSHELFTOCUSTOMERSERVICE,
		CUSTOMER_CUSTOMERSERVICETOCHECKOUT1,
		CUSTOMER_CUSTOMERSERVICETOCHECKOUT2,
		CUSTOMER_CUSTOMERSERVICETOCHECKOUT3,
		CUSTOMER_CUSTOMERSERVICETOCHECKOUT4,
		CUSTOMER_CUSTOMERSERVICETOGATES,
		CUSTOMER_SHELF4TOGATES,
		CUSTOMER_CHECKOUT,
		CUSTOMER_CHECKOUTTOEXIT,
		VEHICLE_ROADTOENTRANCE = 30,
		VEHICLE_ENTRANCETOEMPTYSLOT,
	};


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

	void VehicleAI();
	void CustomerAI();
	void PasserbyAI();
	void PromoterAI();
	void GenerateAIs();

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

	void CollisionCheck(double);

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
	
	vector<Item*> itemVector;
	vector<Shelfslot*> shelfVector;

	Inhand* inhand; 
	/****************************************
	BOUNDS CODE GO HERE
	****************************************/
	BoundingBox boundingBox;
	Vector3 tempStorage;
	Vector3 tempTarget;
	Vector3 tempUp;
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
		PLAYER
	};
#endif