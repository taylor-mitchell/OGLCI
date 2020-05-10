#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>
#include "Transform.h"
#include "Logger.h"

class Shader
{
private:
	Shader(const Shader& sh) {}
	void operator=(const Shader& sh) {}

	static const unsigned int NUM_SHADERS = 2;

	std::string loadShader(const std::string& filename);
	bool checkShaderError(unsigned int shader, unsigned int flag, bool isProgram, const std::string& errorMessage);
	unsigned int createShader(const std::string& text, unsigned int type);

	enum
	{
		TRANSFORM_U,
		NUM_UNIFORMS
	};

	unsigned int programHandle;
	unsigned int shaders[NUM_SHADERS];
	unsigned int uniforms[NUM_UNIFORMS];
	unsigned int error;

public:
	Shader();
	Shader(const std::string& filename);
	virtual ~Shader();

	void bind();
	void update(const Transform& transform);
	unsigned int getError() { return error;	};
	unsigned int getProgram() { return programHandle; };
};


#endif