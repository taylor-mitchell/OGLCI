#pragma once
#include <unordered_map>
#include "Shader.h"

class ShaderFactory
{
private:
	static ShaderFactory* instance;
	std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;

	ShaderFactory()
		:m_shaders()
	{
	}

public:
	static ShaderFactory* getInstance()
	{
		if (!instance)
		{
			instance = new ShaderFactory;
		}
		return instance;
	}

	std::shared_ptr<Shader> getShader(const std::string& filename);
};