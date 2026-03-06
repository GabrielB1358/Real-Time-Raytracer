#include "Vao.h"

Vao::Vao() : m_Id(0), m_DirtyFlag(false) {}
Vao::~Vao() {}

GLuint Vao::GetVaoId()
{
	//Check if Vao exists already, if not then create one and assign VBOs
	if (!m_Id)
	{
		glGenVertexArrays(1, &m_Id);
		if (!m_Id)
		{
			throw std::runtime_error("Error: Could not generate Vao");
		}
	}

	if (m_DirtyFlag)
	{
		glBindVertexArray(m_Id);

		for (int i = 0; i < m_Vbos.size(); i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_Vbos[i]->GetVboId());

			glVertexAttribPointer(i, m_Vbos[i]->GetComponentCount(), GL_FLOAT, GL_FALSE, 0, (void*)0);

			glEnableVertexAttribArray(i);
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_DirtyFlag = false;
	}

	return m_Id;
}

void Vao::AddVbo(std::shared_ptr<Vbo> _vbo)
{
	//Add Vbo to vector
	m_Vbos.push_back(_vbo);

	m_DirtyFlag = true;
}