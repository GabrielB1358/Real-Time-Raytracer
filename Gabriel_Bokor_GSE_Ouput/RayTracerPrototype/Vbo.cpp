#include "Vbo.h"

Vbo::Vbo() : m_Id(0), m_DirtyFlag(false), m_Components(0) {}
Vbo::~Vbo() {}

void Vbo::AddVec3(const glm::vec3& _vec3)
{
	m_Data.push_back(_vec3.x);
	m_Data.push_back(_vec3.y);
	m_Data.push_back(_vec3.z);
	m_DirtyFlag = true;

	m_Components = 3;
}

void Vbo::AddVec2(const glm::vec2& _vec2)
{
	m_Data.push_back(_vec2.x);
	m_Data.push_back(_vec2.y);
	m_DirtyFlag = true;

	m_Components = 2;
}

int Vbo::GetComponentCount() const
{
	if (!m_Components)
	{
		throw std::runtime_error("Error: cant access components, Vbo not yet set");
	}
	return m_Components;
}

GLuint Vbo::GetVboId()
{
	//Check if VBO valid, if not then create one and bind data to it
	if (!m_Id)
	{
		glGenBuffers(1, &m_Id);
		if (!m_Id)
		{
			throw std::runtime_error("Error: Could not generate VBO");
		}
	}
	if (m_DirtyFlag)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_Data.size(), &m_Data.at(0), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_DirtyFlag = false;
	}
	return m_Id;
}