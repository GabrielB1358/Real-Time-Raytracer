#pragma once
#include "Headers.h"

struct Texture
{
	Texture(glm::ivec2 _resolution);//! Constructor for texture class, sets resolution & calls to initialise
	~Texture();//! Deconstructor for Texture class

	/*****************************************************************************//**
	*	Initialises empty texture
	*
	*	Creates the blank texture and sets it up for use within the compute shader
	*********************************************************************************/
	void InitialiseTexture();

	GLuint GetTexId();

private:
	glm::ivec2 m_size;//! Texture resolution
	GLuint m_Id;//! Id for texture, where texture is stored
};