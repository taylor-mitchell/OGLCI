#include "Shader.h"
#include <iostream>
#include <fstream>

Shader::Shader()
{

}

Shader::Shader(const std::string& filename)
{
	error = 0;
	programHandle = glCreateProgram();
	shaders[0] = createShader(loadShader(filename + ".vs"), GL_VERTEX_SHADER);
	shaders[1] = createShader(loadShader(filename + ".fs"), GL_FRAGMENT_SHADER);

	if (error == 0)
	{

		for (unsigned int i = 0; i < NUM_SHADERS; i++)
		{
			glAttachShader(programHandle, shaders[i]);
		}

		glBindAttribLocation(programHandle, 0, "position");
		glBindAttribLocation(programHandle, 1, "texCoord");
		glBindAttribLocation(programHandle, 2, "normal");

		glLinkProgram(programHandle);
		checkShaderError(programHandle, GL_LINK_STATUS, true, "Error linking shader program");

		glValidateProgram(programHandle);
		checkShaderError(programHandle, GL_LINK_STATUS, true, "Invalid shader program");

		uniforms[TRANSFORM_U] = glGetUniformLocation(programHandle, "transform");
	}
}

Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(programHandle, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	glDeleteProgram(programHandle);
}

void Shader::bind()
{
	glUseProgram(programHandle);
}

void Shader::update(const Transform& transforms)
{
	//glm::mat4 model = transforms.getModel();

	//glUniformMatrix4fv(uniforms[TRANSFORM_U], 1, GL_FALSE, &model[0][0]);
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
		ERROR("Unable to load shader: " + filename);
		error = 1;
	}

	return output;
}

bool Shader::checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
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

		ERROR(errorMessage + ": '" + errorString + "'");
		error = 1;
		return false;
	}
	return true;
}

unsigned int Shader::createShader(const std::string& text, unsigned int type)
{
	unsigned int shader = glCreateShader(type);

	if (shader == 0)
	{
		ERROR("Error compiling shader type " + type);
	}

	const char* p[1];
	p[0] = text.c_str();
	int lengths[1];
	lengths[0] = text.length();

	glShaderSource(shader, 1, p, lengths);
	glCompileShader(shader);

	checkShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

	return shader;
}