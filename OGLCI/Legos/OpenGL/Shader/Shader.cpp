#include "Shader.h"
#include "ErrorChecking.h"
#include <fstream>


Shader::Shader()
{
}

Shader::Shader(const std::string& filename)
	:m_filepath(filename), m_rendererId(0)
{
	m_rendererId = glCreateProgram();
	shaders[0] = createShader(loadShader(filename + ".vs"), GL_VERTEX_SHADER);
	shaders[1] = createShader(loadShader(filename + ".fs"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		GLCall(glAttachShader(m_rendererId, shaders[i]));
	}

	GLCall(glLinkProgram(m_rendererId));
	checkShaderError(m_rendererId, GL_LINK_STATUS, true, "Error linking shader program");

	GLCall(glValidateProgram(m_rendererId));
	checkShaderError(m_rendererId, GL_LINK_STATUS, true, "Invalid shader program");


	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		GLCall(glDeleteShader(shaders[i]));
	}
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_rendererId));
}

void Shader::load(const std::string& filepath)
{
	if (m_rendererId)
	{
		GLCall(glDeleteProgram(m_rendererId));
	}

	m_rendererId = glCreateProgram();
	shaders[0] = createShader(loadShader(filepath + ".vs"), GL_VERTEX_SHADER);
	shaders[1] = createShader(loadShader(filepath + ".fs"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		GLCall(glAttachShader(m_rendererId, shaders[i]));
	}

	GLCall(glLinkProgram(m_rendererId));
	checkShaderError(m_rendererId, GL_LINK_STATUS, true, "Error linking shader program");

	GLCall(glValidateProgram(m_rendererId));
	checkShaderError(m_rendererId, GL_LINK_STATUS, true, "Invalid shader program");


	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		GLCall(glDeleteShader(shaders[i]));
	}
}

void Shader::bind() const
{
	GLCall(glUseProgram(m_rendererId));
}

void Shader::unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::setUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(getUniformLocation(name), value));
}

void Shader::setUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
	GLCall(glUniform4f(getUniformLocation(name), f0, f1, f2, f3));
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& value)
{
	GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]));
}

int Shader::getUniformLocation(const std::string& name)
{
	if (m_uniformCache.find(name) != m_uniformCache.end())
	{
		return m_uniformCache[name];
	}

	GLCall(unsigned int location = glGetUniformLocation(m_rendererId, name.c_str()));
	
	if (location == -1)
	{
		WARN_LOG("uniform " + name + " not found in shader");
	}

	m_uniformCache[name] = location;	

	return location;
}


std::string Shader::loadShader(const std::string& filename)
{
	std::ifstream file;
	file.open((filename).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		ERROR_LOG("Unable to open shader file: " + filename);
	}

	return output;
}

bool Shader::checkShaderError(unsigned int shader, unsigned int flag, bool isProgram, const std::string& errorMessage)
{
	int success = 0;
	char errorString[1024] = { 0 };

	if (isProgram)
	{
		glGetProgramiv(shader, flag, &success);
	}
	else
	{
		glGetShaderiv(shader, flag, &success);
	}

	if (success == GL_FALSE)
	{
		if (isProgram)
		{
			glGetProgramInfoLog(shader, sizeof(errorString), NULL, errorString);
		}
		else
		{
			glGetShaderInfoLog(shader, sizeof(errorString), NULL, errorString);
		}

		ERROR_LOG(errorMessage + ": '" + errorString + "'");
		return false;
	}
	return true;
}

unsigned int Shader::createShader(const std::string& text, unsigned int type)
{
	GLCall(unsigned int shader = glCreateShader(type));

	if (shader == 0)
	{
		ERROR_LOG("Error compiling shader type " + type);
	}

	const char* p[1];
	p[0] = text.c_str();
	int lengths[1];
	lengths[0] = static_cast<int>(text.length());

	glShaderSource(shader, 1, p, lengths);
	glCompileShader(shader);

	checkShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

	return shader;
}


