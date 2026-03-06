#pragma once
#include "Headers.h"
#include "Vao.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_sdl.h>
#include <IMGUI/imgui_impl_opengl3.h>

struct Program
{
	struct ShaderData
	{
		glm::mat4 TransformMatrix;
		float RandomNumber;
		float AASampleSize;
		float SSSampleSize;
		float GISampleSize;
		
		float lightOffset;

		int doReflect;
		int doSS;
		int doGI;

		glm::vec3 lightColour;
		int sceneChoice;
		int objectCount;
	};//! Struct contains all data that gets sent to the compute shader

	Program();//! Constructor for Program class
	~Program();//! Destructor for Program class


	/*****************************************************************************//**
	*	The Run function for the Program class
	*
	*	This function creates relevant variables for and then begins the run loop
	*	of the rendering demonstration. Variables such as the window, GL context etc.
	*	
	*	Contains all relevant functionality for the ImGui GUI.
	*********************************************************************************/
	void Run();
};