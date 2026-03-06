#include "Texture.h"

Texture::Texture(glm::ivec2 _resolution)
{
	m_size = _resolution;

	InitialiseTexture();
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_Id);
}

void Texture::InitialiseTexture()
{
	//Create texture and initialise for writing in compute shader
	glCreateTextures(GL_TEXTURE_2D, 1, &m_Id);
	glTextureParameteri(m_Id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(m_Id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureStorage2D(m_Id, 1, GL_RGBA32F, m_size.x, m_size.y);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Id);

	glBindImageTexture(0, m_Id, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}

GLuint Texture::GetTexId()
{
	return m_Id;
}