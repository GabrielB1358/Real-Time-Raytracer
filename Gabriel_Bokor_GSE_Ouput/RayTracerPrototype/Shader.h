#pragma once
#include "Headers.h"

struct Vao;
struct Texture;
struct Shader
{
	/*****************************************************************************//**
	*	Constructor for shader class
	*
	*	Calls the function to actually create the shader using the given parameters
	*	
	*	@param _sPath file path of the shader code
	*	@param _sType OpenGL shader type variable
	*	@param _isCompute true / false to determine if shader is compute or not
	*********************************************************************************/
	Shader(std::string _sPath, GLenum _sType, bool _isCompute);
	~Shader();//! Deconstructor for shader class

	/*****************************************************************************//**
	*	Creates a shader using pre-supplied variables
	*
	*	Creates a shader using the path and type from the constructor. this shader
	*	is then pushed back into a vector of shaders.
	* 
	*	A check is also performed to see if its creation was done properly
	*********************************************************************************/
	void CreateShader(std::string _path);

	GLuint GetProgramId();

	/*****************************************************************************//**
	*	Adds another, new shader to the collection
	*
	*	Resets the shader path and type values and calls to create another shader
	*	with the updated data
	*********************************************************************************/
	void AddShader(std::string _path, GLenum _type);

	/*****************************************************************************//**
	*	Creates a program and links shaders to it
	* 
	*	This function first creates an OpenGL program which can be of Compute type or
	*	normal.
	*	
	*	It then attaches the pre-made shaders to the program and performs a link.
	*	Finally, it checks that the program was made entirely correctly or throws an error
	*********************************************************************************/
	void Link();

	/*****************************************************************************//**
	*	Runs the program and aids rendering
	* 
	*	This function is what actually uses the programs. It does so with respect
	*	to whether or not its a compute program
	* 
	*	A compute program is used with a dispatch compute whereas normal programs
	*	are executed then used to draw a quad with the texture on it
	* 
	*	@param _res The windows resolution to keep everything in scale
	*	@param _vaoId The Vao needed for standard program rendering
	*	@param _texId The Texture to be drawn to and rendered
	*********************************************************************************/
	void Render(glm::vec2 _res, Vao* _vaoId , Texture* _texId);

	std::string ReadFile(std::string _path);//! Converts a txt file to a string

private:
	const GLchar* m_src;//! Shader code post conversion from file
	GLenum m_ShaderType;//! OpenGL variable for shader type e.g. Fragment, shader, compute
	std::vector<GLuint> m_ShaderIds;//! Vector containing shaders
	GLuint m_ProgramId;//! Id for actual program
	bool m_IsCompute;//! bool for controlling whether its a compute shader program or not
};