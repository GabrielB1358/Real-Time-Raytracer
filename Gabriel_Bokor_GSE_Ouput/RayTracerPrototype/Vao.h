#pragma once
#include "Headers.h"
#include "Vbo.h"

struct Vao
{
	Vao();//! Constructor for VAO class, sets ID and dirtyFlag
	~Vao();//! Deconstructor for Vao class

	GLuint GetVaoId();

	void AddVbo(std::shared_ptr<Vbo> _vbo);//! Adds a vbo and sets dirty flag true

private:
	GLuint m_Id;//! Id for Vao
	bool m_DirtyFlag;//! boolean Dirty flag variable
	std::vector<std::shared_ptr<Vbo> > m_Vbos{};//! Vector containing pointers to all VBOs
};