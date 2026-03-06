#pragma once
#include "Headers.h"

struct Vbo
{
	Vbo();//! Constructor for Vbo class, sets id, dirtyflag and component count
	~Vbo();//! Deconstructor for Vbo class

	void AddVec3(const glm::vec3& _vec3);//! Adds a vec3 to the VBO data stored
	void AddVec2(const glm::vec2& _vec2);//! Adds a vec2 to the VBO data stored
	GLuint GetVboId();
	int GetComponentCount() const;

private:
	GLuint m_Id;//! Id for VBO
	std::vector<float> m_Data;//! Vector containing all Vbo data
	bool m_DirtyFlag;//! true / false dirty flag
	int m_Components;//! Number of components of Vbo
};