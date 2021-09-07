#pragma once

#include "c:/development/OGLCI/external-libraries/stb/include/stb_truetype.h"
#include <string>
#include <map>
#include "Texture.h"
#include "Glyph.h"


class Font
{
private:
	std::shared_ptr<Texture> fontTexture;
	std::string filename;
	std::map<float, std::map<char, glm::vec2>> glyphTexturePositions;
	stbtt_fontinfo fontInfo;
	int descent;
	int ascent;
	int lineGap;
	std::vector<float> textureLineHeights;

	void init();

public:
	Font();
	Font(std::string fn);

	Glyph getChar(char c, float ptSize);

	std::shared_ptr<Texture> getTexture()
	{
		return fontTexture;
	}
};