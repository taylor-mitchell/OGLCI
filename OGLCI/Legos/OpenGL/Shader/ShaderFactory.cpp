#include "ShaderFactory.h"

ShaderFactory* ShaderFactory::instance = new ShaderFactory;

std::shared_ptr<Shader> ShaderFactory::getShader(const std::string& filename)
{
	if (m_shaders.find(filename) == m_shaders.end())
	{
		m_shaders.insert(std::pair<std::string, std::shared_ptr<Shader>>(filename, std::make_shared<Shader>(filename)));
	}
	return m_shaders.at(filename);
}
