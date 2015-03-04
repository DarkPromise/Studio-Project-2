#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>
#include "StudioProject2.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame
const int Application::width = 800;
const int Application::height = 600;

bool Application::keys[1024];

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
		{
            Application::keys[key] = true;
		    std::cout << "Press : " << key << std::endl;
		}
        else if (action == GLFW_RELEASE)
		{
            Application::keys[key] = false;
			std::cout << "Release : " << key << std::endl;
		}
    }
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if(button >= 0 && button < 8)
	{
		if(action == GLFW_PRESS)
		{
			Application::keys[button] = true;
			std::cout << "Button Pressed : " << button << std::endl;
		}
		else if (action == GLFW_RELEASE)
		{
			Application::keys[button] = false;
			std::cout << "Button Pressed : " << button << std::endl;
		}
	}
}

static void resize_callback(GLFWwindow* window, int w,int h)
{
	glViewport(0, 0, w, h); //Update openGL with new window size
}

bool Application::IsKeyPressed(unsigned short key)
{
   // return ((GetAsyncKeyState(key) & 0x8001) != 0);
	return Application::keys[key];
}

bool Application::IsButtonPressed(unsigned int button)
{
	//std::cout << "Button : " << button << std::endl;
	return Application::keys[button];
}


Application::Application()
{
	
}

Application::~Application()
{
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(800, 600, "Computer Graphics", NULL, NULL);

	glfwSetWindowSizeCallback(m_window, resize_callback);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	glfwSetKeyCallback(m_window, key_callback);
	glfwSetMouseButtonCallback(m_window, mouse_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}
}

void Application::Run()
{
	//Main Loop
	Scene *scene = new StudioProject2();
	scene->Init();

	glfwSetInputMode(m_window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{
		glfwGetCursorPos(m_window,&xpos,&ypos);
		scene->Update(m_timer.getElapsedTime(),xpos,ypos);
		glfwSetCursorPos(m_window,width/2,height/2);
		scene->Render();
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

	} //Check if the ESC key had been pressed or if the window had been closed
	scene->Exit();
	delete scene;
}

int Application::getHeight()
{
	return height;
}

int Application::getWidth()
{
	return width;
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}