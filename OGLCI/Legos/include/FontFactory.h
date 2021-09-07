#pragma once
#include <unordered_map>
#include "Font.h"

class FontFactory
{
private:
	static FontFactory* instance;
	std::unordered_map<std::string, std::shared_ptr<Font>> m_fonts;

	FontFactory()
		:m_fonts()
	{
	}

public:
	static FontFactory* getInstance()
	{
		if (!instance)
		{
			instance = new FontFactory;
		}
		return instance;
	}

	std::shared_ptr<Font> getFont(const std::string& filename);
};