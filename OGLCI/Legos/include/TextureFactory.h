#pragma once
#include <unordered_map>
#include "Texture.h"
#include <memory>

class TextureFactory
{
private:
	static TextureFactory* instance;
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;

	TextureFactory()
		:m_textures()
	{
	}

public:
	static TextureFactory* getInstance()
	{
		if (!instance)
		{
			instance = new TextureFactory;
		}
		return instance;
	}

	std::shared_ptr<Texture> getTexture(const std::string& filename);
};