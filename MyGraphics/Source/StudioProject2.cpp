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

bool removeItems = false;

//Test
double FPS;
std::string textPS;
double toggleDelay = 0.0f;

double deltaTime;

vector<BoundingBox*> box;
BoundingBox * boxPtr;
BOUNDTYPE bound;

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

	carparkSlot[0] = 0;
	carparkSlot[1] = 0; // 0 = empty, 1 = filled
	carparkSlot[2] = 0;
	carparkSlot[3] = 0;

	SGState = 0; //0 = sitting, 1 = chasing player
	SGTranslate = 0;
	SGLegTranslate = 0;

	cashier1RotateY = 0;
	cashier2RotateY = 0;
	cashier3RotateY = 0;
	cashier4RotateY = 0;

	Customers = 1;
	CustomerX.push_back(5000);
	CustomerZ.push_back(7000);
	CustomerRotation.push_back(90);
	CustomerState.push_back(0);
	CustomerItemsHeld.push_back(0);

	Passerby = 1;
	PasserbyX.push_back(5200);
	PasserbyZ.push_back(0);
	PasserbyRotation.push_back(90);

	Vehicles = 1;
	VehicleX.push_back(6000);
	VehicleZ.push_back(6000);
	VehicleRotation.push_back(180);
	VehicleState.push_back(30);

	playerPos.Set(0,0,0);
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
	boxPtr->Max = doorBounds;
	boxPtr->Min = -doorBounds;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->isPlayer = true;
	boxPtr->Max = playerBounds;
	boxPtr->Min = -playerBounds;
	box.push_back(boxPtr);
	/*******************/

	// Init VBO here
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Default VAO
	glGenVertexArrays(1, &m_vertexArrayID); //Generate "x" buffer
	glBindVertexArray(m_vertexArrayID);

	Mtx44 projection;
	projection.SetToPerspective(50.0f,4.0f / 3.0f, 0.1f, 30000.0f);

	projectionStack.LoadMatrix(projection);

	camera.Init(Vector3(1, -30, -230), Vector3(1, -30, -440), Vector3(0, 1, 0));

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

	meshList[GEO_BOUNDHELPER] = MeshBuilder::GenerateSphere("BoundHelper", Color(1,1,1), 10, 18, 5.f);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//ExportedFont.tga");

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
	/**************************************************************************************************************/

	/***************************************************
	FOR SETTING NEW BOUNDS BECAUSE OF TRANSLATION UPDATE
	***************************************************/

	box[DOOR]->Max += Vector3(2230,-53,1190);
	box[DOOR]->Min += Vector3(2230,-53,1190);
	box[SHELF1]->Max += Vector3(-2085,-103.3,-1085);
	box[SHELF1]->Min += Vector3(-2085,-103.3,-1085);
	box[SHELF2]->Max += Vector3(-1210,-103.3,-820);
	box[SHELF2]->Min += Vector3(-1210,-103.3,-820);

	/***************************************************
	FOR ADDING ITEMS & SHELFSLOTS
	***************************************************/
	inhand = new Inhand(5);
	Item* ip;
	Shelfslot* sp;

	for(float y = 50; y >= -250; y -= 50)
	{
		for(float z = -420; z >= -750; z -= 66)
		{
			ip = new Item(Vector3(12,18,12), Vector3(-12,-18,-12), Vector3(-2000, y , z), Vector3(80,80,80), 90.f, GEO_CACTUSJUICE);
			itemVector.push_back(ip);
			sp = new Shelfslot(Vector3(-2000, y , z),Vector3(12,18,12), Vector3(-12,-18,-12), itemVector.size()-1, false);
			shelfVector.push_back(sp);
		}
	}
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

void StudioProject2::Update(double dt)
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

	////CollisionCheck(dt);

	//if(Application::IsKeyPressed('T'))
	//{
	//	//playerPos += playerDir * moveSpeed;
	//	playerPos.z += 1.f;
	//	cout << "Player Pos : " << playerPos << endl;
	//}
	//if(Application::IsKeyPressed('G'))
	//{
	//	//playerPos -= playerDir * moveSpeed;
	//	playerPos.z -= 1.f;
	//	cout << "Player Pos : " << playerPos << endl;
	//}
	//if(Application::IsKeyPressed('F'))
	//{
	//	//rotateAngle += rotateSpeed * dt;
	//	playerPos.x += 1.f;
	//	cout << "Player Pos : " << playerPos << endl;
	//}
	//if(Application::IsKeyPressed('H'))
	//{
	//	//rotateAngle -= rotateSpeed * dt;
	//	playerPos.x -= 1.f;
	//	cout << "Player Pos : " << playerPos << endl;
	//}

	GenerateAIs();

	PasserbyAI();
	CustomerAI();
	VehicleAI();
	PromoterAI();

	////////////for door//////////
	if(camera.position.x > box[DOOR]->Min.x && camera.position.x < box[DOOR]->Max.x  && camera.position.y > box[DOOR]->Min.y && camera.position.y < box[DOOR]->Max.y && camera.position.z > box[DOOR]->Min.z && camera.position.z < box[DOOR]->Max.z && door1Pos > 700)
	{
		door1Pos -= 500*dt;
		door2Pos += 500*dt;
	}
	if((camera.position.x < box[DOOR]->Min.x || camera.position.x > box[DOOR]->Max.x || camera.position.y < box[DOOR]->Min.y || camera.position.y > box[DOOR]->Max.y || camera.position.z < box[DOOR]->Min.z || camera.position.z > box[DOOR]->Max.z) && door1Pos < 1023)
	{
		door1Pos += 500*dt;
		door2Pos -= 500*dt;
		if(door1Pos > 1023)
		{
			door1Pos = 1023;
			door2Pos = 1365;
		}
	}
	//////////////////////////////
	playerDir.x = dt * sin(Math::DegreeToRadian(rotateAngle));
	playerDir.z = dt * cos(Math::DegreeToRadian(rotateAngle));
	playerDir.y = 0.f;

	FPS = 1 / dt;
	std::ostringstream s;
	s << setprecision(9) << FPS;
	textPS = s.str();

	deltaTime = dt;
	toggleDelay += dt;

	camera.Update(dt,canMove);
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
	for(int i = 0; i < shelfVector.size(); ++i)
	{
		if(camera.target.x > shelfVector[i]->boundMin.x && camera.target.x < shelfVector[i]->boundMax.x  && camera.target.y > shelfVector[i]->boundMin.y && camera.target.y < shelfVector[i]->boundMax.y && camera.target.z > shelfVector[i]->boundMin.z && camera.target.z < shelfVector[i]->boundMax.z && shelfVector[i]->isempty == false && inhand->reachMax == false)
		{
			cout << "seen";
			if(Application::IsKeyPressed('B'))
			{
				inhand->recive(shelfVector[i]->itemid);
				shelfVector[i]->isempty = true;
			}
		}
	}
	for(int i = 0; i < shelfVector.size(); ++i)
	{
		if(camera.target.x > shelfVector[i]->boundMin.x && camera.target.x < shelfVector[i]->boundMax.x  && camera.target.y > shelfVector[i]->boundMin.y && camera.target.y < shelfVector[i]->boundMax.y && camera.target.z > shelfVector[i]->boundMin.z && camera.target.z < shelfVector[i]->boundMax.z && shelfVector[i]->isempty == true && inhand->holding.size() > 0)
		{
			cout << "fap";
			if(Application::IsKeyPressed('N'))
			{
				shelfVector[i]->itemid = inhand->remove();
				itemVector[shelfVector[i]->itemid]->placeItem(shelfVector[i]->position);
				shelfVector[i]->isempty = false;
			}
		}
	}
	if(inhand->holding.size() > 0)
	{
		for(int i = 0; i < inhand->holding.size(); ++i)
		{
			itemVector[inhand->holding[i]]->takeItem(camera.position);
		}
		
		itemVector[inhand->holding.back()]->takeItem(camera.target);
		if(Application::IsKeyPressed(VK_LEFT))
		{
			itemVector[inhand->holding.back()]->updateRotate(150.f * dt);
		}
		if(Application::IsKeyPressed(VK_RIGHT))
		{
			itemVector[inhand->holding.back()]->updateRotate(-150.f * dt);
		}
	}
	//camera.target.Set(CustomerX[0], 0, CustomerZ[0]);
	//camera.position.Set(CustomerX[0], 0, CustomerZ[0] + 500);
}

void StudioProject2::GenerateAIs()
{
	if ( Customers < 10 )
	{
		int canSpawn = rand() % 2222;
		if ( canSpawn == 10 )
		{
			Customers++;
			int spawnWhere = rand() % 2;
			if ( spawnWhere == 0 )
			{
				CustomerX.push_back(5100);
				CustomerZ.push_back(7000);
				CustomerRotation.push_back(90);
				CustomerState.push_back(0);
			}
			else
			{
				CustomerX.push_back(5300);
				CustomerZ.push_back(7000);
				CustomerRotation.push_back(90);
				CustomerState.push_back(0);
			}
		}
	}

	if ( Vehicles < 4 && OwnerX.size() == Vehicles)
	{
		int canSpawn = rand() % 1111;
		if ( canSpawn == 10 );
		{
			Vehicles++;
			VehicleX.push_back(6000);
			VehicleZ.push_back(6000);
			VehicleRotation.push_back(180);
			VehicleState.push_back(30);
		}
	}

	if ( Passerby < 10 )
	{
		int canSpawn = rand() % 1111;
		if ( canSpawn == 10 )
		{
			Passerby++;
			int spawnWhere = rand() % 2;
			if ( spawnWhere == 0 )
			{
				PasserbyX.push_back(5000);
				PasserbyZ.push_back(7000);
				PasserbyRotation.push_back(90);
			}
			else
			{
				PasserbyX.push_back(5200);
				PasserbyZ.push_back(7100);
				PasserbyRotation.push_back(90);
			}
		}
	}
}

void StudioProject2::VehicleAI()
{
	for ( int i = 0; i < Vehicles; i++ )
	{
		if ( VehicleState[i] == 0 )
		{
			//to carpark entrance
			OwnerRotation[i] = 90;
			if ( OwnerZ[i] > 1250 )
				OwnerZ[i] -= 10;
			else
				VehicleState[i] = 1;
		}
		else if ( VehicleState[i] == 1 )
		{
			//to within supermarket
			OwnerRotation[i] = 180;
			if ( OwnerX[i] > 850 )
				OwnerX[i] -= 10;
			else
			{
				int goWhere = rand() % 2;
				if ( goWhere == 0 )
					VehicleState[i] = 2;
				else if ( goWhere == 1 )
					VehicleState[i] = 4;
			}
		}
		else if ( VehicleState[i] == 2 )
		{
			//go to basket
			VehicleState[i] = 4;
		}
		else if ( VehicleState[i] == 3 )
		{
			//go back to entrance
			VehicleState[i] = 4;
		}
		else if ( VehicleState[i] == 4 )
		{
			//go through gates
			OwnerRotation[i] = 90;
			if ( OwnerZ[i] > -200 )
				OwnerZ[i] -= 10;
			else
			{
				int goWhere = rand() % 2;
				if ( goWhere == 0 )
					VehicleState[i] = 5;
				else
				{
					goWhere = rand() % 2;
					if ( goWhere == 0 )
						VehicleState[i] = 7;
					else
						VehicleState[i] = 9;
				}
			}
		}
		else if ( VehicleState[i] == 5 )
		{
			//to trolley from gates
			VehicleState[i] = 6;

		}
		else if ( VehicleState[i] == 6 )
		{
			//trolley to gates
			VehicleState[i] = 7;
		}
		else if ( VehicleState[i] == 7 )
		{
			//gates to drinks section
			OwnerRotation[i] = 0;
			if ( OwnerX[i] < 1700 )
				OwnerX[i] += 10;
			else
			{
				OwnerRotation[i] = 90;
				if ( OwnerZ[i] > -1700 )
					OwnerZ[i] -= 10;
				else
					VehicleState[i] = 8;
			}
		}
		else if ( VehicleState[i] == 8 )
		{
			//drinks section to gate
			OwnerRotation[i] = -90;
			if ( OwnerZ[i] < -200 )
				OwnerZ[i] += 10;
			else
			{
				OwnerRotation[i] = 180;
				if ( OwnerX[i] > 1000 )
					OwnerX[i] -= 10;
				else
				{
					VehicleState[i] = 9;
				}
			}
		}
		else if ( VehicleState[i] == 9 )
		{
			//gate to shelf1
			OwnerRotation[i] = 0;
			if ( OwnerX[i] < 1000 && OwnerZ[i] > -1000)
				OwnerX[i] += 10;
			else
			{
				OwnerRotation[i] = 90;
				if ( OwnerZ[i] > -1000 )
					OwnerZ[i] -= 10;
				else 
				{
					OwnerRotation[i] = 180;
					if ( OwnerX[i] > 600 )
						OwnerX[i] -= 10;
					else
					{
						int GoWhere = rand() % 2;
						if ( GoWhere == 0 )
							VehicleState[i] = 10;
						else
							VehicleState[i] = 11;
					}
				}
			}
		}
		else if ( VehicleState[i] == 10 )
		{
			//shelf1 to shelf2
			OwnerRotation[i] = 180;
			if ( OwnerX[i] > 100 )
				OwnerX[i] -= 10;
			else
			{
				OwnerRotation[i] = -90;
				if ( OwnerZ[i] < -800 )
					OwnerZ[i] += 10;
				else
				{
					OwnerRotation[i] = 180;

					VehicleState[i] = 12;
				}
			}
		}
		else if ( VehicleState[i] == 11 )
		{
			//shelf1 to backshelf
			OwnerRotation[i] = 90;
			if ( OwnerZ[i] > -1500 )
				OwnerZ[i] -= 10;
			else
			{
				OwnerRotation[i] = 180;
				if ( OwnerX[i] > -400 )
					OwnerX[i] -= 10;
				else
				{
					int GoWhere = rand() % 2;
					if ( GoWhere == 0 )
						VehicleState[i] = 13;
					else
						VehicleState[i] = 14;
				}
			}
		}
		else if ( VehicleState[i] == 12 )
		{
			//shelf2 to backshelf
			OwnerRotation[i] = 90;
			if ( OwnerZ[i] > -1500 )
				OwnerZ[i] -= 10;
			else
			{
				OwnerRotation[i] = 180;
				if ( OwnerX[i] > -400 )
					OwnerX[i] -= 10;
				else
				{
					int GoWhere = rand() % 2;
					if ( GoWhere == 0 )
						VehicleState[i] = 13;
					else
						VehicleState[i] = 14;
				}
			}
		}
		else if ( VehicleState[i] == 13 )
		{
			//backshelves to left shelf
			OwnerRotation[i] = 180;
			if ( OwnerX[i] > -1850 )
				OwnerX[i] -= 10;
			else
			{
				OwnerRotation[i] = -90;

				VehicleState[i] = 16;
			}
		}
		else if ( VehicleState[i] == 14 )
		{
			//backshelves to shelf4
			OwnerRotation[i] = 180;
			if ( OwnerX[i] > -850 )
				OwnerX[i] -= 10;
			else
			{
				OwnerRotation[i] = -90;
				if ( OwnerZ[i] < -800 )
					OwnerZ[i] += 10;
				else
				{
					OwnerRotation[i] = 180;

					int GoWhere = rand() % 2;
					if ( GoWhere == 0 )
						VehicleState[i] = 15;
					else
						VehicleState[i] = 23;
				}
			}
		}
		else if ( VehicleState[i] == 15 )
		{
			//shelf4 to checkout1
			OwnerRotation[i] = 180;
			if ( OwnerX[i] > -850 )
				OwnerX[i] -= 10;
			else
				VehicleState[i] = 24;
		}
		else if ( VehicleState[i] == 16 )
		{
			//left shelf to front left shelf
			OwnerRotation[i] = -90;
			if ( OwnerZ[i] < -600 )
				OwnerZ[i] += 10;
			else
			{
				VehicleState[i] = 17;
			}
		}
		else if ( VehicleState[i] == 17 )
		{
			//front left shelves to customer service
			OwnerRotation[i] = -90;
			if ( OwnerZ[i] < -200 )
				OwnerZ[i] += 10;
			else
			{
				if ( CustomerItemsHeld[i] == 0 )
				{
					int GoWhere = rand() % 4;

					if ( GoWhere == 0 )
						VehicleState[i] = 18;
					else if ( GoWhere == 1)
						VehicleState[i] = 19;
					else if ( GoWhere == 2 )
						VehicleState[i] = 20;
					else if ( GoWhere == 3 )
						VehicleState[i] = 21;
				}
				else
					VehicleState[i] = 22;			
			}
		}
		else if ( VehicleState[i] == 18 )
		{
			//customerservice to checkout 1
			OwnerRotation[i] = 0;
			if ( OwnerX[i] < -900 )
				OwnerX[i] += 10;
			else
				VehicleState[i] = 24;
		}
		else if ( VehicleState[i] == 19 )
		{
			//customerservice to checkout2
			OwnerRotation[i] = 0;
			if ( OwnerX[i] < -800 )
				OwnerX[i] += 50;
			else
				VehicleState[i] = 24;
		}
		else if ( VehicleState[i] == 20 )
		{
			//customerservice to checkout3
			OwnerRotation[i] = 0;
			if ( OwnerX[i] < 100 )
				OwnerX[i] += 10;
			else
				VehicleState[i] = 24;
		}
		else if ( VehicleState[i] == 21 )
		{
			//customerservice to checkout4
			OwnerRotation[i] = 0;
			if ( OwnerX[i] < 200 )
				OwnerX[i] += 10;
			else
				VehicleState[i] = 24;
		}
		else if ( VehicleState[i] == 22 )
		{
			//customerservice to gates
			OwnerRotation[i] = 0;
			if ( OwnerX[i] < 850 )
				OwnerX[i] += 10;
			else
			{
				VehicleState[i] = 4;
			}
		}
		else if ( VehicleState[i] == 23 )
		{
			//shelf4 to gates
			OwnerRotation[i] = -90;
			if ( OwnerZ[i] < -200 )
				OwnerZ[i] += 10;
			else
			{
				OwnerRotation[i] = 0;
				if ( OwnerX[i] < 850 )
					OwnerX[i] += 10;
				else
				{
					VehicleState[i] = 4;
				}
			}
		}
		else if ( VehicleState[i] == 24 )
		{
			//checkouts to exit
			OwnerRotation[i] = -90;
			if ( OwnerZ[i] < 350 )
				OwnerZ[i] += 10;
			else
			{
				if ( OwnerX[i] == -900 || OwnerX[i] == 100 )
					OwnerRotation[i] = 180;
				else
					OwnerRotation[i] = 0;

				//cashier response checkout code
				checkingOut = 0;
				if ( checkingOut == 0 )
				{
					int cashierRotate = rand() % 2;
					rotateDelay += 0.2;
					if ( cashierRotate == 0 && rotateDelay > 2)
					{
						rotateDelay -= 2;

						if ( OwnerX[i] == -900 )
							cashier1RotateY = 90;
						else if ( OwnerX[i] == -800 )
							cashier2RotateY = -90;
						else if ( OwnerX[i] == 100 )
							cashier3RotateY = 90;
						else
							cashier4RotateY = -90;
					}
					else if ( cashierRotate == 1 && rotateDelay > 2)
					{
						rotateDelay -= 2;

						if ( OwnerX[i] == -900 )
							cashier1RotateY = 0;
						else if ( OwnerX[i] == -800 )
							cashier2RotateY = 0;
						else if ( OwnerX[i] == 100 )
							cashier3RotateY = 0;
						else
							cashier4RotateY = 0;

						int canLeave = rand() % 2;

						if ( canLeave == 1 )
							VehicleState[i] = 25;
					}
				}
			}
		}
		else if ( VehicleState[i] == 25 )
		{
			OwnerRotation[i] = -90;
			if ( OwnerZ[i] < 1200 && OwnerX[i] != 6000)
				OwnerZ[i] += 10;
			else
			{
				if ( ParkingLocation[i] == 1 )
				{	
					OwnerRotation[i] = 0;
					if ( OwnerX[i] < 3000 )
						OwnerX[i] += 10;
					else
					{
						RenderOwner[i] = 0;
						if ( VehicleZ[i] > 1200 )
							VehicleZ[i] -= 10;
						else
						{
							VehicleRotation[i] = 90;
							if ( VehicleX[i] < 6400 )
								VehicleX[i] += 20;
							else
							{
								VehicleRotation[i] = 180;
								if ( VehicleZ[i] > -7000 )
									VehicleZ[i] -= 20;
								else	
								{
									carparkSlot[0] = 0;
									VehicleX.erase(VehicleX.begin() + i );
									VehicleZ.erase(VehicleZ.begin() + i );
									VehicleRotation.erase(VehicleRotation.begin() + i );
									VehicleState.erase(VehicleState.begin() + i );
									ParkingLocation.erase(ParkingLocation.begin() + i);
									OwnerX.erase(OwnerX.begin() + i);
									OwnerZ.erase(OwnerZ.begin() + i);
									OwnerRotation.erase(OwnerRotation.begin() + i);
									OwnerItemsHeld.erase(OwnerItemsHeld.begin() + i);
									RenderOwner.erase(RenderOwner.begin() + i);
									Vehicles--;
								}
							}
						}
					}
				}
				else if ( ParkingLocation[i] == 2 )
				{
					OwnerRotation[i] = 0;
					if ( OwnerX[i] < 3500 )
						OwnerX[i] += 10;
					else
					{
						RenderOwner[i] = 0;
						if ( VehicleZ[i] > 1200 )
							VehicleZ[i] -= 10;
						else
						{
							VehicleRotation[i] = 90;
							if ( VehicleX[i] < 6400 )
								VehicleX[i] += 20;
							else
							{
								VehicleRotation[i] = 180;
								if ( VehicleZ[i] > -7000 )
									VehicleZ[i] -= 20;
								else	
								{
									carparkSlot[1] = 0;
									VehicleX.erase(VehicleX.begin() + i );
									VehicleZ.erase(VehicleZ.begin() + i );
									VehicleRotation.erase(VehicleRotation.begin() + i );
									VehicleState.erase(VehicleState.begin() + i );
									ParkingLocation.erase(ParkingLocation.begin() + i);
									OwnerX.erase(OwnerX.begin() + i);
									OwnerZ.erase(OwnerZ.begin() + i);
									OwnerRotation.erase(OwnerRotation.begin() + i);
									OwnerItemsHeld.erase(OwnerItemsHeld.begin() + i);
									RenderOwner.erase(RenderOwner.begin() + i);
									Vehicles--;
								}
							}
						}
					}
				}
				else if ( ParkingLocation[i] == 3 )
				{
					OwnerRotation[i] = 0;
					if ( OwnerX[i] < 3900 )
						OwnerX[i] += 10;
					else
					{
						RenderOwner[i] = 0;
						if ( VehicleZ[i] > 1200 )
							VehicleZ[i] -= 10;
						else
						{
							VehicleRotation[i] = 90;
							if ( VehicleX[i] < 6400 )
								VehicleX[i] += 20;
							else
							{
								VehicleRotation[i] = 180;
								if ( VehicleZ[i] > -7000 )
									VehicleZ[i] -= 20;
								else	
								{
									carparkSlot[2] = 0;
									VehicleX.erase(VehicleX.begin() + i );
									VehicleZ.erase(VehicleZ.begin() + i );
									VehicleRotation.erase(VehicleRotation.begin() + i );
									VehicleState.erase(VehicleState.begin() + i );
									ParkingLocation.erase(ParkingLocation.begin() + i);
									OwnerX.erase(OwnerX.begin() + i);
									OwnerZ.erase(OwnerZ.begin() + i);
									OwnerRotation.erase(OwnerRotation.begin() + i);
									OwnerItemsHeld.erase(OwnerItemsHeld.begin() + i);
									RenderOwner.erase(RenderOwner.begin() + i);
									Vehicles--;
								}
							}
						}
					}
				}
				else if ( ParkingLocation[i] == 4 )
				{
					OwnerRotation[i] = 0;
					if ( OwnerX[i] < 4300 )
						OwnerX[i] += 10;
					else
					{
						RenderOwner[i] = 0;
						if ( VehicleZ[i] > 1200 )
							VehicleZ[i] -= 10;
						else
						{
							VehicleRotation[i] = 90;
							if ( VehicleX[i] < 6400 )
								VehicleX[i] += 20;
							else
							{
								VehicleRotation[i] = 180;
								if ( VehicleZ[i] > -7000 )
									VehicleZ[i] -= 20;
								else	
								{
									carparkSlot[3] = 0;
									VehicleX.erase(VehicleX.begin() + i );
									VehicleZ.erase(VehicleZ.begin() + i );
									VehicleRotation.erase(VehicleRotation.begin() + i );
									VehicleState.erase(VehicleState.begin() + i );
									ParkingLocation.erase(ParkingLocation.begin() + i);
									OwnerX.erase(OwnerX.begin() + i);
									OwnerZ.erase(OwnerZ.begin() + i);
									OwnerRotation.erase(OwnerRotation.begin() + i);
									OwnerItemsHeld.erase(OwnerItemsHeld.begin() + i);
									RenderOwner.erase(RenderOwner.begin() + i);
									Vehicles--;
								}
							}
						}
					}
				}
			}
		}
		else if ( VehicleState[i] == 30 )
		{
			//to carpark entrance
			VehicleRotation[i] = 180;
			if ( VehicleZ[i] > 1400 )
				VehicleZ[i] -= 20;
			else
				VehicleState[i] = 31;
		}
		else if ( VehicleState[i] == 31 )
		{
			//park the car
			VehicleRotation[i] = -90;
			if ( carparkSlot[0] == 0 )
			{
				if ( VehicleX[i] > 3040 )
					VehicleX[i] -= 20;
				else
				{
					VehicleRotation[i] = 0;
					if ( VehicleZ[i] < 2050 )
						VehicleZ[i] += 10;
					else
					{
						carparkSlot[0] = 1;
						OwnerX.push_back(3000);
						OwnerZ.push_back(1700);
						OwnerRotation.push_back(90);
						OwnerItemsHeld.push_back(0);
						ParkingLocation.push_back(1);
						RenderOwner.push_back(1);
						VehicleState[i] = 0;
					}
				}
			}
			else if ( carparkSlot[1] == 0 )
			{
				if ( VehicleX[i] > 3500 )
					VehicleX[i] -= 20;
				else
				{
					VehicleRotation[i] = 0;
					if ( VehicleZ[i] < 2050 )
						VehicleZ[i] += 10;
					else
					{
						carparkSlot[1] = 1;
						OwnerX.push_back(3500);
						OwnerZ.push_back(1700);
						OwnerRotation.push_back(90);
						OwnerItemsHeld.push_back(0);
						ParkingLocation.push_back(2);
						RenderOwner.push_back(1);
						VehicleState[i] = 0;
					}
				}
			}
			else if ( carparkSlot[2] == 0 )
			{
				if ( VehicleX[i] > 3900 )
					VehicleX[i] -= 20;
				else
				{
					VehicleRotation[i] = 0;
					if ( VehicleZ[i] < 2050 )
						VehicleZ[i] += 10;
					else
					{
						carparkSlot[2] = 1;
						OwnerX.push_back(3900);
						OwnerZ.push_back(1700);
						OwnerRotation.push_back(90);
						OwnerItemsHeld.push_back(0);
						ParkingLocation.push_back(3);
						RenderOwner.push_back(1);
						VehicleState[i] = 0;
					}
				}
			}
			else if ( carparkSlot[3] == 0 )
			{
				if ( VehicleX[i] > 4300 )
					VehicleX[i] -= 20;
				else
				{
					VehicleRotation[i] = 0;
					if ( VehicleZ[i] < 2050 )
						VehicleZ[i] += 10;
					else
					{
						carparkSlot[3] = 1;
						OwnerX.push_back(4300);
						OwnerZ.push_back(1700);
						OwnerRotation.push_back(90);
						OwnerItemsHeld.push_back(0);
						ParkingLocation.push_back(4);
						RenderOwner.push_back(1);
						VehicleState[i] = 0;
					}
				}
			}
		}
	}
}

void StudioProject2::CustomerAI()
{
	for ( int i = 0; i < Customers; i++ )
	{
		if ( CustomerState[i] == 0 )
		{
			//to carpark entrance
			CustomerRotation[i] = 90;
			if ( CustomerZ[i] > 1250 )
				CustomerZ[i] -= 10;
			else
				CustomerState[i] = 1;
		}
		else if ( CustomerState[i] == 1 )
		{
			//to within supermarket
			CustomerRotation[i] = 180;
			if ( CustomerX[i] > 850 )
				CustomerX[i] -= 10;
			else
			{
				int goWhere = rand() % 2;
				if ( goWhere == 0 )
					CustomerState[i] = 2;
				else if ( goWhere == 1 )
					CustomerState[i] = 4;
			}
		}
		else if ( CustomerState[i] == 2 )
		{
			//go to basket
			CustomerState[i] = 4;
		}
		else if ( CustomerState[i] == 3 )
		{
			//go back to entrance
			CustomerState[i] = 4;
		}
		else if ( CustomerState[i] == 4 )
		{
			//go through gates
			CustomerRotation[i] = 90;
			if ( CustomerZ[i] > -200 )
				CustomerZ[i] -= 10;
			else
			{
				int goWhere = rand() % 2;
				if ( goWhere == 0 )
					CustomerState[i] = 5;
				else
				{
					goWhere = rand() % 2;
					if ( goWhere == 0 )
						CustomerState[i] = 7;
					else
						CustomerState[i] = 9;
				}
			}
		}
		else if ( CustomerState[i] == 5 )
		{
			//to trolley from gates
			CustomerState[i] = 6;

		}
		else if ( CustomerState[i] == 6 )
		{
			//trolley to gates
			CustomerState[i] = 7;
		}
		else if ( CustomerState[i] == 7 )
		{
			//gates to drinks section
			CustomerRotation[i] = 0;
			if ( CustomerX[i] < 1700 )
				CustomerX[i] += 10;
			else
			{
				CustomerRotation[i] = 90;
				if ( CustomerZ[i] > -1700 )
					CustomerZ[i] -= 10;
				else
					CustomerState[i] = 8;
			}
		}
		else if ( CustomerState[i] == 8 )
		{
			//drinks section to gate
			CustomerRotation[i] = -90;
			if ( CustomerZ[i] < -200 )
				CustomerZ[i] += 10;
			else
			{
				CustomerRotation[i] = 180;
				if ( CustomerX[i] > 1000 )
					CustomerX[i] -= 10;
				else
				{
					CustomerState[i] = 9;
				}
			}
		}
		else if ( CustomerState[i] == 9 )
		{
			//gate to shelf1
			CustomerRotation[i] = 0;
			if ( CustomerX[i] < 1000 && CustomerZ[i] > -1000)
				CustomerX[i] += 10;
			else
			{
				CustomerRotation[i] = 90;
				if ( CustomerZ[i] > -1000 )
					CustomerZ[i] -= 10;
				else 
				{
					CustomerRotation[i] = 180;
					if ( CustomerX[i] > 600 )
						CustomerX[i] -= 10;
					else
					{
						int GoWhere = rand() % 2;
						if ( GoWhere == 0 )
							CustomerState[i] = 10;
						else
							CustomerState[i] = 11;
					}
				}
			}
		}
		else if ( CustomerState[i] == 10 )
		{
			//shelf1 to shelf2
			CustomerRotation[i] = 180;
			if ( CustomerX[i] > 100 )
				CustomerX[i] -= 10;
			else
			{
				CustomerRotation[i] = -90;
				if ( CustomerZ[i] < -800 )
					CustomerZ[i] += 10;
				else
				{
					CustomerRotation[i] = 180;

					CustomerState[i] = 12;
				}
			}
		}
		else if ( CustomerState[i] == 11 )
		{
			//shelf1 to backshelf
			CustomerRotation[i] = 90;
			if ( CustomerZ[i] > -1500 )
				CustomerZ[i] -= 10;
			else
			{
				CustomerRotation[i] = 180;
				if ( CustomerX[i] > -400 )
					CustomerX[i] -= 10;
				else
				{
					int GoWhere = rand() % 2;
					if ( GoWhere == 0 )
						CustomerState[i] = 13;
					else
						CustomerState[i] = 14;
				}
			}
		}
		else if ( CustomerState[i] == 12 )
		{
			//shelf2 to backshelf
			CustomerRotation[i] = 90;
			if ( CustomerZ[i] > -1500 )
				CustomerZ[i] -= 10;
			else
			{
				CustomerRotation[i] = 180;
				if ( CustomerX[i] > -400 )
					CustomerX[i] -= 10;
				else
				{
					int GoWhere = rand() % 2;
					if ( GoWhere == 0 )
						CustomerState[i] = 13;
					else
						CustomerState[i] = 14;
				}
			}
		}
		else if ( CustomerState[i] == 13 )
		{
			//backshelves to left shelf
			CustomerRotation[i] = 180;
			if ( CustomerX[i] > -1850 )
				CustomerX[i] -= 10;
			else
			{
				CustomerRotation[i] = -90;

				CustomerState[i] = 16;
			}
		}
		else if ( CustomerState[i] == 14 )
		{
			//backshelves to shelf4
			CustomerRotation[i] = 180;
			if ( CustomerX[i] > -850 )
				CustomerX[i] -= 10;
			else
			{
				CustomerRotation[i] = -90;
				if ( CustomerZ[i] < -800 )
					CustomerZ[i] += 10;
				else
				{
					CustomerRotation[i] = 180;

					int GoWhere = rand() % 2;
					if ( GoWhere == 0 )
						CustomerState[i] = 15;
					else
						CustomerState[i] = 23;
				}
			}
		}
		else if ( CustomerState[i] == 15 )
		{
			//shelf4 to checkout1
			CustomerRotation[i] = 180;
			if ( CustomerX[i] > -850 )
				CustomerX[i] -= 10;
			else
				CustomerState[i] = 24;
		}
		else if ( CustomerState[i] == 16 )
		{
			//left shelf to front left shelf
			CustomerRotation[i] = -90;
			if ( CustomerZ[i] < -600 )
				CustomerZ[i] += 10;
			else
			{
				CustomerState[i] = 17;
			}
		}
		else if ( CustomerState[i] == 17 )
		{
			//front left shelves to customer service
			CustomerRotation[i] = -90;
			if ( CustomerZ[i] < -200 )
				CustomerZ[i] += 10;
			else
			{
				if ( CustomerItemsHeld[i] == 0 )
				{
					int GoWhere = rand() % 4;

					if ( GoWhere == 0 )
						CustomerState[i] = 18;
					else if ( GoWhere == 1)
						CustomerState[i] = 19;
					else if ( GoWhere == 2 )
						CustomerState[i] = 20;
					else if ( GoWhere == 3 )
						CustomerState[i] = 21;
				}
				else
					CustomerState[i] = 22;			
			}
		}
		else if ( CustomerState[i] == 18 )
		{
			//customerservice to checkout 1
			CustomerRotation[i] = 0;
			if ( CustomerX[i] < -900 )
				CustomerX[i] += 10;
			else
				CustomerState[i] = 24;
		}
		else if ( CustomerState[i] == 19 )
		{
			//customerservice to checkout2
			CustomerRotation[i] = 0;
			if ( CustomerX[i] < -800 )
				CustomerX[i] += 50;
			else
				CustomerState[i] = 24;
		}
		else if ( CustomerState[i] == 20 )
		{
			//customerservice to checkout3
			CustomerRotation[i] = 0;
			if ( CustomerX[i] < 100 )
				CustomerX[i] += 10;
			else
				CustomerState[i] = 24;
		}
		else if ( CustomerState[i] == 21 )
		{
			//customerservice to checkout4
			CustomerRotation[i] = 0;
			if ( CustomerX[i] < 200 )
				CustomerX[i] += 10;
			else
				CustomerState[i] = 24;
		}
		else if ( CustomerState[i] == 22 )
		{
			//customerservice to gates
			CustomerRotation[i] = 0;
			if ( CustomerX[i] < 850 )
				CustomerX[i] += 10;
			else
			{
				CustomerState[i] = 4;
			}
		}
		else if ( CustomerState[i] == 23 )
		{
			//shelf4 to gates
			CustomerRotation[i] = -90;
			if ( CustomerZ[i] < -200 )
				CustomerZ[i] += 10;
			else
			{
				CustomerRotation[i] = 0;
				if ( CustomerX[i] < 850 )
					CustomerX[i] += 10;
				else
				{
					CustomerState[i] = 4;
				}
			}
		}
		else if ( CustomerState[i] == 24 )
		{
			//checkouts to exit
			CustomerRotation[i] = -90;
			if ( CustomerZ[i] < 350 )
				CustomerZ[i] += 10;
			else
			{
				if ( CustomerX[i] == -900 || CustomerX[i] == 100 )
					CustomerRotation[i] = 180;
				else
					CustomerRotation[i] = 0;

				//cashier response checkout code
				checkingOut = 0;
				if ( checkingOut == 0 )
				{
					int cashierRotate = rand() % 2;
					rotateDelay += 0.14;
					if ( cashierRotate == 0 && rotateDelay > 2)
					{
						rotateDelay -= 2;

						if ( CustomerX[i] == -900 )
							cashier1RotateY = 90;
						else if ( CustomerX[i] == -800 )
							cashier2RotateY = -90;
						else if ( CustomerX[i] == 100 )
							cashier3RotateY = 90;
						else
							cashier4RotateY = -90;
					}
					else if ( cashierRotate == 1 && rotateDelay > 2)
					{
						rotateDelay -= 2;

						if ( CustomerX[i] == -900 )
							cashier1RotateY = 0;
						else if ( CustomerX[i] == -800 )
							cashier2RotateY = 0;
						else if ( CustomerX[i] == 100 )
							cashier3RotateY = 0;
						else
							cashier4RotateY = 0;

						int canLeave = rand() % 2;

						if ( canLeave == 1 )
							CustomerState[i] = 25;
					}
				}
			}
		}
		else if ( CustomerState[i] == 25 )
		{
			CustomerRotation[i] = -90;
			if ( CustomerZ[i] < 1200 && CustomerX[i] != 5000)
				CustomerZ[i] += 10;
			else
			{
				CustomerRotation[i] = 0;
				if ( CustomerX[i] < 5000 )
					CustomerX[i] += 10;
				else
				{
					CustomerRotation[i] = 90;
					if ( CustomerZ[i] > -7000)
						CustomerZ[i] -= 10;
					else
					{
						CustomerZ.erase(CustomerZ.begin() + i);
						CustomerX.erase(CustomerX.begin() + i);
						CustomerState.erase(CustomerState.begin() + i);
						CustomerRotation.erase(CustomerRotation.begin() + i);
						Customers--;
					}
				}
			}
		}
	}
}

void StudioProject2::PasserbyAI()
{
	for ( int i = 0; i < Passerby; i++ )
	{
		if ( PasserbyZ[i] > -7000 )
			PasserbyZ[i] -= 15;
		else
		{
			PasserbyX.erase(PasserbyX.begin() + i);
			PasserbyZ.erase(PasserbyZ.begin() + i);
			PasserbyRotation.erase(PasserbyRotation.begin() + i);
			Passerby--;
		}
	}
}

void StudioProject2::PromoterAI()
{
	if ( itemRotateY < 360 )
		itemRotateY += 50 * 0.2;
	else
		itemRotateY = 0;

	if ( Passerby != 0 )
	{
		if ( PasserbyZ[0] == 0 )
			promoterRotateY = 0;
		else if ( PasserbyZ[0] < 0 )
			promoterRotateY = -45;
		else 
			promoterRotateY = 45;
	}
	else
	{
		if ( playerPos.z > -100 && playerPos.z < 100)
		{
			promoterRotateY = 0;
			//player interact with ai code?
		}
		else if ( playerPos.z < 0 )
			promoterRotateY = -45;
		else
			promoterRotateY = 45;

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

	modelStack.LoadIdentity();

	Position lightPosition_cameraspace = viewStack.Top() * light[0].position;

	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); // Remember, matrix multiplication is the other way around

	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); //update the shader with new MVP

	//RenderMesh(meshList[GEO_AXES], false, false);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	renderBounds();

	//////////////////////////////////////////////////////////////////////////////////

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false, false);
	modelStack.PopMatrix();

	///////////////////////////////////////////////////////////////////////////////////

	modelStack.PushMatrix();
	modelStack.Translate(camera.target.x, camera.target.y, camera.target.z);
	modelStack.Scale(0.2,0.2,0.2);
	RenderMesh(meshList[GEO_LIGHTBALL], false, false);
	modelStack.PopMatrix();
	
	///////////////////////////////////////////////////////////////////////////////////
	renderSkybox();
	renderSupermarket();
	renderOutside();
	renderItems();

	/*modelStack.PushMatrix();
	modelStack.Translate(playerPos.x, playerPos.y, playerPos.z);
	modelStack.Rotate(rotateAngle,0,1,0);
	modelStack.PushMatrix();
	modelStack.Rotate(-90,0,1,0);
	renderPlayer();
	modelStack.PopMatrix();
	modelStack.PopMatrix();*/

	modelStack.PushMatrix();
	modelStack.Translate(-250, -275, 350);
	modelStack.Rotate(cashier3RotateY, 0, 1, 0);
	renderCashier();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-500, -275, 350);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(cashier2RotateY, 0, 1, 0);
	renderCashier();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(500, -275, 350);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(cashier4RotateY, 0, 1, 0);
	renderCashier();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1200, -275, 350);
	modelStack.Rotate(cashier1RotateY, 0, 1, 0);
	renderCashier();
	modelStack.PopMatrix();

	for ( int i = 0; i < Customers; i++ )
	{
		modelStack.PushMatrix();
		modelStack.Translate(CustomerX[i], -275, CustomerZ[i]);
		modelStack.Rotate(CustomerRotation[i], 0, 1, 0);
		renderCustomer();
		modelStack.PopMatrix();
	}

	for ( int i = 0; i < Passerby; i++ )
	{
		modelStack.PushMatrix();
		modelStack.Translate(PasserbyX[i], -275, PasserbyZ[i]);
		modelStack.Rotate(PasserbyRotation[i], 0, 1, 0);
		renderPasserby();
		modelStack.PopMatrix();
	}

	for ( int i = 0; i < Vehicles; i++ )
	{
		modelStack.PushMatrix();
		modelStack.Translate(VehicleX[i], -275, VehicleZ[i]);
		modelStack.Rotate(VehicleRotation[i], 0, 1, 0);
		renderVehicle();
		modelStack.PopMatrix();
	}

	for (int i = 0; i < RenderOwner.size(); i++ )
	{
		modelStack.PushMatrix();
		modelStack.Translate(OwnerX[i], -275, OwnerZ[i]);
		modelStack.Rotate(OwnerRotation[i], 0, 1, 0);
		if ( RenderOwner[i] == 1 )
			renderOwner();
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(6000, -275, 0);
	modelStack.Rotate(180, 0, 1, 0);
	renderPromoter();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	if (SGState == 1 )
		modelStack.Translate(1100, -220, 400);
	else
		modelStack.Translate(1100, -275, 480);
	modelStack.Rotate(-90, 0, 1, 0);
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
}

void StudioProject2::renderBounds()
{
	/*modelStack.PushMatrix();
	modelStack.Translate(playerPos.x,playerPos.y,playerPos.z);
	RenderMesh(meshList[GEO_BOUNDHELPER], false, false);
	modelStack.PopMatrix();*/

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
	modelStack.Rotate(rotateAngle,0,1,0);
	RenderMesh(meshList[GEO_PLAYERBOUNDS],false,false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(2230,-53,1190);
	RenderMesh(meshList[GEO_DOORBOUNDS], false, false);
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
	modelStack.Rotate(promoterRotateY, 0, 1, 0);
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
	modelStack.Rotate(itemRotateY, 0, 1, 0);
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
	modelStack.Translate(0, 0, 0);
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
	modelStack.Translate(0, SGTranslate, 0);
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
	if ( SGState == 1 )
	{
		modelStack.Translate(5, 4.5, -0.7);
		modelStack.Rotate(180, 0, 1, 0);
		modelStack.Rotate(-90, 0, 0, 1);
		modelStack.Rotate(-10, 1, 0, 0);
	}
	RenderMesh(meshList[GEO_SGLEFTLEG], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	if ( SGState == 1 )
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
	modelStack.Translate(2050, -280, -1600);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_CHILLER], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2050, -280, -1200);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_CHILLER], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2050, -280, -800);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_CHILLER], false, false);
	modelStack.PopMatrix();

	//Freezer at the right side
	modelStack.PushMatrix();
	modelStack.Translate(1400, -250, -1550);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_FREEZER], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1400, -250, -1000);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_FREEZER], false, false);
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
	for(float k = 50;k >= -250;k = k-50)
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
	for (float k = 40; k >-240; k = k-50)
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
	for(float k = 32; k > -268; k = k - 50)
	{
		for(float i = -2040; i > -2160; i = i - 40)
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
	for(float k = 30; k > -270; k = k - 50)
	{
		for(float i = -1365; i < -1265; i = i + 50)
		{
			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1000);
			modelStack.Scale(15,15,15);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CATFOOD], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -925);
			modelStack.Scale(15,15,15);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CATFOOD], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -850);
			modelStack.Scale(15,15,15);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CATFOOD], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -775);
			modelStack.Scale(15,15,15);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CATFOOD], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -700);
			modelStack.Scale(15,15,15);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CATFOOD], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -625);
			modelStack.Scale(15,15,15);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CATFOOD], false, false);
			modelStack.PopMatrix();
		}
	}

	//Cereal Box

	for( float k = 27; k > -273; k = k - 50)
	{
		for(float i = -1020; i > -1170; i = i - 30)
		{
			modelStack.PushMatrix();
			modelStack.Translate(i, k, -650);
			modelStack.Scale(40,40,40);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CEREAL], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -720);
			modelStack.Scale(40,40,40);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CEREAL], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -790);
			modelStack.Scale(40,40,40);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CEREAL], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -860);
			modelStack.Scale(40,40,40);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CEREAL], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -930);
			modelStack.Scale(40,40,40);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CEREAL], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1000);
			modelStack.Scale(40,40,40);
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
	for(float k = 30;k > -270;k = k -50)
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
	for(float k = 50; k > -250; k = k -50)
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
	for(float k = 49; k > -251;k = k -50)
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
	for(float k = 30;k > -270;k = k -50)
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
	for(float k = 30;k > -270;k = k -50)
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

void StudioProject2::CollisionCheck(double dt)
{
	/************
	   TESTING
	*************/

	playerPos = camera.position;
	box[PLAYER]->Max = playerPos + playerBounds;
	box[PLAYER]->Min = playerPos - playerBounds;

	currView = "NONE";
	//lastView = "NONE";

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
		if( ((box[PLAYER]->Min.x < box[SHELF1]->Max.x) && (box[PLAYER]->Max.x > box[SHELF1]->Min.x)  &&
			 (box[PLAYER]->Min.y < box[SHELF1]->Max.y) && (box[PLAYER]->Max.y > box[SHELF1]->Min.y)  &&
			 (box[PLAYER]->Min.z < box[SHELF1]->Max.z) && (box[PLAYER]->Max.z > box[SHELF1]->Min.z)) ||

			((box[PLAYER]->Min.x < box[SHELF2]->Max.x) && (box[PLAYER]->Max.x > box[SHELF2]->Min.x)  &&
			 (box[PLAYER]->Min.y < box[SHELF2]->Max.y) && (box[PLAYER]->Max.y > box[SHELF2]->Min.y)  &&
			 (box[PLAYER]->Min.z < box[SHELF2]->Max.z) && (box[PLAYER]->Max.z > box[SHELF2]->Min.z)) )
		{
			canMove = false;
		}
		else
		{
			canMove = true;
		}
	}
	if(Application::IsKeyPressed('S'))
	{
		if( ((box[PLAYER]->Min.x < box[SHELF1]->Max.x) && (box[PLAYER]->Max.x > box[SHELF1]->Min.x)  &&
			 (box[PLAYER]->Min.y < box[SHELF1]->Max.y) && (box[PLAYER]->Max.y > box[SHELF1]->Min.y)  &&
			 (box[PLAYER]->Min.z < box[SHELF1]->Max.z) && (box[PLAYER]->Max.z > box[SHELF1]->Min.z)) ||

			((box[PLAYER]->Min.x < box[SHELF2]->Max.x) && (box[PLAYER]->Max.x > box[SHELF2]->Min.x)  &&
			 (box[PLAYER]->Min.y < box[SHELF2]->Max.y) && (box[PLAYER]->Max.y > box[SHELF2]->Min.y)  &&
			 (box[PLAYER]->Min.z < box[SHELF2]->Max.z) && (box[PLAYER]->Max.z > box[SHELF2]->Min.z)) )
		{
			canMove = false;
		}
		else
		{
			canMove = true;
		}
	}
	if(Application::IsKeyPressed('A'))//(playerPos.x < newBox.Min.x))
	{
		if( ((box[PLAYER]->Min.x < box[SHELF1]->Max.x) && (box[PLAYER]->Max.x > box[SHELF1]->Min.x)  &&
			 (box[PLAYER]->Min.y < box[SHELF1]->Max.y) && (box[PLAYER]->Max.y > box[SHELF1]->Min.y)  &&
			 (box[PLAYER]->Min.z < box[SHELF1]->Max.z) && (box[PLAYER]->Max.z > box[SHELF1]->Min.z)) ||

			((box[PLAYER]->Min.x < box[SHELF2]->Max.x) && (box[PLAYER]->Max.x > box[SHELF2]->Min.x)  &&
			 (box[PLAYER]->Min.y < box[SHELF2]->Max.y) && (box[PLAYER]->Max.y > box[SHELF2]->Min.y)  &&
			 (box[PLAYER]->Min.z < box[SHELF2]->Max.z) && (box[PLAYER]->Max.z > box[SHELF2]->Min.z)) )
		{
			canMove = false;
		}
		else
		{
			canMove = true;
		}
	}
	if(Application::IsKeyPressed('D'))//(playerPos.x > newBox.Max.x))
	{
		if( ((box[PLAYER]->Min.x < box[SHELF1]->Max.x) && (box[PLAYER]->Max.x > box[SHELF1]->Min.x)  &&
			 (box[PLAYER]->Min.y < box[SHELF1]->Max.y) && (box[PLAYER]->Max.y > box[SHELF1]->Min.y)  &&
			 (box[PLAYER]->Min.z < box[SHELF1]->Max.z) && (box[PLAYER]->Max.z > box[SHELF1]->Min.z)) ||

			((box[PLAYER]->Min.x < box[SHELF2]->Max.x) && (box[PLAYER]->Max.x > box[SHELF2]->Min.x)  &&
			 (box[PLAYER]->Min.y < box[SHELF2]->Max.y) && (box[PLAYER]->Max.y > box[SHELF2]->Min.y)  &&
			 (box[PLAYER]->Min.z < box[SHELF2]->Max.z) && (box[PLAYER]->Max.z > box[SHELF2]->Min.z)) )
		{
			canMove = false;
		}
		else
		{
			canMove = true;
		}
	}
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


