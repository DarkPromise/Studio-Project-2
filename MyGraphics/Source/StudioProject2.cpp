#include "StudioProject2.h"
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "Mtx44.h"
#include <iostream>
#include "Application.h"
#include <sstream>
#include <iomanip>

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Material.h"
#include "Utility.h"
#include "LoadTGA.h"

using std::setprecision;
using std::cout;
using std::endl;

double FPS;
std::string textPS;
std::string pX;
std::string pZ;

double toggleDelay = 0.0f;

bool isFlying = true;
bool isOpen = false;
bool isThinking = false;

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
	
	//glEnable(GL_CULL_FACE);

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
	animateFlying = 5.0f;
	animateDoor = 0.0f;

	rotateAngle = 0.0f;

	playerPos.Set(0.f,0.f,0.f);

	// Init VBO here
	glClearColor(0.3f, 0.0f, 0.0f, 0.0f);

	//Default VAO
	glGenVertexArrays(1, &m_vertexArrayID); //Generate "x" buffer
	glBindVertexArray(m_vertexArrayID);

	Mtx44 projection;
	projection.SetToPerspective(50.0f,4.0f / 3.0f, 0.1f, 3000.0f);

	projectionStack.LoadMatrix(projection);

	camera.Init(Vector3(1, 150, -230), playerPos, Vector3(0, 1, 0));

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//ExportedFont.tga");

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("light", Color(1,1,1), 10, 10, 50);
}

void StudioProject2::RenderMesh(Mesh *mesh, bool enableLight)
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

	playerDir.x = dt * sin(Math::DegreeToRadian(rotateAngle));
	playerDir.z = dt * cos(Math::DegreeToRadian(rotateAngle));
	playerDir.y = 0.f;

	if(Application::IsKeyPressed('T'))
	{
		playerPos += playerDir * moveSpeed;
	}
	if(Application::IsKeyPressed('G'))
	{
		playerPos -= playerDir * moveSpeed;
	}
	if(Application::IsKeyPressed('F'))
	{
		rotateAngle += rotateSpeed * dt;
	}
	if(Application::IsKeyPressed('H'))
	{
		rotateAngle -= rotateSpeed * dt;
	}

	if(isFlying)
	{
		if(flyingDirection == 0)
		{
			if((animateFlying < 10) && (animateFlying > 0))
			{ 
				animateFlying += (float) (10 * dt);
			}
			else
			{
				setTranslate(1);
			}
		}
		else
		{
			if(animateFlying > 5 )
			{
				animateFlying -= (float) (10 * dt);
			}
			else
			{
				setTranslate(0);
			}
		}
	}

	if(isOpen)
	{
		if(animateDoor <= 0.8)
		{
			animateDoor += (float) (1 * dt);
		}
	}
	else
	{
		if(animateDoor > 0)
		{
			animateDoor -= (float) (1 * dt);
		}
	}

	FPS = 1 / dt;
	std::ostringstream s;
	s << setprecision(9) << FPS;
	textPS = s.str();
	
	std::ostringstream s1;
	s1 << playerPos.x;
	pX = s1.str();

	std::ostringstream s3;
	s3 << playerPos.z;
	pZ = s3.str();

	toggleDelay += dt;
	camera.Update(dt);
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

	RenderMesh(meshList[GEO_AXES], false);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*modelStack.PushMatrix();
	modelStack.Translate(0,100,0);
	renderSkybox();
	modelStack.PopMatrix();*/

	//////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////////
	
	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	///////////////////////////////////////////////////////////////////////////////////
	
	

	///////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if((playerPos.z >= 350 && playerPos.z <= 430) && (playerPos.x <= 70) && playerPos.x >= -70)
	{
		RenderTextOnScreen(meshList[GEO_TEXT],"Press E to interact with Door", Color(0, 1, 0), 2.5, 8.7, 10);
		if((Application::IsKeyPressed('E')) && (toggleDelay > 0.2))
		{
			if(isOpen)
			{
				isOpen = false;
				toggleDelay = 0.0;
				cout << "Closing" << endl;
			}
			else
			{
				isOpen = true;
				toggleDelay = 0.0;
				cout << "Opening" << endl;
			}
		}
	}

	if((playerPos.z <= -200 && playerPos.z >= -350) && (playerPos.x <= 150) && playerPos.x >= -150)
	{
		if((Application::IsKeyPressed('E')) && (toggleDelay > 0.2))
		{
			if(isThinking)
			{
				isThinking = false;
				toggleDelay = 0.0;
			}
			else
			{
				isThinking = true;
				toggleDelay = 0.0;
			}
		}
		if(isThinking)
		{
			RenderTextOnScreen(meshList[GEO_TEXT],"Metagross thinks you should give marks", Color(1, 0, 0), 2.5, 7.6, 9);
		}
		else
		{
			RenderTextOnScreen(meshList[GEO_TEXT],"Press E to Think", Color(0, 1, 0), 2.5, 11.8, 10);
		}
	}

	RenderTextOnScreen(meshList[GEO_TEXT],"FPS=" + textPS, Color(0, 1, 1), 2.5, 0, 23);
	RenderTextOnScreen(meshList[GEO_TEXT],"PlayerPosition", Color(0, 1, 0), 2.5, 0, 22);
	RenderTextOnScreen(meshList[GEO_TEXT],"X=" + pX, Color(0, 1, 0), 2.5, 0, 21);
	RenderTextOnScreen(meshList[GEO_TEXT],"Z=" + pZ, Color(0, 1, 0), 2.5, 0, 20);
}

//void StudioProject2::renderSkybox()
//{
//	modelStack.PushMatrix();
//	modelStack.Translate(0,0,-998);
//	RenderMesh(meshList[GEO_BACK], false);
//	modelStack.PopMatrix();
//
//	modelStack.PushMatrix();
//	modelStack.Rotate(180,0,1,0);
//	modelStack.Translate(0,0,-998);
//	RenderMesh(meshList[GEO_FRONT], false);
//	modelStack.PopMatrix();
//
//	modelStack.PushMatrix();
//	modelStack.Rotate(90,0,1,0);
//	modelStack.Translate(0,0,-998);
//	RenderMesh(meshList[GEO_RIGHT], false);
//	modelStack.PopMatrix();
//
//	modelStack.PushMatrix();
//	modelStack.Rotate(-90,0,1,0);
//	modelStack.Translate(0,0,-998);
//	RenderMesh(meshList[GEO_LEFT], false);
//	modelStack.PopMatrix();
//
//	modelStack.PushMatrix();
//	modelStack.Rotate(180,0,0,1);
//	modelStack.Rotate(90,1,0,0);
//	modelStack.Translate(0,0,-998);
//	RenderMesh(meshList[GEO_BOTTOM], false);
//	modelStack.PopMatrix();
//
//	modelStack.PushMatrix();
//
//	modelStack.Rotate(90,1,0,0);
//	modelStack.Rotate(180,0,0,1);
//	modelStack.Translate(0,0,-498);
//	RenderMesh(meshList[GEO_TOP], false);
//	modelStack.PopMatrix();
//}


//void StudioProject2::renderMetagross()
//{
//	modelStack.PushMatrix();
//	modelStack.Scale(15,15,15);
//	RenderMesh(meshList[GEO_CHAR],true);
//	RenderMesh(meshList[GEO_CHAREYE],true);
//	RenderMesh(meshList[GEO_CHARETC],false);
//	modelStack.PopMatrix();
//}

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


