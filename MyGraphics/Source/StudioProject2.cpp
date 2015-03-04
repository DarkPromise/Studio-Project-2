#include "StudioProject2.h"
#include "Mtx44.h"
#include <iostream>
#include "Application.h"
#include <sstream>
#include <iomanip>
#include <vector>

#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Material.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "define.h"

using std::setprecision;
using std::cout;
using std::endl;
using std::vector;
using std::iterator;
using std::string;

bool removeItems = false;

//Test
double FPS;
std::string textPS;
double toggleDelay = 0.0f;

double deltaTime;

vector<BoundingBox*> box;
BoundingBox * boxPtr;
BOUNDTYPE bound;

Passerby		* myPasserby;
Promoter		* myPromoter;
Customer		* myCustomer;
Vehicle			* myVehicle;
Cashier			* myCashier;
SecurityGuard	* myGuard;

StudioProject2::StudioProject2()
{
}

StudioProject2::~StudioProject2()
{
}

void StudioProject2::Init()
{
	int modelrotatey = 0;
	//Depth buffer
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);

	//Load vertex and fragment shaders
	m_programID = LoadShaders( "Shader//Texture.vertexshader", "Shader//Text.fragmentshader" );

	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	glUseProgram(m_programID);

	light[0].position.Set(0, 530, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 100;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;

	// Make sure you pass uniform parameters after glUseProgram()
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);

	//Init default values

	LSPEED = 100.0f;
	rotateSpeed = 200.0f;
	moveSpeed = 500.0f;
	rotateAngle = 0.0f;
	canMove = true;
	canPhase = false;
	
	stoptime = true;
	menuX = 2.8f; 

	carparkSlot[0] = 0;
	carparkSlot[1] = 0; // 0 = empty, 1 = filled
	carparkSlot[2] = 0;
	carparkSlot[3] = 0;

	//Init AI
	myPasserby = new Passerby();
	myPasserby->AILimit = passerbyLimit;
	myPasserby->AICurrent = 0;
	myPasserby->spawnRate = passerbySpawnRate;

	myPromoter = new Promoter();
	myPromoter->AILimit = promoterLimit;
	myPromoter->AICurrent = 0;
	myPromoter->itemRotateY = 0;

	myCustomer = new Customer();
	myCustomer->AILimit = customerLimit;
	myCustomer->AICurrent = 0;
	myCustomer->spawnRate = customerSpawnRate;

	myVehicle = new Vehicle();
	myVehicle->AILimit = vehicleLimit;
	myVehicle->AICurrent = 0;
	myVehicle->spawnRate = vehicleSpawnRate;

	myCashier = new Cashier();
	myCashier->AILimit = cashierLimit;
	myCashier->AICurrent = 0;

	myGuard = new SecurityGuard();
	myGuard->AILimit = 1;
	myGuard->AICurrent = 0;

	playerPos.Set(0,0,0);
	testPos.Set(0,0,0);
	tempStorage.Set(0,0,0);
	tempTarget.Set(0,0,0);

	/********************
	BOUNDS INIT GO HERE
	********************/
	boxPtr = new BoundingBox();
	boxPtr->Max = NorthSouthWall;
	boxPtr->Min = -NorthSouthWall;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->Max = NorthSouthWall;
	boxPtr->Min = -NorthSouthWall;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->Max = EastWestWall;
	boxPtr->Min = -EastWestWall;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->Max = EastWestWall;
	boxPtr->Min = -EastWestWall;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->isObj = true;
	boxPtr->Max = leftShelfBounds;
	boxPtr->Min = -leftShelfBounds;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->isObj = true;
	boxPtr->Max = middleLeftShelfBounds;
	boxPtr->Min = -middleLeftShelfBounds;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->isObj = true;
	boxPtr->Max = middleRightShelfBounds;
	boxPtr->Min = -middleRightShelfBounds;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->isObj = true;
	boxPtr->Max = backShelfBounds;
	boxPtr->Min = -backShelfBounds;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->isObj = true;
	boxPtr->Max = loneShelfBounds;
	boxPtr->Min = -loneShelfBounds;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->isObj = true;
	boxPtr->canPhase = true;
	boxPtr->isInteractive = true;
	boxPtr->Max = doorBounds;
	boxPtr->Min = -doorBounds;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->isPlayer = true;
	boxPtr->Max = playerBounds;
	boxPtr->Min = -playerBounds;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->isObj = true;
	boxPtr->Max = freezerBounds;
	boxPtr->Min = -freezerBounds;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->isObj = true;
	boxPtr->Max = chillerBounds;
	boxPtr->Min = -chillerBounds;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->isObj = true;
	boxPtr->canPhase = true;
	boxPtr->isInteractive = true;
	boxPtr->Max = gateBoundsRight;
	boxPtr->Min = -gateBoundsRight;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->isObj = true;
	boxPtr->canPhase = true;
	boxPtr->isInteractive = true;
	boxPtr->Max = gateBoundsLeft;
	boxPtr->Min = -gateBoundsLeft;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->isObj = true;
	boxPtr->Max = cashierBounds;
	boxPtr->Min = -cashierBounds;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->isObj = true;
	boxPtr->Max = middleCashierBounds;
	boxPtr->Min = -middleCashierBounds;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->isObj = true;
	boxPtr->Max = cashierBounds;
	boxPtr->Min = -cashierBounds;
	box.push_back(boxPtr);
	/*******************/

	// Init VBO here
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Default VAO
	glGenVertexArrays(1, &m_vertexArrayID); //Generate "x" buffer
	glBindVertexArray(m_vertexArrayID);

	Mtx44 projection;
	projection.SetToPerspective(45.0f,4.0f / 3.0f, 0.1f, 30000.0f);

	projectionStack.LoadMatrix(projection);

	camera.Init(Vector3(0, 0, 0), Vector3(0, 0, -150), Vector3(0, 1, 0)); //Position , Front(view), Up

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);

	meshList[GEO_FRONT] = MeshBuilder::GenerateOBJ("Supermarket Front", "Object//Front Wall.obj");
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//Front Wall.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateOBJ("Supermarket Back", "Object//Back Wall.obj");
	meshList[GEO_BACK]->textureID = LoadTGA("Image//Back Wall.tga");
	meshList[GEO_LEFT] = MeshBuilder::GenerateOBJ("Supermarket Left", "Object//Left Wall.obj");
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//Left Wall.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateOBJ("Supermarket Right", "Object//Right Wall.obj");
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//Right Wall.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateOBJ("Ceiling", "Object//Ceiling.obj");
	meshList[GEO_TOP]->textureID = LoadTGA("Image//Ceiling.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("Floor", Color (0, 0, 0), 2500.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//Floor.tga");
	meshList[GEO_GLASSDOOR] = MeshBuilder::GenerateOBJ("glass door", "Object//glass door.obj");
	meshList[GEO_GLASSDOOR]->textureID = LoadTGA("Image//glass door.tga");

	meshList[GEO_LEFTGATE] = MeshBuilder::GenerateOBJ("left gate", "Object//left gate.obj");
	meshList[GEO_LEFTGATE]->textureID = LoadTGA("Image//gate.tga");
	meshList[GEO_RIGHTGATE] = MeshBuilder::GenerateOBJ("left gate", "Object//right gate.obj");
	meshList[GEO_RIGHTGATE]->textureID = LoadTGA("Image//gate.tga");

	meshList[GEO_CARPARK] = MeshBuilder::GenerateQuad("carpark", Color (0, 0, 0), 1.f);
	meshList[GEO_CARPARK]->textureID = LoadTGA("Image//carpark.tga");
	meshList[GEO_CAR] = MeshBuilder::GenerateOBJ("car", "Object//car.obj");
	meshList[GEO_CAR]->textureID = LoadTGA("Image//car.tga");
	meshList[GEO_GREYBRICK] = MeshBuilder::GenerateOBJ("Grey Brick", "Object//grey brick.obj");
	meshList[GEO_GREYBRICK]->textureID = LoadTGA("Image//grey brick.tga");
	meshList[GEO_GRASS] = MeshBuilder::GenerateQuadx20("grass", Color (0, 0, 0), 1.f);
	meshList[GEO_GRASS]->textureID = LoadTGA("Image//grass.tga");
	meshList[GEO_ROAD] = MeshBuilder::GenerateQuad("road", Color (0, 0, 0), 1.f);
	meshList[GEO_ROAD]->textureID = LoadTGA("Image//road.tga");
	meshList[GEO_ROAD2] = MeshBuilder::GenerateQuadx10("road", Color (0, 0, 0), 1.f);
	meshList[GEO_ROAD2]->textureID = LoadTGA("Image//road.tga");
	meshList[GEO_WHITE] = MeshBuilder::GenerateQuad("road", Color (1, 1, 1), 1.f);

	meshList[GEO_CUSTOMERSERVICE] = MeshBuilder::GenerateOBJ("Customer Service Counter and Office", "Object//customer service.obj");
	meshList[GEO_CUSTOMERSERVICE]->textureID = LoadTGA("Image//customer service.tga");
	meshList[GEO_CASHIERTABLE] = MeshBuilder::GenerateOBJ("Cashier Table", "Object//CashierTable.obj");
	meshList[GEO_CASHIERTABLE]->textureID = LoadTGA("Image//cashreg.tga");
	meshList[GEO_CASHIERTABLE2] = MeshBuilder::GenerateOBJ("Cashier Table 2", "Object//CashierTableFLIPPED.obj");
	meshList[GEO_CASHIERTABLE2]->textureID = LoadTGA("Image//cashreg.tga");
	meshList[GEO_SECURITYCOUNTER] = MeshBuilder::GenerateOBJ("Security Counter", "Object//Security Counter.obj");
	meshList[GEO_SECURITYCOUNTER]->textureID = LoadTGA("Image//security.tga");
	meshList[GEO_SHELF] = MeshBuilder::GenerateOBJ("Shelf", "Object//shelf.obj");
	meshList[GEO_SHELF]->textureID = LoadTGA("Image//shelf.tga");
	meshList[GEO_CHILLER] = MeshBuilder::GenerateOBJ("Chiller", "Object//Chiller.obj");
	meshList[GEO_CHILLER]->textureID = LoadTGA("Image//Chiller.tga");
	meshList[GEO_FREEZER] = MeshBuilder::GenerateOBJ("Shelf", "Object//Freezer.obj");
	meshList[GEO_FREEZER]->textureID = LoadTGA("Image//Freezer.tga");

	meshList[GEO_PLAYERHEAD] = MeshBuilder::GenerateOBJ("Player head", "Object//playerHead.obj");
	meshList[GEO_PLAYERHEAD]->textureID = LoadTGA("Image//skin_20150223054713114664.tga");
	meshList[GEO_PLAYERBODY] = MeshBuilder::GenerateOBJ("Player body", "Object//playerBody.obj");
	meshList[GEO_PLAYERBODY]->textureID = LoadTGA("Image//skin_20150223054713114664.tga");
	meshList[GEO_PLAYERLEFTARM] = MeshBuilder::GenerateOBJ("Player arms", "Object//playerLeftArm.obj");
	meshList[GEO_PLAYERLEFTARM]->textureID = LoadTGA("Image//skin_20150223054713114664.tga");
	meshList[GEO_PLAYERRIGHTARM] = MeshBuilder::GenerateOBJ("Player arms", "Object//playerRightArm.obj");
	meshList[GEO_PLAYERRIGHTARM]->textureID = LoadTGA("Image//skin_20150223054713114664.tga");
	meshList[GEO_PLAYERLEFTLEG] = MeshBuilder::GenerateOBJ("Player legs", "Object//playerLeftLeg.obj");
	meshList[GEO_PLAYERLEFTLEG]->textureID = LoadTGA("Image//skin_20150223054713114664.tga");
	meshList[GEO_PLAYERRIGHTLEG] = MeshBuilder::GenerateOBJ("Player legs", "Object//playerRightLeg.obj");
	meshList[GEO_PLAYERRIGHTLEG]->textureID = LoadTGA("Image//skin_20150223054713114664.tga");

	meshList[GEO_CASHIERHEAD] = MeshBuilder::GenerateOBJ("Player head", "Object//playerHead.obj");
	meshList[GEO_CASHIERHEAD]->textureID = LoadTGA("Image//skin_20150218034156170726.tga");
	meshList[GEO_CASHIERBODY] = MeshBuilder::GenerateOBJ("Player body", "Object//playerBody.obj");
	meshList[GEO_CASHIERBODY]->textureID = LoadTGA("Image//skin_20150218034156170726.tga");
	meshList[GEO_CASHIERLEFTARM] = MeshBuilder::GenerateOBJ("Player arms", "Object//playerLeftArm.obj");
	meshList[GEO_CASHIERLEFTARM]->textureID = LoadTGA("Image//skin_20150218034156170726.tga");
	meshList[GEO_CASHIERRIGHTARM] = MeshBuilder::GenerateOBJ("Player arms", "Object//playerRightArm.obj");
	meshList[GEO_CASHIERRIGHTARM]->textureID = LoadTGA("Image//skin_20150218034156170726.tga");
	meshList[GEO_CASHIERLEFTLEG] = MeshBuilder::GenerateOBJ("Player legs", "Object//playerLeftLeg.obj");
	meshList[GEO_CASHIERLEFTLEG]->textureID = LoadTGA("Image//skin_20150218034156170726.tga");
	meshList[GEO_CASHIERRIGHTLEG] = MeshBuilder::GenerateOBJ("Player legs", "Object//playerRightLeg.obj");
	meshList[GEO_CASHIERRIGHTLEG]->textureID = LoadTGA("Image//skin_20150218034156170726.tga");

	meshList[GEO_SGHEAD] = MeshBuilder::GenerateOBJ("Player head", "Object//playerHead.obj");
	meshList[GEO_SGHEAD]->textureID = LoadTGA("Image//skin_2014110205564268218.tga");
	meshList[GEO_SGBODY] = MeshBuilder::GenerateOBJ("Player body", "Object//playerBody.obj");
	meshList[GEO_SGBODY]->textureID = LoadTGA("Image//skin_2014110205564268218.tga");
	meshList[GEO_SGLEFTARM] = MeshBuilder::GenerateOBJ("Player arms", "Object//playerLeftArm.obj");
	meshList[GEO_SGLEFTARM]->textureID = LoadTGA("Image//skin_2014110205564268218.tga");
	meshList[GEO_SGRIGHTARM] = MeshBuilder::GenerateOBJ("Player arms", "Object//playerRightArm.obj");
	meshList[GEO_SGRIGHTARM]->textureID = LoadTGA("Image//skin_2014110205564268218.tga");
	meshList[GEO_SGLEFTLEG] = MeshBuilder::GenerateOBJ("Player legs", "Object//playerLeftLeg.obj");
	meshList[GEO_SGLEFTLEG]->textureID = LoadTGA("Image//skin_2014110205564268218.tga");
	meshList[GEO_SGRIGHTLEG] = MeshBuilder::GenerateOBJ("Player legs", "Object//playerRightLeg.obj");
	meshList[GEO_SGRIGHTLEG]->textureID = LoadTGA("Image//skin_2014110205564268218.tga");

	meshList[GEO_CUSTOMERHEAD] = MeshBuilder::GenerateOBJ("Player head", "Object//playerHead.obj");
	meshList[GEO_CUSTOMERHEAD]->textureID = LoadTGA("Image//skin_20150222212123152908.tga");
	meshList[GEO_CUSTOMERBODY] = MeshBuilder::GenerateOBJ("Player body", "Object//playerBody.obj");
	meshList[GEO_CUSTOMERBODY]->textureID = LoadTGA("Image//skin_20150222212123152908.tga");
	meshList[GEO_CUSTOMERLEFTARM] = MeshBuilder::GenerateOBJ("Player arms", "Object//playerLeftArm.obj");
	meshList[GEO_CUSTOMERLEFTARM]->textureID = LoadTGA("Image//skin_20150222212123152908.tga");
	meshList[GEO_CUSTOMERRIGHTARM] = MeshBuilder::GenerateOBJ("Player arms", "Object//playerRightArm.obj");
	meshList[GEO_CUSTOMERRIGHTARM]->textureID = LoadTGA("Image//skin_20150222212123152908.tga");
	meshList[GEO_CUSTOMERLEFTLEG] = MeshBuilder::GenerateOBJ("Player legs", "Object//playerLeftLeg.obj");
	meshList[GEO_CUSTOMERLEFTLEG]->textureID = LoadTGA("Image//skin_20150222212123152908.tga");
	meshList[GEO_CUSTOMERRIGHTLEG] = MeshBuilder::GenerateOBJ("Player legs", "Object//playerRightLeg.obj");
	meshList[GEO_CUSTOMERRIGHTLEG]->textureID = LoadTGA("Image//skin_20150222212123152908.tga");

	meshList[GEO_PROMOTERHEAD] = MeshBuilder::GenerateOBJ("Player head", "Object//playerHead.obj");
	meshList[GEO_PROMOTERHEAD]->textureID = LoadTGA("Image//Best_Buy_Employee.tga");
	meshList[GEO_PROMOTERBODY] = MeshBuilder::GenerateOBJ("Player body", "Object//playerBody.obj");
	meshList[GEO_PROMOTERBODY]->textureID = LoadTGA("Image//Best_Buy_Employee.tga");
	meshList[GEO_PROMOTERLEFTARM] = MeshBuilder::GenerateOBJ("Player arms", "Object//playerLeftArm.obj");
	meshList[GEO_PROMOTERLEFTARM]->textureID = LoadTGA("Image//Best_Buy_Employee.tga");
	meshList[GEO_PROMOTERRIGHTARM] = MeshBuilder::GenerateOBJ("Player arms", "Object//playerRightArm.obj");
	meshList[GEO_PROMOTERRIGHTARM]->textureID = LoadTGA("Image//Best_Buy_Employee.tga");
	meshList[GEO_PROMOTERLEFTLEG] = MeshBuilder::GenerateOBJ("Player legs", "Object//playerLeftLeg.obj");
	meshList[GEO_PROMOTERLEFTLEG]->textureID = LoadTGA("Image//Best_Buy_Employee.tga");
	meshList[GEO_PROMOTERRIGHTLEG] = MeshBuilder::GenerateOBJ("Player legs", "Object//playerRightLeg.obj");
	meshList[GEO_PROMOTERRIGHTLEG]->textureID = LoadTGA("Image//Best_Buy_Employee.tga");
	meshList[GEO_PROMOTERTABLE] = MeshBuilder::GenerateOBJ("Promoter's table", "Object//promoterTable.obj");
	meshList[GEO_PROMOTERTABLE]->textureID = LoadTGA("Image//dark.tga");

	meshList[GEO_OWNERHEAD] = MeshBuilder::GenerateOBJ("Player head", "Object//playerHead.obj");
	meshList[GEO_OWNERHEAD]->textureID = LoadTGA("Image//rich_kid.tga");
	meshList[GEO_OWNERBODY] = MeshBuilder::GenerateOBJ("Player body", "Object//playerBody.obj");
	meshList[GEO_OWNERBODY]->textureID = LoadTGA("Image//rich_kid.tga");
	meshList[GEO_OWNERLEFTARM] = MeshBuilder::GenerateOBJ("Player arms", "Object//playerLeftArm.obj");
	meshList[GEO_OWNERLEFTARM]->textureID = LoadTGA("Image//rich_kid.tga");
	meshList[GEO_OWNERRIGHTARM] = MeshBuilder::GenerateOBJ("Player arms", "Object//playerRightArm.obj");
	meshList[GEO_OWNERRIGHTARM]->textureID = LoadTGA("Image//rich_kid.tga");
	meshList[GEO_OWNERLEFTLEG] = MeshBuilder::GenerateOBJ("Player legs", "Object//playerLeftLeg.obj");
	meshList[GEO_OWNERLEFTLEG]->textureID = LoadTGA("Image//rich_kid.tga");
	meshList[GEO_OWNERRIGHTLEG] = MeshBuilder::GenerateOBJ("Player legs", "Object//playerRightLeg.obj");
	meshList[GEO_OWNERRIGHTLEG]->textureID = LoadTGA("Image//rich_kid.tga");
	
	meshList[GEO_PASSERBYHEAD] = MeshBuilder::GenerateOBJ("Player head", "Object//playerHead.obj");
	meshList[GEO_PASSERBYHEAD]->textureID = LoadTGA("Image//unicorn67_Business.tga");
	meshList[GEO_PASSERBYBODY] = MeshBuilder::GenerateOBJ("Player body", "Object//playerBody.obj");
	meshList[GEO_PASSERBYBODY]->textureID = LoadTGA("Image//unicorn67_Business.tga");
	meshList[GEO_PASSERBYLEFTARM] = MeshBuilder::GenerateOBJ("Player arms", "Object//playerLeftArm.obj");
	meshList[GEO_PASSERBYLEFTARM]->textureID = LoadTGA("Image//unicorn67_Business.tga");
	meshList[GEO_PASSERBYRIGHTARM] = MeshBuilder::GenerateOBJ("Player arms", "Object//playerRightArm.obj");
	meshList[GEO_PASSERBYRIGHTARM]->textureID = LoadTGA("Image//unicorn67_Business.tga");
	meshList[GEO_PASSERBYLEFTLEG] = MeshBuilder::GenerateOBJ("Player legs", "Object//playerLeftLeg.obj");
	meshList[GEO_PASSERBYLEFTLEG]->textureID = LoadTGA("Image//unicorn67_Business.tga");
	meshList[GEO_PASSERBYRIGHTLEG] = MeshBuilder::GenerateOBJ("Player legs", "Object//playerRightLeg.obj");
	meshList[GEO_PASSERBYRIGHTLEG]->textureID = LoadTGA("Image//unicorn67_Business.tga");

	meshList[GEO_CACTUSJUICE] = MeshBuilder::GenerateOBJ("Cactus Juice", "Object//CactusJuice.obj");
	meshList[GEO_CACTUSJUICE]->textureID = LoadTGA("Image//CactusJuice.tga");
	meshList[GEO_CADBURY] = MeshBuilder::GenerateOBJ("Cadbury", "Object//Cadbury.obj");
	meshList[GEO_CADBURY]->textureID = LoadTGA("Image//Cadbury.tga");
	meshList[GEO_CANVEGE] = MeshBuilder::GenerateOBJ("Canned Vegetables", "Object//CanVege.obj");
	meshList[GEO_CANVEGE]->textureID = LoadTGA("Image//CanVege.tga");
	meshList[GEO_CATFOOD] = MeshBuilder::GenerateOBJ("Cat Food", "Object//CatFood.obj");
	meshList[GEO_CATFOOD]->textureID = LoadTGA("Image//CatFood2.tga");
	meshList[GEO_CEREAL] = MeshBuilder::GenerateOBJ("Cereal", "Object//Cereal.obj");
	meshList[GEO_CEREAL]->textureID = LoadTGA("Image//Cereal.tga");
	meshList[GEO_DETERGENT] = MeshBuilder::GenerateOBJ("Detergent", "Object//Detergent.obj");
	meshList[GEO_DETERGENT]->textureID = LoadTGA("Image//Detergent.tga");
	meshList[GEO_DUREX] = MeshBuilder::GenerateOBJ("dUREX", "Object//durex.obj");
	meshList[GEO_DUREX]->textureID = LoadTGA("Image//durex.tga");
	meshList[GEO_FERERRO] = MeshBuilder::GenerateOBJ("Fererro", "Object//Fererro.obj");
	meshList[GEO_FERERRO]->textureID = LoadTGA("Image//Fererro.tga");
	meshList[GEO_MACCHZ] = MeshBuilder::GenerateOBJ("Mac Chz", "Object//MacChz.obj");
	meshList[GEO_MACCHZ]->textureID = LoadTGA("Image//MacChz.tga");
	meshList[GEO_MILK] = MeshBuilder::GenerateOBJ("Milk", "Object//milk.obj");
	meshList[GEO_MILK]->textureID = LoadTGA("Image//milk.tga");
	meshList[GEO_MILOCAN] = MeshBuilder::GenerateOBJ("Milo", "Object//MiloCan.obj");
	meshList[GEO_MILOCAN]->textureID = LoadTGA("Image//milocan.tga");
	meshList[GEO_MOUNTAINDEW] = MeshBuilder::GenerateOBJ("Mountain Dew", "Object//Mountain Dew.obj");
	meshList[GEO_MOUNTAINDEW]->textureID = LoadTGA("Image//Mountain Dew Txture.tga");
	meshList[GEO_PIZZAHUT] = MeshBuilder::GenerateOBJ("Pizza", "Object//PizzaHut.obj");
	meshList[GEO_PIZZAHUT]->textureID = LoadTGA("Image//pizzahut.tga");
	meshList[GEO_POTATOCHIPS] = MeshBuilder::GenerateOBJ("Potato Chips", "Object//PotatoChips.obj");
	meshList[GEO_POTATOCHIPS]->textureID = LoadTGA("Image//PotatoChips.tga");
	meshList[GEO_SARDINES] = MeshBuilder::GenerateOBJ("Sardines", "Object//Sardines.obj");
	meshList[GEO_SARDINES]->textureID = LoadTGA("Image//Sardines.tga");
	meshList[GEO_SPAM] = MeshBuilder::GenerateOBJ("Spam", "Object//Spam.obj");
	meshList[GEO_SPAM]->textureID = LoadTGA("Image//Spam.tga");

	meshList[GEO_OUTSIDEFRONT] = MeshBuilder::GenerateQuad("sky front", Color (0, 0, 0), 1);
	meshList[GEO_OUTSIDEFRONT]->textureID = LoadTGA("Image//front.tga");
	meshList[GEO_OUTSIDEBACK] = MeshBuilder::GenerateQuad("sky back", Color (0, 0, 0), 1);
	meshList[GEO_OUTSIDEBACK]->textureID = LoadTGA("Image//back.tga");
	meshList[GEO_OUTSIDELEFT] = MeshBuilder::GenerateQuad("sky left", Color (0, 0, 0), 1);
	meshList[GEO_OUTSIDELEFT]->textureID = LoadTGA("Image//left.tga");
	meshList[GEO_OUTSIDERIGHT] = MeshBuilder::GenerateQuad("sky right", Color (0, 0, 0), 1);
	meshList[GEO_OUTSIDERIGHT]->textureID = LoadTGA("Image//right.tga");
	meshList[GEO_OUTSIDETOP] = MeshBuilder::GenerateQuad("sky top", Color (0, 0, 0), 1);
	meshList[GEO_OUTSIDETOP]->textureID = LoadTGA("Image//top.tga");
	meshList[GEO_OUTSIDEBOTTOM] = MeshBuilder::GenerateQuad("sky bottom", Color (0, 0, 0), 1.f);
	meshList[GEO_OUTSIDEBOTTOM]->textureID = LoadTGA("Image//bottom.tga");

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("light", Color(1,1,1), 10, 10, 50);

	meshList[GEO_BOUNDHELPER] = MeshBuilder::GenerateSphere("BoundHelper", Color(1,1,1), 10, 18, 2.f);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//ExportedFont.tga");

	meshList[GEO_MENU] = MeshBuilder::GenerateQuad("Menu", Color (0, 0, 0), 1.f);
	meshList[GEO_MENU]->textureID = LoadTGA("Image//Menu.tga");
	/***************************************************************************************************************
	USER INERFACE
	***************************************************************************************************************/
	meshList[GEO_SIDEBAR] = MeshBuilder::GenerateQuad("sidebar", Color (0, 0, 0), 1);
	meshList[GEO_SIDEBAR]->textureID = LoadTGA("Image//sidebar.tga");

	meshList[GEO_CACTUSJUICEUI] = MeshBuilder::GenerateQuad("CactusJuice ui", Color (0, 0, 0), 1);
	meshList[GEO_CACTUSJUICEUI]->textureID = LoadTGA("Image//CactusJuice UI.tga");
	meshList[GEO_CADBURYUI] = MeshBuilder::GenerateQuad("Cadbury ui", Color (0, 0, 0), 1);
	meshList[GEO_CADBURYUI]->textureID = LoadTGA("Image//Cadbury UI.tga");
	meshList[GEO_CANVEGEUI] = MeshBuilder::GenerateQuad("Canned Vegetables ui", Color (0, 0, 0), 1);
	meshList[GEO_CANVEGEUI]->textureID = LoadTGA("Image//CanVege UI.tga");
	meshList[GEO_CATFOODUI] = MeshBuilder::GenerateQuad("Cat Foodui", Color (0, 0, 0), 1);
	meshList[GEO_CATFOODUI]->textureID = LoadTGA("Image//CatFood2 UI.tga");
	meshList[GEO_CEREALUI] = MeshBuilder::GenerateQuad("Cereal ui", Color (0, 0, 0), 1);
	meshList[GEO_CEREALUI]->textureID = LoadTGA("Image//Cereal UI.tga");
	meshList[GEO_DETERGENTUI] = MeshBuilder::GenerateQuad("Detergent ui", Color (0, 0, 0), 1);
	meshList[GEO_DETERGENTUI]->textureID = LoadTGA("Image//Detergent UI.tga");
	meshList[GEO_FERERROUI] = MeshBuilder::GenerateQuad("Fererro ui", Color (0, 0, 0), 1);
	meshList[GEO_FERERROUI]->textureID = LoadTGA("Image//Fererro UI.tga");
	meshList[GEO_MACCHZUI] = MeshBuilder::GenerateQuad("Mac Chz ui", Color (0, 0, 0), 1);
	meshList[GEO_MACCHZUI]->textureID = LoadTGA("Image//MacChz UI.tga");
	meshList[GEO_MILOCANUI] = MeshBuilder::GenerateQuad("Milo ui", Color (0, 0, 0), 1);
	meshList[GEO_MILOCANUI]->textureID = LoadTGA("Image//milocan UI.tga");
	meshList[GEO_POTATOCHIPSUI] = MeshBuilder::GenerateQuad("Potato Chips ui", Color (0, 0, 0), 1);
	meshList[GEO_POTATOCHIPSUI]->textureID = LoadTGA("Image//PotatoChips UI.tga");
	meshList[GEO_SARDINESUI] = MeshBuilder::GenerateQuad("Sardines ui", Color (0, 0, 0), 1);
	meshList[GEO_SARDINESUI]->textureID = LoadTGA("Image//Sardines UI.tga");

	meshList[GEO_TEXTBOX] = MeshBuilder::GenerateQuad("text box", Color (0, 0, 0), 1);
	meshList[GEO_TEXTBOX]->textureID = LoadTGA("Image//text box.tga");

	meshList[GEO_SHOPPINGLIST] = MeshBuilder::GenerateQuad("shopping list", Color (0, 0, 0), 1);
	meshList[GEO_SHOPPINGLIST]->textureID = LoadTGA("Image//shopping list.tga");
	/***************************************************************************************************************
	THIS SECTION IS FOR BOUNDS MESH INIT
	***************************************************************************************************************/
	meshList[GEO_MARKETWALL1] = MeshBuilder::GenerateBoundingBox("Left Wall", box[MARKETWALL1]->Max, box[MARKETWALL1]->Min, Color(0,1,1));
	meshList[GEO_MARKETWALL2] = MeshBuilder::GenerateBoundingBox("Right Wall", box[MARKETWALL2]->Max, box[MARKETWALL2]->Min, Color(0,1,1));
	meshList[GEO_MARKETWALL3] = MeshBuilder::GenerateBoundingBox("Back Wall", box[MARKETWALL3]->Max, box[MARKETWALL3]->Min, Color(0,1,1));
	meshList[GEO_MARKETWALL4] = MeshBuilder::GenerateBoundingBox("Back Wall", box[MARKETWALL4]->Max, box[MARKETWALL4]->Min, Color(0,1,1));

	meshList[GEO_SHELFBOUNDS1] = MeshBuilder::GenerateBoundingBox("ShelfBounds1", box[SHELF1]->Max, box[SHELF1]->Min, Color(0,0,1));
	meshList[GEO_SHELFBOUNDS2] = MeshBuilder::GenerateBoundingBox("Shelf2Bounds", box[SHELF2]->Max, box[SHELF2]->Min, Color(0,0,1));
	meshList[GEO_SHELFBOUNDS3] = MeshBuilder::GenerateBoundingBox("Shelf3Bounds", box[SHELF3]->Max, box[SHELF3]->Min, Color(0,0,1));
	meshList[GEO_SHELFBOUNDS4] = MeshBuilder::GenerateBoundingBox("Shelf4Bounds", box[SHELF4]->Max, box[SHELF4]->Min, Color(0,0,1));
	meshList[GEO_SHELFBOUNDS5] = MeshBuilder::GenerateBoundingBox("Shelf5Bounds", box[SHELF5]->Max, box[SHELF5]->Min, Color(0,0,1));
	meshList[GEO_DOORBOUNDS] = MeshBuilder::GenerateBoundingBox("doorbounds", box[DOOR]->Max, box[DOOR]->Min, Color(0,1,0));
	meshList[GEO_PLAYERBOUNDS] = MeshBuilder::GenerateBoundingBox("Player", box[PLAYER]->Max, box[PLAYER]->Min, Color(0,0,0));
	meshList[GEO_FREEZERBOUNDS] = MeshBuilder::GenerateBoundingBox("freezer", box[FREEZER]->Max, box[FREEZER]->Min, Color(0,0,1));
	meshList[GEO_CHILLERBOUNDS] = MeshBuilder::GenerateBoundingBox("freezer", box[CHILLER]->Max, box[CHILLER]->Min, Color(0,0,1));
	meshList[GEO_GATEBOUNDS1] = MeshBuilder::GenerateBoundingBox("gatebounds", box[GATE1]->Max, box[GATE1]->Min, Color(0,1,0));
	meshList[GEO_GATEBOUNDS2] = MeshBuilder::GenerateBoundingBox("gatebounds", box[GATE2]->Max, box[GATE2]->Min, Color(0,1,0));
	meshList[GEO_CASHIERRIGHTBOUNDS] = MeshBuilder::GenerateBoundingBox("rightCashier", box[RIGHTCASHIER]->Max, box[RIGHTCASHIER]->Min, Color(0,0,1));
	meshList[GEO_MIDDLECASHIERBOUNDS] = MeshBuilder::GenerateBoundingBox("middleCashier", box[MIDDLECASHIER]->Max, box[MIDDLECASHIER]->Min, Color(0,0,1));
	meshList[GEO_LEFTCASHIERBOUNDS] = MeshBuilder::GenerateBoundingBox("leftCashier", box[LEFTCASHIER]->Max, box[LEFTCASHIER]->Min, Color(0,0,1));
	/**************************************************************************************************************/

	/***************************************************
	FOR SETTING NEW BOUNDS BECAUSE OF TRANSLATION UPDATE
	***************************************************/

	box[DOOR]->Max += doorTranslation;
	box[DOOR]->Min += doorTranslation;
	box[SHELF1]->Max += leftShelfTranslate;
	box[SHELF1]->Min += leftShelfTranslate;
	box[SHELF2]->Max += middleLeftShelfTranslate;
	box[SHELF2]->Min += middleLeftShelfTranslate;
	box[SHELF3]->Max += middleRightShelfTranslate;
	box[SHELF3]->Min += middleRightShelfTranslate;
	box[SHELF4]->Max += backShelfTranslate;
	box[SHELF4]->Min += backShelfTranslate;
	box[SHELF5]->Max += loneShelfTranslate;
	box[SHELF5]->Min += loneShelfTranslate;

	box[MARKETWALL1]->Max += MarketWallNorthSouthTranslate;
	box[MARKETWALL1]->Min += MarketWallNorthSouthTranslate;
	box[MARKETWALL2]->Max += -MarketWallNorthSouthTranslate;
	box[MARKETWALL2]->Min += -MarketWallNorthSouthTranslate;
	box[MARKETWALL3]->Max += MarketWallEastWestTranslate;
	box[MARKETWALL3]->Min += MarketWallEastWestTranslate;
	box[MARKETWALL4]->Max += -MarketWallEastWestTranslate;
	box[MARKETWALL4]->Min += -MarketWallEastWestTranslate;
	
	box[FREEZER]->Max += FreezerTranslate;
	box[FREEZER]->Min += FreezerTranslate;
	box[CHILLER]->Max += ChillerTranslate;
	box[CHILLER]->Min += ChillerTranslate;

	box[GATE1]->Max += gateBounds1Translation;
	box[GATE1]->Min += gateBounds1Translation;
	box[GATE2]->Max += gateBounds2Translation;
	box[GATE2]->Min += gateBounds2Translation;

	box[RIGHTCASHIER]->Max += rightCashierTranslate;
	box[RIGHTCASHIER]->Min += rightCashierTranslate;
	box[MIDDLECASHIER]->Max += middleCashierTranslate;
	box[MIDDLECASHIER]->Min += middleCashierTranslate;
	box[LEFTCASHIER]->Max += leftCashierTranslate;
	box[LEFTCASHIER]->Min += leftCashierTranslate;
	/***************************
	FOR ADDING ITEMS & SHELFSLOTS
	****************************/
	inhand = new Inhand(5);
	Item* ip;
	Shelfslot* sp;

	for(float z = -420; z >= -750; z -= 66)
	{
		for(float y = 27; y >= -250; y -= 50)
		{
			ip = new Item(Vector3(12,36,12), Vector3(-12,0,-12), Vector3(-2000, y , z), Vector3(80,80,80), 90.f, GEO_CACTUSJUICE, GEO_CACTUSJUICEUI);
			itemVector.push_back(ip);
			sp = new Shelfslot(Vector3(-2000, y , z),Vector3(22,36,22), Vector3(-22,0,-22), itemVector.size()-1, false);
			shelfVector.push_back(sp);
		}
	}

	for(float z = -920; z >= -1270; z -= 70)
	{
		for(float y = 30; y >= -240; y -= 50)
		{
			ip = new Item(Vector3(12,13,27), Vector3(-12,0,-27), Vector3(-2030, y , z), Vector3(80,80,80), 90.f, GEO_CADBURY, GEO_CADBURYUI);
			itemVector.push_back(ip);
			sp = new Shelfslot(Vector3(-2030, y , z),Vector3(22,20,27), Vector3(-22,0,-27), itemVector.size()-1, false);
			shelfVector.push_back(sp);
		}
	}

	for(float z = -1430; z >= -1760; z -= 66)
	{
		for(float y = 31; y >= -268; y -= 50)
		{
			ip = new Item(Vector3(17,36,17), Vector3(-17,0,-17), Vector3(-2005, y , z), Vector3(60,60,60), 90.f, GEO_CANVEGE, GEO_CANVEGEUI);
			itemVector.push_back(ip);
			sp = new Shelfslot(Vector3(-2005, y , z),Vector3(22,36,22), Vector3(-22,0,-22), itemVector.size()-1, false);
			shelfVector.push_back(sp);
		}
	}

	for(float x = -1200; x <= -800; x += 80)
	{
		for(float y = 25; y > -275; y -= 50)
		{
			ip = new Item(Vector3(15,40,8), Vector3(-15,0,-8), Vector3(x, y , -1640), Vector3(15,15,15), 0.f, GEO_DETERGENT, GEO_DETERGENTUI);
			itemVector.push_back(ip);
			sp = new Shelfslot(Vector3(x, y , -1640),Vector3(22,40,22), Vector3(-22,0,-22), itemVector.size()-1, false);
			shelfVector.push_back(sp);
		}
	}

	for(float x = -700; x <= -300; x += 80)
	{
		for(float y = 25; y > -275; y -= 50)
		{
			ip = new Item(Vector3(15,40,6), Vector3(-15,0,-6), Vector3(x, y , -1650), Vector3(30,30,30), 0.f, GEO_MACCHZ, GEO_MACCHZUI);
			itemVector.push_back(ip);
			sp = new Shelfslot(Vector3(x, y , -1650),Vector3(22,40,22), Vector3(-22,0,-22), itemVector.size()-1, false);
			shelfVector.push_back(sp);
		}
	}

	for(float x = -150; x <= 180; x += 66)
	{
		for(float y = 25; y > -275; y -= 50)
		{
			ip = new Item(Vector3(20,40,20), Vector3(-20,0,-20), Vector3(x, y , -1650), Vector3(40,40,40), -45.f, GEO_MILOCAN, GEO_MILOCANUI);
			itemVector.push_back(ip);
			sp = new Shelfslot(Vector3(x, y , -1650),Vector3(22,40,22), Vector3(-22,0,-22), itemVector.size()-1, false);
			shelfVector.push_back(sp);
		}
	}

	for(float z = -1000; z <= -625; z += 75)
	{
		for(float y = 27; y > -273; y -= 50)
		{
			ip = new Item(Vector3(14,40,14), Vector3(-14,0,-14), Vector3(-1415, y , z), Vector3(15,13,15), 90.f, GEO_CATFOOD, GEO_CATFOODUI);
			itemVector.push_back(ip);
			sp = new Shelfslot(Vector3(-1415, y , z),Vector3(22,40,22), Vector3(-22,0,-22), itemVector.size()-1, false);
			shelfVector.push_back(sp);
		}
	}

	for(float z = -650; z >= -1000; z -= 70)
	{
		for(float y = 27; y > -273; y -= 50)
		{
			ip = new Item(Vector3(6,40,16), Vector3(-6,0,-16), Vector3(-1000, y , z), Vector3(30,30,30), 90.f, GEO_CEREAL, GEO_CEREALUI);
			itemVector.push_back(ip);
			sp = new Shelfslot(Vector3(-1000, y , z),Vector3(22,40,22), Vector3(-22,0,-22), itemVector.size()-1, false);
			shelfVector.push_back(sp);
		}
	}

	for(float z = -990; z <= -660; z += 66)
	{
		for(float y = 28; y >= -270; y -= 50)
		{
			ip = new Item(Vector3(20,7,20), Vector3(-20,0,-20), Vector3(-510, y , z), Vector3(30,30,30), -90.f, GEO_FERERRO, GEO_FERERROUI);
			itemVector.push_back(ip);
			sp = new Shelfslot(Vector3(-510, y , z),Vector3(22,40,22), Vector3(-22,0,-22), itemVector.size()-1, false);
			shelfVector.push_back(sp);
		}
	}

	for(float z = -660; z >= -990; z -= 66)
	{
		for(float y = 28; y >= -270; y -= 50)
		{
			ip = new Item(Vector3(5,40,14), Vector3(-5,0,-14), Vector3(-100, y , z), Vector3(35,35,35), 90.f, GEO_POTATOCHIPS, GEO_POTATOCHIPSUI);
			itemVector.push_back(ip);
			sp = new Shelfslot(Vector3(-100, y , z),Vector3(22,40,22), Vector3(-22,0,-22), itemVector.size()-1, false);
			shelfVector.push_back(sp);
		}
	}

	for(float x = 750; x >= 425; x -= 65)
	{
	 	for(float y = 26; y > -274; y -= 50)
		{
			ip = new Item(Vector3(12,36,12), Vector3(-12,0,-12), Vector3(x, y , -770), Vector3(45,45,45), 0.f, GEO_SARDINES, GEO_SARDINESUI);
			itemVector.push_back(ip);
			sp = new Shelfslot(Vector3(x, y , -770),Vector3(22,36,22), Vector3(-22,0,-22), itemVector.size()-1, false);
			shelfVector.push_back(sp);
		}
	}
	/***************************
	FOR SHOPPING
	****************************/
	vector<int> GEOS;
	GEOS.push_back(GEO_CACTUSJUICE);
	GEOS.push_back(GEO_CADBURY);
	GEOS.push_back(GEO_CANVEGE);
	GEOS.push_back(GEO_CATFOOD);
	GEOS.push_back(GEO_CEREAL);
	GEOS.push_back(GEO_DETERGENT);
	GEOS.push_back(GEO_FERERRO);
	GEOS.push_back(GEO_MACCHZ);
	GEOS.push_back(GEO_MILOCAN);
	GEOS.push_back(GEO_POTATOCHIPS);
	GEOS.push_back(GEO_SARDINES);

	shopping.set(GEOS);
	
	shoppingList = shopping.randomList(5);
	/******************************/
}

void StudioProject2::RenderMesh(Mesh *mesh, bool enableLight, bool transparent)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if(transparent)
	{
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	}
	else
	{
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}

	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}

	mesh->Render(); //this line should only be called once 

	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void StudioProject2::Update(double dt, double xpos, double ypos)
{
	if(stoptime == false)
	{
	if(Application::IsKeyPressed('5'))
	{
		rotateAngle += 0.1f;
		cout << rotateAngle << endl;
	}

	if(Application::IsKeyPressed('6'))
	{
		rotateAngle -= 0.1f;
		cout << rotateAngle << endl;
	}
	if(Application::IsKeyPressed('1'))
	{
		glEnable(GL_CULL_FACE);
	}
	if(Application::IsKeyPressed('2'))
	{
		glDisable(GL_CULL_FACE);
	}
	if(Application::IsKeyPressed('3'))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if(Application::IsKeyPressed('4'))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if(Application::IsKeyPressed('I'))
	{
		light[0].position.z -= (float)(LSPEED * dt);
	}
	if(Application::IsKeyPressed('K'))
	{
		light[0].position.z += (float)(LSPEED * dt);
	}
	if(Application::IsKeyPressed('J'))
	{
		light[0].position.x -= (float)(LSPEED * dt);
	}
	if(Application::IsKeyPressed('L'))
	{
		light[0].position.x += (float)(LSPEED * dt);
	}
	if(Application::IsKeyPressed('O'))
	{
		light[0].position.y -= (float)(LSPEED * dt);
	}
	if(Application::IsKeyPressed('P'))
	{
		light[0].position.y += (float)(LSPEED * dt);
	}
	if(Application::IsKeyPressed('Z')) // on
	{
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
		glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
		glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	}
	if(Application::IsKeyPressed('X')) // off
	{
		glUniform1f(m_parameters[U_LIGHT0_POWER],0.f);
		glUniform1f(m_parameters[U_LIGHT0_KC], 0.f);
		glUniform1f(m_parameters[U_LIGHT0_KL], 0.f);
		glUniform1f(m_parameters[U_LIGHT0_KQ], 0.f);
	}

	if(Application::IsKeyPressed('T'))
	{
		//playerPos += playerDir * moveSpeed;
		testPos.z += 1.f;
		cout << "Test Pos : " << testPos << endl;
	}
	if(Application::IsKeyPressed('G'))
	{
		//playerPos -= playerDir * moveSpeed;
		testPos.z -= 1.f;
		cout << "Test Pos : " << testPos << endl;
	}
	if(Application::IsKeyPressed('F'))
	{
		//rotateAngle += rotateSpeed * dt;
		testPos.x += 1.f;
		cout << "Test Pos : " << testPos << endl;
	}
	if(Application::IsKeyPressed('H'))
	{
		//rotateAngle -= rotateSpeed * dt;
		testPos.x -= 1.f;
		cout << "Test Pos : " << testPos << endl;
	}

	///////////////////showing shopping list////////////////////////
	if(Application::IsKeyPressed('I') && listlocation > 1.4)
	{
		listlocation -= dt;
	}
	else if((!Application::IsKeyPressed('I')) && listlocation < 2.15)
	{
		listlocation += dt;
	}
	if(listlocation <= 1.4)
	{
		showList = true;
	}
	else
		showList = false;
	/////////////////////////////////////////////////////////////

	myPasserby->spawnAI();
	myPasserby->updateAI();

	myPromoter->spawnAI();
	myPromoter->updateAI();

	myCustomer->spawnAI();
	myCustomer->updateAI();

	myVehicle->spawnAI();
	myVehicle->updateAI();

	myCashier->spawnAI();
	myCashier->updateAI();

	myGuard->spawnAI();
	myGuard->updateAI();

	playerDir.x = dt * sin(Math::DegreeToRadian(rotateAngle));
	playerDir.z = dt * cos(Math::DegreeToRadian(rotateAngle));
	playerDir.y = 0.f;

	FPS = 1 / dt;
	std::ostringstream s;
	s << setprecision(9) << FPS;
	textPS = s.str();

	deltaTime = dt;
	toggleDelay += dt;

	camera.Update(dt,canMove, xpos, ypos);
	CollisionCheck(dt);

	playerPos = camera.position;
	box[PLAYER]->Max = playerPos + playerBounds;
	box[PLAYER]->Min = playerPos - playerBounds;

	if(canMove)
	{
		tempStorage = playerPos;
		tempTarget = camera.target;
		tempUp = camera.up;
	}
	else
	{
		camera.position = tempStorage;
		camera.target = tempTarget;
		camera.up = tempUp;
		canMove = true;
	}
	int takeItem = rand() % itemsonShelf;
	//camera.target.Set(CustomerX[0], 0, CustomerZ[0]);
	//camera.position.Set(CustomerX[0], 0, CustomerZ[0] + 500);
	}
}

void StudioProject2::Render()
{                                                              //MATRIX WILL BE DONE USING THIS
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt
		(camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z);

	camera.mouseSpeed = 0.2f;  //Brute fix for camera.

	modelStack.LoadIdentity();

	Position lightPosition_cameraspace = viewStack.Top() * light[0].position;

	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); // Remember, matrix multiplication is the other way around

	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); //update the shader with new MVP

	if(stoptime == true)
	{
		MainMenu();
	}
	else
	{
	//RenderMesh(meshList[GEO_AXES], false, false);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	renderBounds();

	//////////////////////////////////////////////////////////////////////////////////

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false, false);
	modelStack.PopMatrix();

	///////////////////////////////////////////////////////////////////////////////////

	/*modelStack.PushMatrix();
	modelStack.Translate(camera.target.x, camera.target.y, camera.target.z);
	modelStack.Scale(0.2,0.2,0.2);
	RenderMesh(meshList[GEO_LIGHTBALL], false, false);
	modelStack.PopMatrix();*/
	
	///////////////////////////////////////////////////////////////////////////////////
	renderSkybox();
	renderOutside();
	renderItems();
	renderSupermarket();

	/*modelStack.PushMatrix();
	modelStack.Translate(playerPos.x, playerPos.y, playerPos.z);
	modelStack.Rotate(rotateAngle,0,1,0);
	modelStack.PushMatrix();
	modelStack.Rotate(-90,0,1,0);
	renderPlayer();
	modelStack.PopMatrix();
	modelStack.PopMatrix();*/


	for ( int i = 0; i < myCashier->AICurrent; i++ )
	{
		modelStack.PushMatrix();
		modelStack.Translate(myCashier->Coordinates[i].x, myCashier->Coordinates[i].y, myCashier->Coordinates[i].z);
		modelStack.Rotate(myCashier->rotateY[i], 0, 1, 0);
		renderCashier();
		modelStack.PopMatrix();
	}

	for ( int i = 0; i < myCustomer->AICurrent; i++ )
	{
		modelStack.PushMatrix();
		modelStack.Translate(myCustomer->Coordinates[i].x, myCustomer->Coordinates[i].y, myCustomer->Coordinates[i].z);
		modelStack.Rotate(myCustomer->rotateY[i], 0, 1, 0);
		renderCustomer();
		modelStack.PopMatrix();
	}

	for ( int i = 0; i < myPasserby->AICurrent; i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(myPasserby->Coordinates[i].x, myPasserby->Coordinates[i].y, myPasserby->Coordinates[i].z);
		modelStack.Rotate(myPasserby->rotateY[i], 0, 1, 0);
		renderPasserby();
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(myPromoter->Coordinates[0].x, myPromoter->Coordinates[0].y, myPromoter->Coordinates[0].z);
	modelStack.Rotate(180, 0, 1, 0);
	renderPromoter();
	modelStack.PopMatrix();

	for ( int i = 0; i < myVehicle->AICurrent; i++ )
	{
		modelStack.PushMatrix();
		modelStack.Translate(myVehicle->vehicleCoordinates[0].x, myVehicle->vehicleCoordinates[0].y, myVehicle->vehicleCoordinates[0].z);
		modelStack.Rotate(myVehicle->vehicleRotateY[i], 0, 1, 0);
		renderVehicle();
		modelStack.PopMatrix();
	}

	for (int i = 0; i < myVehicle->Coordinates.size(); i++ )
	{
		modelStack.PushMatrix();
		modelStack.Translate(myVehicle->Coordinates[0].x, myVehicle->Coordinates[0].y, myVehicle->Coordinates[0].z);
		modelStack.Rotate(myVehicle->rotateY[i], 0, 1, 0);
		if ( myVehicle->renderOwner[i] == RenderOwner )
			renderOwner();
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(myGuard->Coordinates[0].x, myGuard->Coordinates[0].y, myGuard->Coordinates[0].z);
	modelStack.Rotate(myGuard->rotateY[0], 0, 1, 0);
	renderSecurityGuard();
	modelStack.PopMatrix();

	//render glass door
	modelStack.PushMatrix();
	modelStack.Translate(2235, -47, door1Pos);
	modelStack.Scale(100, 50, 50.5);
	RenderMesh(meshList[GEO_GLASSDOOR], false, false);
	modelStack.PopMatrix();

	//render glass door
	modelStack.PushMatrix();
	modelStack.Translate(2235, -47, door2Pos);
	modelStack.Scale(100, 50, 50.5);
	RenderMesh(meshList[GEO_GLASSDOOR], false, false);
	modelStack.PopMatrix();

	RenderTextOnScreen(meshList[GEO_TEXT],"FPS=" + textPS, Color(0, 1, 1), 2.5, 0, 23);
	RenderTextOnScreen(meshList[GEO_TEXT],"Direction=" + currView, Color(0, 0, 1), 2.5, 12.7, 23);

	//renderUI
	renderUI();
	}
}

void StudioProject2::renderBounds()
{
	modelStack.PushMatrix();
	modelStack.Translate(testPos.x,testPos.y,testPos.z);
	RenderMesh(meshList[GEO_BOUNDHELPER], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0,0,1850);
	RenderMesh(meshList[GEO_MARKETWALL1], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0,0,-1850);
	RenderMesh(meshList[GEO_MARKETWALL2], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2226,0,0);
	RenderMesh(meshList[GEO_MARKETWALL3], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2226,0,0);
	RenderMesh(meshList[GEO_MARKETWALL4], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2085,-103.3,-1085);
	RenderMesh(meshList[GEO_SHELFBOUNDS1], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1210,-103.3,-820);
	RenderMesh(meshList[GEO_SHELFBOUNDS2], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-310,-103.3,-820);
	RenderMesh(meshList[GEO_SHELFBOUNDS3], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-498,-103.3,-1709);
	RenderMesh(meshList[GEO_SHELFBOUNDS4], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(600,-103.3,-690);
	RenderMesh(meshList[GEO_SHELFBOUNDS5], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x,camera.position.y,camera.position.z);
	modelStack.Rotate(0,0,1,0);
	RenderMesh(meshList[GEO_PLAYERBOUNDS],false,false);

	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(2230,-53,1190);
	RenderMesh(meshList[GEO_DOORBOUNDS], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1374.4,-110,-1271);
	RenderMesh(meshList[GEO_FREEZERBOUNDS], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2083,-150.4,-1217.5);
	RenderMesh(meshList[GEO_CHILLERBOUNDS], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(820, -125, 302);
	RenderMesh(meshList[GEO_GATEBOUNDS1], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1490, -125, 238);
	RenderMesh(meshList[GEO_GATEBOUNDS2], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(463.8, -125, 385.3);
	RenderMesh(meshList[GEO_CASHIERRIGHTBOUNDS], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-367.5, -125, 385.3);
	RenderMesh(meshList[GEO_MIDDLECASHIERBOUNDS], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1148.2, -125, 385.3);
	RenderMesh(meshList[GEO_LEFTCASHIERBOUNDS], false, false);
	modelStack.PopMatrix();
}

void StudioProject2::renderPlayer()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, -275, 0);
	modelStack.Scale(20, 20, 20);

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_PLAYERHEAD], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_PLAYERBODY], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_PLAYERLEFTARM], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_PLAYERRIGHTARM], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_PLAYERLEFTLEG], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_PLAYERRIGHTLEG], false, false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();	
}

void StudioProject2::renderCustomer()
{
	modelStack.PushMatrix();
	modelStack.Scale(20, 20, 20);

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_CUSTOMERHEAD], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_CUSTOMERBODY], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_CUSTOMERLEFTARM], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_CUSTOMERRIGHTARM], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_CUSTOMERLEFTLEG], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_CUSTOMERRIGHTLEG], false, false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();	
}

void StudioProject2::renderPasserby()
{
	modelStack.PushMatrix();
	modelStack.Scale(20, 20, 20);

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_PASSERBYHEAD], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_PASSERBYBODY], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_PASSERBYLEFTARM], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_PASSERBYRIGHTARM], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_PASSERBYLEFTLEG], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_PASSERBYRIGHTLEG], false, false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();	
}

void StudioProject2::renderVehicle()
{
	modelStack.PushMatrix();
	modelStack.Scale(50, 50, 50);

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_CAR], false, false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void StudioProject2::renderPromoter()
{
	modelStack.PushMatrix();
	modelStack.Scale(20, 20, 20);

	modelStack.PushMatrix();
	modelStack.Rotate(myPromoter->rotateY[0], 0, 1, 0);
	RenderMesh(meshList[GEO_PROMOTERHEAD], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_PROMOTERBODY], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_PROMOTERLEFTARM], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_PROMOTERRIGHTARM], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_PROMOTERLEFTLEG], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_PROMOTERRIGHTLEG], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(15, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(0.3, 0.3, 0.3);
	RenderMesh(meshList[GEO_PROMOTERTABLE], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(15, 10, 0);
	modelStack.Rotate(myPromoter->itemRotateY, 0, 1, 0);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_CACTUSJUICE], false, false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();	
}

void StudioProject2::renderCashier()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Scale(20, 20, 20);

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_CASHIERHEAD], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_CASHIERBODY], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_CASHIERLEFTARM], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_CASHIERRIGHTARM], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_CASHIERLEFTLEG], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_CASHIERRIGHTLEG], false, false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();	
}

void StudioProject2::renderOwner()
{
	modelStack.PushMatrix();
	modelStack.Scale(20, 20, 20);

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_OWNERHEAD], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_OWNERBODY], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_OWNERLEFTARM], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_OWNERRIGHTARM], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_OWNERLEFTLEG], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_OWNERRIGHTLEG], false, false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();	
}

void StudioProject2::renderSecurityGuard()
{

	modelStack.PushMatrix();
	modelStack.Scale(20, 20, 20);

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_SGHEAD], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_SGBODY], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_SGLEFTARM], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_SGRIGHTARM], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	if ( myGuard->State[0] == Sitting )
	{
		modelStack.Translate(5, 4.5, -0.7);
		modelStack.Rotate(180, 0, 1, 0);
		modelStack.Rotate(-90, 0, 0, 1);
		modelStack.Rotate(-10, 1, 0, 0);
	}
	RenderMesh(meshList[GEO_SGLEFTLEG], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	if ( myGuard->State[0] == Sitting )
	{
		modelStack.Translate(5, 4.5, 0.7);
		modelStack.Rotate(180, 0, 1, 0);
		modelStack.Rotate(-90, 0, 0, 1);
		modelStack.Rotate(10, 1, 0, 0);
	}
	RenderMesh(meshList[GEO_SGRIGHTLEG], false, false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void StudioProject2::renderSupermarket()
{
	//Office/Cstomer service
	modelStack.PushMatrix();
	modelStack.Translate(-1980, -275, 400);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_CUSTOMERSERVICE], false, false);
	modelStack.PopMatrix();

	//Cashiers
	modelStack.PushMatrix();
	modelStack.Translate(-1100, -275, 400);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_CASHIERTABLE2], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-600, -275, 400);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_CASHIERTABLE], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-150, -275, 400);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_CASHIERTABLE2], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(400, -275, 400);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_CASHIERTABLE], false, false);
	modelStack.PopMatrix();

	//Security Post
	modelStack.PushMatrix();
	modelStack.Translate(1100, -275, 400);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SECURITYCOUNTER], false, false);
	modelStack.PopMatrix();

	//Shelf on left wall
	modelStack.PushMatrix();
	modelStack.Translate(-2112.7, -275, -1579.5);
	modelStack.Rotate(0, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SHELF], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2112.7, -275, -1086.4);
	modelStack.Rotate(0, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SHELF], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2112.7, -275, -593.5);
	modelStack.Rotate(0, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SHELF], false, false);
	modelStack.PopMatrix();

	//Shelf on back wall
	modelStack.PushMatrix();
	modelStack.Translate(-3.8, -275, -1736.3);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SHELF], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-496.9, -275, -1736.3);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SHELF], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-990, -275, -1736.3);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SHELF], false, false);
	modelStack.PopMatrix();

	//Pair of shelves near left wall
	modelStack.PushMatrix();
	modelStack.Translate(-1300, -275, -820);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SHELF], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1120, -275, -820);
	modelStack.Rotate(0, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SHELF], false, false);
	modelStack.PopMatrix();

	//Pair of shelves in center
	modelStack.PushMatrix();
	modelStack.Translate(-400, -275, -820);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SHELF], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-220, -275, -820);
	modelStack.Rotate(0, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SHELF], false, false);
	modelStack.PopMatrix();

	//Lone shelf near right-center
	modelStack.PushMatrix();
	modelStack.Translate(600, -275, -663.9);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SHELF], false, false);
	modelStack.PopMatrix();

	//Chiller at the right wall
	modelStack.PushMatrix();
	modelStack.Translate(2084, -280, -1600);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_CHILLER], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2084, -280, -1200);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_CHILLER], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2084, -280, -800);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_CHILLER], false, false);
	modelStack.PopMatrix();

	//Freezer at the right side
	modelStack.PushMatrix();
	modelStack.Translate(1400, -278.2, -1550);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_FREEZER], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1400, -278.2, -1000);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_FREEZER], false, false);
	modelStack.PopMatrix();

	//render right gate1
	modelStack.PushMatrix();
	modelStack.Translate(960, -325, 230);
	modelStack.Rotate(gate1RightRotate, 0, 1, 0);
	modelStack.Scale(70, 70, 70);
	RenderMesh(meshList[GEO_RIGHTGATE], false, false);
	modelStack.PopMatrix();

	//render left gate1
	modelStack.PushMatrix();
	modelStack.Translate(680, -325, 230);
	modelStack.Rotate(gate1LeftRotate, 0, 1, 0);
	modelStack.Scale(70, 70, 70);
	RenderMesh(meshList[GEO_LEFTGATE], false, false);
	modelStack.PopMatrix();

	//render right gate2
	modelStack.PushMatrix();
	modelStack.Translate(-1630, -325, 320);
	modelStack.Rotate(gate2RightRotate, 0, 1, 0);
	modelStack.Scale(70, 70, 70);
	RenderMesh(meshList[GEO_RIGHTGATE], false, false);
	modelStack.PopMatrix();

	//render left gate2
	modelStack.PushMatrix();
	modelStack.Translate(-1350, -325, 320);
	modelStack.Rotate(gate2LeftRotate, 0, 1, 0);
	modelStack.Scale(70, 70, 70);
	RenderMesh(meshList[GEO_LEFTGATE], false, false);
	modelStack.PopMatrix();
}

void StudioProject2::renderItems()
{
	for(int i = 0; i < itemVector.size(); i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(itemVector[i]->position.x, itemVector[i]->position.y, itemVector[i]->position.z);
		modelStack.Rotate(itemVector[i]->rotateY, 0, 1, 0);
		modelStack.Scale(itemVector[i]->scale.x,itemVector[i]->scale.y,itemVector[i]->scale.z);
		RenderMesh(meshList[itemVector[i]->mesh], false, false);
		modelStack.PopMatrix();
	}

	for(float k = 27;k >= -250;k = k-50)
	{
		for(float i = -2040;i > -2160;i = i - 40)
		{
			modelStack.PushMatrix();
			modelStack.Translate(i, k, -750);
			modelStack.Scale(80,80,80);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CACTUSJUICE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -684);
			modelStack.Scale(80,80,80);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CACTUSJUICE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -618);
			modelStack.Scale(80,80,80);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CACTUSJUICE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -552);
			modelStack.Scale(80,80,80);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CACTUSJUICE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -486);
			modelStack.Scale(80,80,80);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CACTUSJUICE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -420);
			modelStack.Scale(80,80,80);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CACTUSJUICE], false, false);
			modelStack.PopMatrix();
		}
	}

	//Cadbury
	for (float k = 30; k >-240; k = k-50)
	{
		for (float i = -2080; i>-2180; i = i- 50)
		{
			modelStack.PushMatrix();
			modelStack.Translate(i, k, -920);
			modelStack.Scale(80,80,80);
			modelStack.Rotate(-90,0,1,0);
			RenderMesh(meshList[GEO_CADBURY], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -990);
			modelStack.Scale(80,80,80);
			modelStack.Rotate(-90,0,1,0);
			RenderMesh(meshList[GEO_CADBURY], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1060);
			modelStack.Scale(80,80,80);
			modelStack.Rotate(-90,0,1,0);
			RenderMesh(meshList[GEO_CADBURY], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1130);
			modelStack.Scale(80,80,80);
			modelStack.Rotate(-90,0,1,0);
			RenderMesh(meshList[GEO_CADBURY], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1200);
			modelStack.Scale(80,80,80);
			modelStack.Rotate(-90,0,1,0);
			RenderMesh(meshList[GEO_CADBURY], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1270);
			modelStack.Scale(80,80,80);
			modelStack.Rotate(-90,0,1,0);
			RenderMesh(meshList[GEO_CADBURY], false, false);
			modelStack.PopMatrix();
		}
	}

	//Can Vege
	for(float k = 31; k > -268; k = k - 50)
	{
		for(float i = -2045; i > -2165; i = i - 40)
		{
			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1430);
			modelStack.Scale(60,60,60);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CANVEGE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1496);
			modelStack.Scale(60,60,60);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CANVEGE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1562);
			modelStack.Scale(60,60,60);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CANVEGE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1628);
			modelStack.Scale(60,60,60);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CANVEGE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1694);
			modelStack.Scale(60,60,60);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CANVEGE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1760);
			modelStack.Scale(60,60,60);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CANVEGE], false, false);
			modelStack.PopMatrix();
		}
	}

	//Catfood
	for(float k = 27; k > -273; k = k - 50)
	{
		for(float i = -1365; i < -1265; i = i + 50)
		{
			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1000);
			modelStack.Scale(15,13,15);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CATFOOD], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -925);
			modelStack.Scale(15,13,15);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CATFOOD], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -850);
			modelStack.Scale(15,13,15);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CATFOOD], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -775);
			modelStack.Scale(15,13,15);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CATFOOD], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -700);
			modelStack.Scale(15,13,15);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CATFOOD], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -625);
			modelStack.Scale(15,13,15);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CATFOOD], false, false);
			modelStack.PopMatrix();
		}
	}

	//Cereal Box

	for( float k = 27; k > -273; k = k - 50)
	{
		for(float i = -1035; i > -1170; i = i - 35)
		{
			modelStack.PushMatrix();
			modelStack.Translate(i, k, -650);
			modelStack.Scale(30,30,30);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CEREAL], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -720);
			modelStack.Scale(30,30,30);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CEREAL], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -790);
			modelStack.Scale(30,30,30);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CEREAL], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -860);
			modelStack.Scale(30,30,30);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CEREAL], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -930);
			modelStack.Scale(30,30,30);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CEREAL], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1000);
			modelStack.Scale(30,30,30);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CEREAL], false, false);
			modelStack.PopMatrix();
		}
	}

	//Detergent
	for (float k = 25; k > - 275; k = k -50)
	{
		for(float i = -1690; i > -1790;i = i - 50)
		{
			modelStack.PushMatrix();
			modelStack.Translate(-1200, k, i);
			modelStack.Scale(15,15,15);
			RenderMesh(meshList[GEO_DETERGENT], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-1120, k, i);
			modelStack.Scale(15,15,15);
			RenderMesh(meshList[GEO_DETERGENT], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-1040, k, i);
			modelStack.Scale(15,15,15);
			RenderMesh(meshList[GEO_DETERGENT], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-960, k, i);
			modelStack.Scale(15,15,15);
			RenderMesh(meshList[GEO_DETERGENT], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-880, k, i);
			modelStack.Scale(15,15,15);
			RenderMesh(meshList[GEO_DETERGENT], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-800, k, i);
			modelStack.Scale(15,15,15);
			RenderMesh(meshList[GEO_DETERGENT], false, false);
			modelStack.PopMatrix();
		}
	}

	//Fererro
	for(float k = 28;k > -270;k = k -50)
	{
		for(float i = -460;i < -360;i = i + 50)
		{
			modelStack.PushMatrix();
			modelStack.Translate(i, k, -660);
			modelStack.Scale(30,30,30);
			modelStack.Rotate(-90,0,1,0);
			RenderMesh(meshList[GEO_FERERRO], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -726);
			modelStack.Scale(30,30,30);
			modelStack.Rotate(-90,0,1,0);
			RenderMesh(meshList[GEO_FERERRO], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -792);
			modelStack.Scale(30,30,30);
			modelStack.Rotate(-90,0,1,0);
			RenderMesh(meshList[GEO_FERERRO], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -858);
			modelStack.Scale(30,30,30);
			modelStack.Rotate(-90,0,1,0);
			RenderMesh(meshList[GEO_FERERRO], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -924);
			modelStack.Scale(30,30,30);
			modelStack.Rotate(-90,0,1,0);
			RenderMesh(meshList[GEO_FERERRO], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -990);
			modelStack.Scale(30,30,30);
			modelStack.Rotate(-90,0,1,0);
			RenderMesh(meshList[GEO_FERERRO], false, false);
			modelStack.PopMatrix();

		}
	}

	//Mac & Chz
	for(float k = 25; k > -275; k = k -50)
	{
		for(float i = -1700; i > -1800;i = i - 50)
		{
			modelStack.PushMatrix();
			modelStack.Translate(-700, k, i);
			modelStack.Scale(30,30,30);
			RenderMesh(meshList[GEO_MACCHZ], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-620, k, i);
			modelStack.Scale(30,30,30);
			RenderMesh(meshList[GEO_MACCHZ], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-540, k, i);
			modelStack.Scale(30,30,30);
			RenderMesh(meshList[GEO_MACCHZ], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-460, k, i);
			modelStack.Scale(30,30,30);
			RenderMesh(meshList[GEO_MACCHZ], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-380, k, i);
			modelStack.Scale(30,30,30);
			RenderMesh(meshList[GEO_MACCHZ], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-300, k, i);
			modelStack.Scale(30,30,30);
			RenderMesh(meshList[GEO_MACCHZ], false, false);
			modelStack.PopMatrix();
		}
	}

	//Milo Can
	for(float k = 25; k > -275;k = k -50)
	{
		for(float i = -1700; i > -1800; i = i - 50)
		{
			modelStack.PushMatrix();
			modelStack.Translate(-150, k, i);
			modelStack.Scale(40,40,40);
			modelStack.Rotate(-45,0,1,0);
			RenderMesh(meshList[GEO_MILOCAN], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-84, k, i);
			modelStack.Scale(40,40,40);
			modelStack.Rotate(-45,0,1,0);
			RenderMesh(meshList[GEO_MILOCAN], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-18, k, i);
			modelStack.Scale(40,40,40);
			modelStack.Rotate(-45,0,1,0);
			RenderMesh(meshList[GEO_MILOCAN], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(48, k, i);
			modelStack.Scale(40,40,40);
			modelStack.Rotate(-45,0,1,0);
			RenderMesh(meshList[GEO_MILOCAN], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(114, k, i);
			modelStack.Scale(40,40,40);
			modelStack.Rotate(-45,0,1,0);
			RenderMesh(meshList[GEO_MILOCAN], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(180, k, i);
			modelStack.Scale(40,40,40);
			modelStack.Rotate(-45,0,1,0);
			RenderMesh(meshList[GEO_MILOCAN], false, false);
			modelStack.PopMatrix();
		}
	}

	//Potato Chips
	for(float k = 28;k > -270;k = k -50)
	{
		for(float i = -130; i > -250; i = i -30)
		{
			modelStack.PushMatrix();
			modelStack.Translate(i, k, -660);
			modelStack.Scale(35,35,35);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_POTATOCHIPS], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -726);
			modelStack.Scale(35,35,35);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_POTATOCHIPS], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -792);
			modelStack.Scale(35,35,35);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_POTATOCHIPS], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -858);
			modelStack.Scale(35,35,35);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_POTATOCHIPS], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -924);
			modelStack.Scale(35,35,35);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_POTATOCHIPS], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -990);
			modelStack.Scale(35,35,35);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_POTATOCHIPS], false, false);
			modelStack.PopMatrix();
		}
	}

	//Sardines
	for(float k = 26;k > -274;k = k -50)
	{
		for(float i = -740; i < -620;i = i +30)
		{
			modelStack.PushMatrix();
			modelStack.Translate(750, k, i);
			modelStack.Scale(45,45,45);
			RenderMesh(meshList[GEO_SARDINES], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(685, k, i);
			modelStack.Scale(45,45,45);
			RenderMesh(meshList[GEO_SARDINES], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(620, k, i);
			modelStack.Scale(45,45,45);
			RenderMesh(meshList[GEO_SARDINES], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(555, k, i);
			modelStack.Scale(45,45,45);
			RenderMesh(meshList[GEO_SARDINES], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(490, k, i);
			modelStack.Scale(45,45,45);
			RenderMesh(meshList[GEO_SARDINES], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(425, k, i);
			modelStack.Scale(45,45,45);
			RenderMesh(meshList[GEO_SARDINES], false, false);
			modelStack.PopMatrix();
		}
	}
}

void StudioProject2::renderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 1851);
	modelStack.Scale(50, 50, 50);

	RenderMesh(meshList[GEO_FRONT], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -1851);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_BACK], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2227, 0, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_LEFT], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2227, 0, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_RIGHT], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -275, 0);
	modelStack.Scale(1.8, 1, 1.5);
	modelStack.Rotate(-90,1,0,0);
	RenderMesh(meshList[GEO_BOTTOM], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-35, 300, -20);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_TOP], false, false);
	modelStack.PopMatrix();

	///////////////////////////////////////////////////////////////////////////////////

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 14970);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(30000, 30000, 1);
	RenderMesh(meshList[GEO_OUTSIDEFRONT], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-14970, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(30000, 30000, 1);
	RenderMesh(meshList[GEO_OUTSIDERIGHT], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -14970);
	modelStack.Scale(30000, 30000, 1);
	RenderMesh(meshList[GEO_OUTSIDEBACK], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(14970, 0, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(30000, 30000, 1);
	RenderMesh(meshList[GEO_OUTSIDELEFT], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 14970, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(30000, 30000, 1);
	RenderMesh(meshList[GEO_OUTSIDETOP], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -14970, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(30000, 30000, 1);
	RenderMesh(meshList[GEO_OUTSIDEBOTTOM], false, false);
	modelStack.PopMatrix();
}

void StudioProject2::renderOutside()
{
	modelStack.PushMatrix();
	modelStack.Translate(3500, -278, 1250);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(2500, 2500, 1);
	RenderMesh(meshList[GEO_CARPARK], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(3500, -278, -1250);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(2500, 2500, 1);
	RenderMesh(meshList[GEO_CARPARK], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-50, -278, -1150);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(4600, 2700, 1);
	RenderMesh(meshList[GEO_CARPARK], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-50, -278, 1150);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(4600, 2700, 1);
	RenderMesh(meshList[GEO_CARPARK], false, false);
	modelStack.PopMatrix();


	for(float a=3710; a > -3710; a-=2076)
	{
		modelStack.PushMatrix();
		modelStack.Translate(a, -277, 2550);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(50, 50, 50);
		RenderMesh(meshList[GEO_GREYBRICK], false, false);
		modelStack.PopMatrix();
	}

	for(float a=3710; a > -3710; a-=2076)
	{
		modelStack.PushMatrix();
		modelStack.Translate(a, -277, -2550);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(50, 50, 50);
		RenderMesh(meshList[GEO_GREYBRICK], false, false);
		modelStack.PopMatrix();
	}

	for(float a=-2550; a < 3710; a+=5100)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-2300, -277, a);
		modelStack.Scale(50, 50, 50);
		RenderMesh(meshList[GEO_GREYBRICK], false, false);
		modelStack.PopMatrix();
	}

	for(float a=-2500; a < 3710; a+=5000)
	{
		modelStack.PushMatrix();
		modelStack.Translate(4800, -277, a);
		modelStack.Scale(50, 50, 35);
		RenderMesh(meshList[GEO_GREYBRICK], false, false);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(4800, -277, 0);
	modelStack.Scale(50, 50, 35);
	RenderMesh(meshList[GEO_GREYBRICK], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(12250, -280, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(15000, 15000, 1);
	RenderMesh(meshList[GEO_GRASS], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(5495, -276, 1250);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(1500, 1050, 1);
	RenderMesh(meshList[GEO_ROAD], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(5495, -276, -1250);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(1500, 1050, 1);
	RenderMesh(meshList[GEO_ROAD], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(7240, -277, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(2000, 15000, 1);
	RenderMesh(meshList[GEO_ROAD2], false, false);
	modelStack.PopMatrix();

	for(float a = -7000; a < 7000; a+=1400)
	{
		modelStack.PushMatrix();
		modelStack.Translate(7240, -275, a);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(200, 700, 1);
		RenderMesh(meshList[GEO_WHITE], false, false);
		modelStack.PopMatrix();
	}

	//modelStack.PushMatrix();
	////modelStack.Translate(7240, -275, a);
	////modelStack.Rotate(-90, 1, 0, 0);
	//modelStack.Scale(50, 50, 50);
	//RenderMesh(meshList[GEO_CAR], false, false);
	//modelStack.PopMatrix();
}

void StudioProject2::renderUI()
{
	RenderQuadOnScreen(meshList[GEO_SIDEBAR], 30, 27, 1.33, 0.11);

	if(inhand->holding.size() == 1)
	{
		RenderQuadOnScreen(meshList[itemVector[inhand->holding[0]]->uimesh], 4.8, 4.7, 5.8, 0.62);
	}
	if(inhand->holding.size() == 2)
	{
		RenderQuadOnScreen(meshList[itemVector[inhand->holding[1]]->uimesh], 4.8, 4.7, 5.8, 0.62);
		RenderQuadOnScreen(meshList[itemVector[inhand->holding[0]]->uimesh], 4.8, 4.7, 7.07, 0.62);
	}
	if(inhand->holding.size() == 3)
	{
		RenderQuadOnScreen(meshList[itemVector[inhand->holding[2]]->uimesh], 4.8, 4.7, 5.8, 0.62);
		RenderQuadOnScreen(meshList[itemVector[inhand->holding[1]]->uimesh], 4.8, 4.7, 7.07, 0.62);
		RenderQuadOnScreen(meshList[itemVector[inhand->holding[0]]->uimesh], 4.8, 4.7, 8.32, 0.62);
	}
	if(inhand->holding.size() == 4)
	{
		RenderQuadOnScreen(meshList[itemVector[inhand->holding[3]]->uimesh], 4.8, 4.7, 5.8, 0.62);
		RenderQuadOnScreen(meshList[itemVector[inhand->holding[2]]->uimesh], 4.8, 4.7, 7.07, 0.62);
		RenderQuadOnScreen(meshList[itemVector[inhand->holding[1]]->uimesh], 4.8, 4.7, 8.32, 0.62);
		RenderQuadOnScreen(meshList[itemVector[inhand->holding[0]]->uimesh], 4.8, 4.7, 9.56, 0.62);
	}
	if(inhand->holding.size() == 5)
	{
		RenderQuadOnScreen(meshList[itemVector[inhand->holding[4]]->uimesh], 4.8, 4.7, 5.8, 0.62);
		RenderQuadOnScreen(meshList[itemVector[inhand->holding[3]]->uimesh], 4.8, 4.7, 7.07, 0.62);
		RenderQuadOnScreen(meshList[itemVector[inhand->holding[2]]->uimesh], 4.8, 4.7, 8.32, 0.62);
		RenderQuadOnScreen(meshList[itemVector[inhand->holding[1]]->uimesh], 4.8, 4.7, 9.56, 0.62);
		RenderQuadOnScreen(meshList[itemVector[inhand->holding[0]]->uimesh], 4.8, 4.7, 10.825, 0.62);
	}
	if(pickUpText == true)
	{
		RenderQuadOnScreen(meshList[GEO_TEXTBOX], 40, 6, 1, 2.5);
		RenderTextOnScreen(meshList[GEO_TEXT],"Press 'B' to pick up item", Color(1, 0, 0), 2.5, 9.75, 5.5);
	}
	if(putBackText == true)
	{
		RenderQuadOnScreen(meshList[GEO_TEXTBOX], 40, 6, 1, 2.5);
		RenderTextOnScreen(meshList[GEO_TEXT],"Press 'N' to put down item", Color(1, 0, 0), 2.5, 9.5, 5.5);
	}

	RenderQuadOnScreen(meshList[GEO_SHOPPINGLIST], 45, 45, listlocation, 0.7);
	if(showList == true)
	{
		for(int i = 0; i < shoppingList.size();++i)
		{	
			RenderTextOnScreen(meshList[GEO_TEXT],shoppingList[i], Color(0, 0, 0), 2.5, 21, 11.5+i);
		}
	}
}

void StudioProject2::MainMenu()
{
	//bool Run = true;
	RenderQuadOnScreen(meshList[GEO_MENU], 80, 80, 0.5, 0.4);
	RenderTextOnScreen(meshList[GEO_TEXT],"Main Menu",Color(0,0,1),10,1.8,3.7);
	RenderTextOnScreen(meshList[GEO_TEXT],"Start",Color(0,0,1),10,2.8,2.7);
	RenderTextOnScreen(meshList[GEO_TEXT],"End",Color(0,0,1),10,2.8,1.7);
	RenderTextOnScreen(meshList[GEO_TEXT],"->",Color(1,0,0),10,1.8,menuX);

	if(Application::IsKeyPressed(GLFW_KEY_UP))
	{
		menuX = 2.8f;
	}
	if(Application::IsKeyPressed(GLFW_KEY_DOWN))
	{
		menuX = 1.8f;
	}
	if(Application::IsKeyPressed(GLFW_KEY_ENTER) && menuX == 2.8f)
	{
		stoptime = false;
	}
	if(Application::IsKeyPressed(GLFW_KEY_ENTER) && menuX == 1.8f)
	{
		Application::derpClose();
	}
}

void StudioProject2::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.5f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void StudioProject2::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(0.5f + i * 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void StudioProject2::RenderQuadOnScreen(Mesh* mesh, float scalex, float scaley, float x, float y)
{
	if(!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(scalex, scaley, 1);
	modelStack.Translate(x, y, 0);

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	mesh->Render();
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void StudioProject2::CollisionCheck(double dt)
{
	/************
	   TESTING
	*************/
	playerPos = camera.position;

	box[PLAYER]->Max = playerPos + playerBounds;
	box[PLAYER]->Min = playerPos - playerBounds;

	currView = "NONE";

	if(camera.view.x > -0.7 && camera.view.x < 0.7  && camera.view.z < 0)
	{
		currView = "North";
	}
	if(camera.view.x > -0.7 && camera.view.x < 0.7  && camera.view.z > 0)
	{
		currView = "South";
	}
	if(camera.view.z > -0.7 && camera.view.z < 0.7 && camera.view.x < 0)
	{
		currView = "West";
	}
	if(camera.view.z > -0.7 && camera.view.z < 0.7 && camera.view.x > 0)
	{
		currView = "East";
	}

	if(Application::IsKeyPressed('W')) //(playerPos.z < newBox.Min.z))
	{
		for(int i = 0; i < box.size(); i++)
		{
			if(box[i]->isPlayer == false && box[i]->isInteractive == false)
			{
				if( (box[PLAYER]->Min.x < box[i]->Max.x) && (box[PLAYER]->Max.x > box[i]->Min.x) &&
					(box[PLAYER]->Min.y < box[i]->Max.y) && (box[PLAYER]->Max.y > box[i]->Min.y) &&
					(box[PLAYER]->Min.z < box[i]->Max.z) && (box[PLAYER]->Max.z > box[i]->Min.z) )
				{
					if(canPhase)
					{
						canMove = true;
						return;
					}
					canMove = false;
					return;
				}
				else
				{
					canMove = true;
				}
			}
		}
	}

	if(Application::IsKeyPressed('S'))
	{
		for(int i = 0; i < box.size(); i++)
		{
			if(box[i]->isPlayer == false && box[i]->isInteractive == false)
			{
				if( (box[PLAYER]->Min.x < box[i]->Max.x) && (box[PLAYER]->Max.x > box[i]->Min.x) &&
					(box[PLAYER]->Min.y < box[i]->Max.y) && (box[PLAYER]->Max.y > box[i]->Min.y) &&
					(box[PLAYER]->Min.z < box[i]->Max.z) && (box[PLAYER]->Max.z > box[i]->Min.z) )
				{
					if(canPhase)
					{
						canMove = true;
						return;
					}
					canMove = false;
					return;
				}
				else
				{
					canMove = true;
				}
			}
		}
	}
	if(Application::IsKeyPressed('A'))//(playerPos.x < newBox.Min.x))
	{
		for(int i = 0; i < box.size(); i++)
		{
			if(box[i]->isPlayer == false && box[i]->isInteractive == false)
			{
				if( (box[PLAYER]->Min.x < box[i]->Max.x) && (box[PLAYER]->Max.x > box[i]->Min.x) &&
					(box[PLAYER]->Min.y < box[i]->Max.y) && (box[PLAYER]->Max.y > box[i]->Min.y) &&
					(box[PLAYER]->Min.z < box[i]->Max.z) && (box[PLAYER]->Max.z > box[i]->Min.z) )
				{
					if(canPhase)
					{
						canMove = true;
						return;
					}
					canMove = false;
					return;
				}
				else
				{
					canMove = true;
				}
			}
		}
	}

	if(Application::IsKeyPressed('D'))//(playerPos.x > newBox.Max.x))
	{
		for(int i = 0; i < box.size(); i++)
		{
			if(box[i]->isPlayer == false && box[i]->isInteractive == false)
			{
				if( (box[PLAYER]->Min.x < box[i]->Max.x) && (box[PLAYER]->Max.x > box[i]->Min.x) &&
					(box[PLAYER]->Min.y < box[i]->Max.y) && (box[PLAYER]->Max.y > box[i]->Min.y) &&
					(box[PLAYER]->Min.z < box[i]->Max.z) && (box[PLAYER]->Max.z > box[i]->Min.z) )
				{
					if(canPhase)
					{
						canMove = true;
						return;
					}
					canMove = false;
					return;
				}
				else
				{
					canMove = true;
				}
			}
		}
	}

	/******************************************************************************************************************************
	INTERACTION GOES UNDER HERE
	*******************************************************************************************************************************/

	if(camera.position.x > box[DOOR]->Min.x && camera.position.x < box[DOOR]->Max.x && camera.position.y > box[DOOR]->Min.y && camera.position.y < box[DOOR]->Max.y && camera.position.z > box[DOOR]->Min.z && camera.position.z < box[DOOR]->Max.z && door1Pos > 700)
	{
		canPhase = true;
		door1Pos -= 500*dt;
		door2Pos += 500*dt;
	}
	if((camera.position.x < box[DOOR]->Min.x || camera.position.x > box[DOOR]->Max.x || camera.position.y < box[DOOR]->Min.y || camera.position.y > box[DOOR]->Max.y || camera.position.z < box[DOOR]->Min.z || camera.position.z > box[DOOR]->Max.z) && door1Pos < 1023)
	{
		canPhase = false;
		door1Pos += 500*dt;
		door2Pos -= 500*dt;
		if(door1Pos > 1023)
		{
			door1Pos = 1023;
			door2Pos = 1365;
		}
	}

	if(camera.position.x > box[GATE1]->Min.x && camera.position.x < box[GATE1]->Max.x && camera.position.y > box[GATE1]->Min.y && camera.position.y < box[GATE1]->Max.y && camera.position.z > box[GATE1]->Min.z && camera.position.z < box[GATE1]->Max.z && gate1RightRotate > -90)
	{
		gate1RightRotate -= 450*dt;
		gate1LeftRotate += 450*dt;
	}
	if((camera.position.x < box[GATE1]->Min.x || camera.position.x > box[GATE1]->Max.x || camera.position.y < box[GATE1]->Min.y || camera.position.y > box[GATE1]->Max.y || camera.position.z < box[GATE1]->Min.z || camera.position.z > box[GATE1]->Max.z) && gate1RightRotate < 0)
	{
		gate1RightRotate += 450*dt;
		gate1LeftRotate -= 450*dt;
		if(gate1RightRotate > 0)
		{
			gate1RightRotate = 0;
			gate1LeftRotate = 0;
		}
	}

	if(camera.position.x > box[GATE2]->Min.x && camera.position.x < box[GATE2]->Max.x && camera.position.y > box[GATE2]->Min.y && camera.position.y < box[GATE2]->Max.y && camera.position.z > box[GATE2]->Min.z && camera.position.z < box[GATE2]->Max.z && gate2RightRotate > 90)
	{
		gate2RightRotate -= 450*dt;
		gate2LeftRotate += 450*dt;
	}
	if((camera.position.x < box[GATE2]->Min.x || camera.position.x > box[GATE2]->Max.x || camera.position.y < box[GATE2]->Min.y || camera.position.y > box[GATE2]->Max.y || camera.position.z < box[GATE2]->Min.z || camera.position.z > box[GATE2]->Max.z) && gate2RightRotate < 180)
	{
		gate2RightRotate += 450*dt;
		gate2LeftRotate -= 450*dt;
		if(gate2RightRotate > 180)
		{
			gate2RightRotate = 180;
			gate2LeftRotate = -180;
		}
	}

	for(int i = 0; i < shelfVector.size(); ++i)
	{
		if(camera.target.x > shelfVector[i]->boundMin.x && camera.target.x < shelfVector[i]->boundMax.x  && camera.target.y > shelfVector[i]->boundMin.y && camera.target.y < shelfVector[i]->boundMax.y && camera.target.z > shelfVector[i]->boundMin.z && camera.target.z < shelfVector[i]->boundMax.z && shelfVector[i]->isempty == false && inhand->reachMax == false)
		{
			pickUpText = true;
			if((Application::IsButtonPressed(GLFW_MOUSE_BUTTON_LEFT)))
			{
				inhand->recive(shelfVector[i]->itemid);
				shelfVector[i]->isempty = true;		
			}
			break;
		}
		else
		{
			pickUpText = false;
		}
	}
	for(int i = 0; i < shelfVector.size(); ++i)
	{
		if(camera.target.x > shelfVector[i]->boundMin.x && camera.target.x < shelfVector[i]->boundMax.x  && camera.target.y > shelfVector[i]->boundMin.y && camera.target.y < shelfVector[i]->boundMax.y && camera.target.z > shelfVector[i]->boundMin.z && camera.target.z < shelfVector[i]->boundMax.z && shelfVector[i]->isempty == true && inhand->holding.size() > 0)
		{
			putBackText = true;
			if(Application::IsButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
			{
				shelfVector[i]->itemid = inhand->remove();
				itemVector[shelfVector[i]->itemid]->placeItem(shelfVector[i]->position);
				shelfVector[i]->isempty = false;
			}
			break;
		}
		else
		{
			putBackText = false;
		}
	}

	if(inhand->holding.size() > 0)
	{
		for(int i = 0; i < inhand->holding.size(); ++i)
		{
			itemVector[inhand->holding[i]]->takeItem(camera.position - Vector3(0, 1000, 0));
		}
		
		itemVector[inhand->holding.back()]->takeItem(camera.target);
		
		itemVector[inhand->holding.back()]->updateRotate(camera.yaw);
	}

	if(Application::IsKeyPressed('0'))
	{
		bool hello = shopping.check(itemVector, inhand->holding);

		if(hello == true)
		{
			cout << " :D";
		}
	}
	/*********************************************************************************************************************************/
}

void StudioProject2::Exit()
{
	// Cleanup VBO here
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i] != NULL)
		{
			delete meshList[i];
		}
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}


