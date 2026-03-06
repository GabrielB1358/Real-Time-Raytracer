#include "Shader.h"
#include "Vao.h"
#include "Texture.h"

Shader::Shader(std::string _sPath, GLenum _sType, bool _isCompute)
{
	m_IsCompute = _isCompute;
	m_ShaderType = _sType;
	CreateShader(_sPath);
}

Shader::~Shader()
{
}

void Shader::CreateShader(std::string _path)
{
	//Convert file code to text and use to create shader
	std::string tmp = ReadFile(_path);
	m_src = tmp.c_str();

	GLuint temp = glCreateShader(m_ShaderType);
	glShaderSource(temp, 1, & m_src, NULL);
	glCompileShader(temp);

	//Check for success
	GLint m_success = 0;
	glGetShaderiv(temp, GL_COMPILE_STATUS, &m_success);
	if (!m_success)
	{
		GLint maxLength = 0;
		glGetShaderiv(temp, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(temp, maxLength, +&maxLength, &errorLog[0]);
		std::cout << &errorLog.at(0) << std::endl;
		throw std::runtime_error("Shader failed to compile");
	}

	m_ShaderIds.push_back(temp);
}

void Shader::Link()
{
	//Create Program & link shaders
	m_ProgramId = glCreateProgram();
	GLint m_success = 0;
	if (m_IsCompute)
	{
		//Compute shader program
		glAttachShader(m_ProgramId, m_ShaderIds[0]);

		glLinkProgram(m_ProgramId);
		glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &m_success);

		if (!m_success)
		{
			GLchar errorLog[1024] = { 0 };
			glGetProgramInfoLog(m_ProgramId, 1024, NULL, errorLog);
			std::cout << "Linking Error " << errorLog << std::endl;
			throw std::exception();
		}
	}
	else
	{
		//Normal shader program
		for (int i = 0; i < m_ShaderIds.size(); i++)
		{
			glAttachShader(m_ProgramId, m_ShaderIds[i]);
		}

		glBindAttribLocation(m_ProgramId, 0, "in_Position");
		glBindAttribLocation(m_ProgramId, 1, "a_TexCoords");

		glLinkProgram(m_ProgramId);
		glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &m_success);

		if (!m_success)
		{
			GLchar errorLog[1024] = { 0 };
			glGetProgramInfoLog(m_ProgramId, 1024, NULL, errorLog);
			std::cout << "Linking Error " << errorLog << std::endl;
			throw std::exception();
		}
	}

	//Detach and delete shaders post use
	for (int i = 0; i < m_ShaderIds.size(); i++)
	{
		glDetachShader(m_ProgramId, m_ShaderIds[i]);
		glDeleteShader(m_ShaderIds[i]);
	}
}

void Shader::Render(glm::vec2 _res, Vao* _vaoId, Texture* _texId)
{
	//Use the program to either write to a texture or to render quad & texture
	if (m_IsCompute)
	{
		glUseProgram(m_ProgramId);
		glDispatchCompute(_res.x /8, _res.y/8, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		glUseProgram(0);
	}
	else
	{
		glUseProgram(m_ProgramId);
		glBindVertexArray(_vaoId->GetVaoId());
		glBindTexture(GL_TEXTURE_2D, _texId->GetTexId());
		glUniform1i(glGetUniformLocation(m_ProgramId, "u_Texture"), 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);

		glViewport(0, 0, _res.x, _res.y);
	}
}

void Shader::AddShader(std::string _path, GLenum _type)
{
	m_ShaderType = _type;
	CreateShader(_path);
}

GLuint Shader::GetProgramId()
{
	return m_ProgramId;
}

std::string Shader::ReadFile(std::string _path)
{
	std::string line;
	std::string fileString = "";
	std::ifstream file(_path);
	if (file.is_open())
	{
		while (std::getline(file, line)) { fileString += line + '\n'; }
		file.close();
	}
	else { throw std::runtime_error("Can't open shader file"); }

	return fileString;
}