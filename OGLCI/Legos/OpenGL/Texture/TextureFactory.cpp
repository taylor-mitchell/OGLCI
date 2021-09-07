#include "TextureFactory.h"

TextureFactory* TextureFactory::instance = new TextureFactory;

std::shared_ptr<Texture> TextureFactory::getTexture(const std::string& filename)
{
	if (m_textures.find(filename) == m_textures.end())
	{
		m_textures.insert(std::pair<std::string, std::shared_ptr<Texture>>(filename, std::make_shared<Texture>(filename)));
	}
	return m_textures.at(filename);
}
