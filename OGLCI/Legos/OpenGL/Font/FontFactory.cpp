#include "FontFactory.h"

FontFactory* FontFactory::instance = new FontFactory;

std::shared_ptr<Font> FontFactory::getFont(const std::string& filename)
{
	if (m_fonts.find(filename) == m_fonts.end())
	{
		m_fonts.insert(std::pair<std::string, std::shared_ptr<Font>>(filename, std::make_shared<Font>(filename)));
	}
	return m_fonts.at(filename);
}
