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

//Test
double FPS;
std::string textPS;
double toggleDelay = 0.0f;

vector<BoundingBox*> box;
BoundingBox * boxPtr;

StudioProject2::StudioProject2()
{
}

StudioProject2::~StudioProject2()
{
}

void StudioProject2::Init()
{
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
	moveSpeed = 200.0f;
	rotateAngle = 0.0f;

	SGState = 0; //0 = sitting, 1 = chasing player
	SGTranslate = 0;
	SGLegTranslate = 0;

	Customers = 1;
	CustomerX[0] = 5000;
	CustomerZ[0] = 7000;
	CustomerRotation[0] = 90;
	CustomerState[0] = 0;

	/********************
	BOUNDS INIT GO HERE
	********************/
	boxPtr = new BoundingBox();
	boxPtr->Max = insideBounds;
	boxPtr->Min = -insideBounds;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->isInside = false;
	boxPtr->Max = outsideBounds;
	boxPtr->Min = -outsideBounds;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->isObj = true;
	boxPtr->Max = shelfBounds1Max;
	boxPtr->Min = shelfBounds1Min;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->isObj = true;
	boxPtr->Max = shelfBounds2Max;
	boxPtr->Min = shelfBounds2Min;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->isObj = true;
	boxPtr->Max = shelfBounds3Max;
	boxPtr->Min = shelfBounds3Min;
	box.push_back(boxPtr);

	boxPtr = new BoundingBox();
	boxPtr->isObj = true;
	boxPtr->Max = shelfBounds4Max;
	boxPtr->Min = shelfBounds4Min;
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

	camera.Init(Vector3(1, 150, -230), playerPos, Vector3(0, 1, 0));

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

	meshList[GEO_BOUNDHELPER] = MeshBuilder::GenerateSphere("BoundHelper", Color(1,1,1), 10, 18, 5.f);

	/***************************************************************************************************************
	THIS SECTION IS FOR BOUNDS MESH INIT
	***************************************************************************************************************/
	meshList[GEO_INSIDEMARKETBOUNDS] = MeshBuilder::GenerateBoundingBox("InsideBounds", box[0]->Max, box[0]->Min, Color(1,0,0));
	meshList[GEO_OUTSIDEMARKETBOUNDS] = MeshBuilder::GenerateBoundingBox("OutsideBounds", box[1]->Max, box[1]->Min, Color(1,0,0));
	meshList[GEO_SHELFBOUNDS1] = MeshBuilder::GenerateBoundingBox("ShelfBounds1", box[2]->Max, box[2]->Min, Color(0,0,1));
	meshList[GEO_SHELFBOUNDS2] = MeshBuilder::GenerateBoundingBox("Shelf2Bounds", box[3]->Max, box[3]->Min, Color(0,0,1));
	meshList[GEO_SHELFBOUNDS3] = MeshBuilder::GenerateBoundingBox("Shelf3Bounds", box[4]->Max, box[4]->Min, Color(0,0,1));
	meshList[GEO_SHELFBOUNDS4] = MeshBuilder::GenerateBoundingBox("Shelf4Bounds", box[5]->Max, box[5]->Min, Color(0,0,1));
	/**************************************************************************************************************/

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("light", Color(1,1,1), 10, 10, 50);
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

	if(Application::IsKeyPressed('T') && (CollisionCheck())) //(playerPos.z < newBox.Min.z))
	{
		//playerPos += playerDir * moveSpeed;
		playerPos.z += 5.f;
		cout << "Player Pos : " << playerPos << endl;
	}
	if(Application::IsKeyPressed('G') && (CollisionCheck()))//(playerPos.z > newBox.Max.z))
	{
		//playerPos -= playerDir * moveSpeed;
		playerPos.z -= 5.f;
		cout << "Player Pos : " << playerPos << endl;
	}
	if(Application::IsKeyPressed('F') && (CollisionCheck()))//(playerPos.x < newBox.Min.x))
	{
		//rotateAngle += rotateSpeed * dt;
		playerPos.x += 5.f;
		cout << "Player Pos : " << playerPos << endl;
	}
	if(Application::IsKeyPressed('H') && (CollisionCheck()))//(playerPos.x > newBox.Max.x))
	{
		//rotateAngle -= rotateSpeed * dt;
		playerPos.x -= 5.f;
		cout << "Player Pos : " << playerPos << endl;
	}

	if ( Customers < 10 )
	{
		if ( rand() % 11 == 10 )
		{
			Customers++;
		}
	}
	
	for ( int i = 0; i < Customers; i++)
	{
		if ( CustomerState[i] == 0 )
		{
			//to carpark entrance
			if ( CustomerZ[i] > 1250 )
				CustomerZ[i] -= 50;
			else
				CustomerState[i] = 1;
		}
		else if ( CustomerState[i] == 1 )
		{
			//to within supermarket
			CustomerRotation[i] = 180;
			if ( CustomerX[i] > 850 )
				CustomerX[i] -= 50;
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
				CustomerZ[i] -= 50;
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
			if ( CustomerX[i] < 1700 )
				CustomerX[i] += 50;
			else
			{
				if ( CustomerZ[i] > -1700 )
					CustomerZ[i] -= 50;
				else
					CustomerState[i] = 8;
			}
		}
		else if ( CustomerState[i] == 8 )
		{
			//drinks section to gate
			CustomerRotation[i] = -90;
			if ( CustomerZ[i] < -200 )
				CustomerZ[i] += 50;
			else if ( CustomerX[i] > 1000 )
				CustomerX[i] -= 50;
			else
			{
				CustomerState[i] = 9;
			}
		}
		else if ( CustomerState[i] == 9 )
		{
			//gate to shelf1
			CustomerRotation[i] = 90;
			if ( CustomerZ[i] > -1000 )
				CustomerZ[i] -= 50;
			else 
			{
				CustomerRotation[i] = 180;
				if ( CustomerX[i] > 600 )
					CustomerX[i] -= 50;
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
		else if ( CustomerState[i] == 10 )
		{
			//shelf1 to shelf2
			CustomerRotation[i] = 180;
			if ( CustomerX[i] > 100 )
				CustomerX[i] -= 50;
			else
			{
				CustomerRotation[i] = -90;
				if ( CustomerZ[i] < -800 )
					CustomerZ[i] += 50;
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
				CustomerZ[i] -= 50;
		}
		else if ( CustomerState[i] == 12 )
		{
			//shelf2 to backshelf
		}
		else if ( CustomerState[i] == 13 )
		{
			//backshelves to left shelf
		}
		else if ( CustomerState[i] == 14 )
		{
			//backshelves to shelf4
		}
		else if ( CustomerState[i] == 15 )
		{
			//shelf4 to checkout1
		}
		else if ( CustomerState[i] == 16 )
		{
			//left shelf to front left shelf
		}
		else if ( CustomerState[i] == 17 )
		{
			//left shelves to customer service
		}
		else if ( CustomerState[i] == 18 )
		{
			//customerservice to checkout 1
		}
		else if ( CustomerState[i] == 19 )
		{
			//customerservice to checkout2
		}
		else if ( CustomerState[i] == 20 )
		{
			//customerservice to checkout3
		}
		else if ( CustomerState[i] == 21 )
		{
			//customerservice to checkout4
		}
		else if ( CustomerState[i] == 22 )
		{
			//customerservice to gates
		}
	}

	playerDir.x = dt * sin(Math::DegreeToRadian(rotateAngle));
	playerDir.z = dt * cos(Math::DegreeToRadian(rotateAngle));
	playerDir.y = 0.f;

	FPS = 1 / dt;
	std::ostringstream s;
	s << setprecision(9) << FPS;
	textPS = s.str();

	toggleDelay += dt;
	camera.Update(dt);

	//camera.target.Set(CustomerX[0], 0, CustomerZ[0]);
	//camera.position.Set(CustomerX[0], 0, CustomerZ[0] + 500);
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
	 
	renderSkybox();
	renderSupermarket();
	renderOutside();
	renderItems();

	modelStack.PushMatrix();
	modelStack.Translate(playerPos.x, playerPos.y, playerPos.z);
	//renderPlayer();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-250, -275, 350);
	renderCashier();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-500, -275, 350);
	modelStack.Rotate(180, 0, 1, 0);
	renderCashier();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(500, -275, 350);
	modelStack.Rotate(180, 0, 1, 0);
	renderCashier();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1200, -275, 350);
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
	modelStack.Translate(2240, -50, 1025);
	modelStack.Scale(50, 52, 50.5);
	RenderMesh(meshList[GEO_GLASSDOOR], false, false);
	modelStack.PopMatrix();
	
	//render glass door
	modelStack.PushMatrix();
	modelStack.Translate(2240, -50, 1365);
	modelStack.Scale(50, 52, 50.5);
	RenderMesh(meshList[GEO_GLASSDOOR], false, false);
	modelStack.PopMatrix();

	RenderTextOnScreen(meshList[GEO_TEXT],"FPS=" + textPS, Color(0, 1, 1), 2.5, 0, 23);
}

void StudioProject2::renderBounds()
{
	modelStack.PushMatrix();
	modelStack.Translate(playerPos.x,playerPos.y,playerPos.z);
	RenderMesh(meshList[GEO_BOUNDHELPER], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_INSIDEMARKETBOUNDS], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_OUTSIDEMARKETBOUNDS], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_SHELFBOUNDS1], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_SHELFBOUNDS2], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_SHELFBOUNDS3], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_SHELFBOUNDS4], false, false);
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
	modelStack.Translate(-2150, -275, -1579.5);
	modelStack.Rotate(0, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SHELF], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2150, -275, -1086.4);
	modelStack.Rotate(0, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SHELF], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-2150, -275, -593.5);
	modelStack.Rotate(0, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SHELF], false, false);
	modelStack.PopMatrix();

	//Shelf on back wall
	modelStack.PushMatrix();
	modelStack.Translate(-3.8, -275, -1750);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SHELF], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-496.9, -275, -1750);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SHELF], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-990, -275, -1750);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SHELF], false, false);
	modelStack.PopMatrix();

	//Shelf on right wall
	modelStack.PushMatrix();
	modelStack.Translate(2120, -275, -1600);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SHELF], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2120, -275, -1110);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SHELF], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(2120, -275, -620);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SHELF], false, false);
	modelStack.PopMatrix();

	//Shelf opposite right wall
	modelStack.PushMatrix();
	modelStack.Translate(1250, -275, -1600);
	modelStack.Rotate(0, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SHELF], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1250, -275, -1210);
	modelStack.Rotate(0, 0, 1, 0);
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
	modelStack.Translate(600, -275, -662);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SHELF], false, false);
	modelStack.PopMatrix();
}

void StudioProject2::renderItems()
{
	//Cactus Juice
	for(int k = 50;k >= -250;k = k-50)
	{
		for(int i = -2050;i > -2210;i = i - 40)
		{
			modelStack.PushMatrix();
			modelStack.Translate(i, k, -830);
			modelStack.Scale(80,80,80);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CACTUSJUICE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -780);
			modelStack.Scale(80,80,80);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CACTUSJUICE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -730);
			modelStack.Scale(80,80,80);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CACTUSJUICE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -680);
			modelStack.Scale(80,80,80);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CACTUSJUICE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -630);
			modelStack.Scale(80,80,80);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CACTUSJUICE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -580);
			modelStack.Scale(80,80,80);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CACTUSJUICE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -530);
			modelStack.Scale(80,80,80);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CACTUSJUICE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -480);
			modelStack.Scale(80,80,80);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CACTUSJUICE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -430);
			modelStack.Scale(80,80,80);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CACTUSJUICE], false, false);
			modelStack.PopMatrix();
		}
	}

	//Cadbury
	for (int k = 40; k >-240; k = k-50)
	{
		for (int i = -2030; i>-2180; i = i- 50)
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
	for(int k = 32; k > -268; k = k - 50)
	{
		for(int i = -2030; i > -2190; i = i - 40)
		{
			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1380);
			modelStack.Scale(60,60,60);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CANVEGE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1430);
			modelStack.Scale(60,60,60);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CANVEGE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1480);
			modelStack.Scale(60,60,60);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CANVEGE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1530);
			modelStack.Scale(60,60,60);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CANVEGE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1580);
			modelStack.Scale(60,60,60);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CANVEGE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1630);
			modelStack.Scale(60,60,60);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CANVEGE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1680);
			modelStack.Scale(60,60,60);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CANVEGE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1730);
			modelStack.Scale(60,60,60);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CANVEGE], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1780);
			modelStack.Scale(60,60,60);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CANVEGE], false, false);
			modelStack.PopMatrix();
		}
	}

	//Catfood
	for(int k = 30; k > -270; k = k - 50)
	{
		for(int i = -1415; i < -1265; i = i + 50)
		{
			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1050);
			modelStack.Scale(15,15,15);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CATFOOD], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -1000);
			modelStack.Scale(15,15,15);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CATFOOD], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -950);
			modelStack.Scale(15,15,15);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CATFOOD], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -900);
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
			modelStack.Translate(i, k, -800);
			modelStack.Scale(15,15,15);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CATFOOD], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -750);
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
			modelStack.Translate(i, k, -650);
			modelStack.Scale(15,15,15);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CATFOOD], false, false);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(i, k, -600);
			modelStack.Scale(15,15,15);
			modelStack.Rotate(90,0,1,0);
			RenderMesh(meshList[GEO_CATFOOD], false, false);
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
	modelStack.Translate(0, -275.5, 0);
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
	modelStack.Translate(3500, -276, 1250);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(2500, 2500, 1);
	RenderMesh(meshList[GEO_CARPARK], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(3500, -277, -1250);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(2500, 2500, 1);
	RenderMesh(meshList[GEO_CARPARK], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-50, -277, -1150);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(4600, 2700, 1);
	RenderMesh(meshList[GEO_CARPARK], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-50, -277, 1150);
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
	modelStack.Translate(5495, -273, 1250);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(1500, 1050, 1);
	RenderMesh(meshList[GEO_ROAD], false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(5495, -277, -1250);
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

bool StudioProject2::CollisionCheck()
{
	return true;
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


