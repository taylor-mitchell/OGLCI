#pragma once

#include "GLIncludes.h"
#include <string>
#include "Logger.h"
#include <unordered_map>
#include <glm.hpp>

class Shader
{
private:
	static const int NUM_SHADERS = 2;

	unsigned int shaders[NUM_SHADERS];
	unsigned int m_rendererId;
	std::string m_filepath;
	std::unordered_map<std::string, unsigned int> m_uniformCache;

	bool checkShaderError(unsigned int shader, unsigned int flag, bool isProgram, const std::string& errorMessage);
	unsigned int createShader(const std::string& text, unsigned int type);
	int getUniformLocation(const std::string& name);
	std::string loadShader(const std::string& filename);

public:
	Shader();
	Shader(const std::string& filepath);
	virtual ~Shader();

	void load(const std::string& filepath);
	void bind() const;
	void unbind() const;

	void setUniform1i(const std::string& name, int value);
	void setUniform1f(const std::string& name, float value);
	void setUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
	void setUniformMat4f(const std::string& name, const glm::mat4& value);
	//void update(const Transform& transform);
};
