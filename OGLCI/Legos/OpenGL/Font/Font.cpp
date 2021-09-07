
#define STB_TRUETYPE_IMPLEMENTATION
#include "Font.h"
#include "Array2D.h"
#include "RendererFactory.h"
#include <iostream>
#include <fstream>


void Font::init()
{
	std::ifstream file;
	
	
	int textureWidth = 0;
	int textureHeight = 0;

	//setting to 30 for now
	textureLineHeights.push_back(30.0f);

	file.open(filename, std::ios::binary);

	if (!file.is_open())
	{
		ERROR_LOG("failed to open file");
		return;
	}

	int size = 0;

	file.seekg(0, std::ios_base::end);
	size = file.tellg();
	file.seekg(0);

	unsigned char* data = new unsigned char[size];

	file.read((char*)data, size);

	file.close();

	if (!stbtt_InitFont(&fontInfo, data, 0))
	{
		ERROR_LOG("failed to initialize font");
		delete[] data;
		data = 0;
		return;
	}
		
	const char* allCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	float scale = stbtt_ScaleForPixelHeight(&fontInfo, textureLineHeights[0]);
	stbtt_GetFontVMetrics(&fontInfo, &ascent, &descent, &lineGap);
	textureHeight = roundf(ascent * scale) - roundf(descent * scale);

	for (int i = 0; i < strlen(allCharacters); ++i)
	{
		int ax;
		int lsb;
		stbtt_GetCodepointHMetrics(&fontInfo, allCharacters[i], &ax, &lsb);
		textureWidth += roundf(ax * scale);


		int c_x1, c_y1, c_x2, c_y2;
		stbtt_GetCodepointBitmapBox(&fontInfo, allCharacters[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
	}

	int x = 0;
	unsigned char* bitmap = (unsigned char*)malloc(textureWidth * textureHeight * sizeof(unsigned char));
	memset(bitmap, 0, textureWidth * textureHeight * sizeof(unsigned char));
	for (int i = 0; i < strlen(allCharacters); ++i)
	{
		int ax;
		int lsb;
		stbtt_GetCodepointHMetrics(&fontInfo, allCharacters[i], &ax, &lsb);

		int c_x1, c_y1, c_x2, c_y2;
		stbtt_GetCodepointBitmapBox(&fontInfo, allCharacters[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

		int y = roundf(ascent * scale) + c_y1;

		int byteOffset = x + roundf(lsb * scale) + (y * textureWidth);
		stbtt_MakeCodepointBitmap(&fontInfo, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, textureWidth, scale, scale, allCharacters[i]);

		glyphTexturePositions[textureLineHeights[0]][allCharacters[i]].x = (float)(x-1) / (float)textureWidth;
		glyphTexturePositions[textureLineHeights[0]][allCharacters[i]].y = (float)(std::abs(roundf(descent * scale)) - c_y2) / (float)textureHeight;

		x += roundf(ax * scale);
	}

	Array2D<glm::vec3> pixels(textureWidth, textureHeight);
	for (int i = 0; i < textureWidth; ++i)
	{
		for (int j = 0; j < textureHeight; ++j)
		{

			pixels(i, j) = glm::vec3((float)(bitmap[i + ((textureHeight - j) * textureWidth)]));
			
		}
	}
	
	
	fontTexture = std::make_shared<Texture>(textureWidth, textureHeight);

	RendererFactory::getInstance()->getRenderer(0)->renderToTexture(*fontTexture, pixels);	
}

Font::Font()
	:filename(),
	textureLineHeights()
{
}

Font::Font(std::string fn)
	:filename(fn),
	textureLineHeights()
{
	init();	
}

Glyph Font::getChar(char c, float ptSize)
{
	float closestTextureHeight = textureLineHeights[0];
	float diff = std::abs(ptSize - textureLineHeights[0]);
	for (float it : textureLineHeights)
	{
		float newDiff = std::abs(ptSize - it);
		if (newDiff < diff)
		{
			closestTextureHeight = it;
			diff = newDiff;
		}
	}

	float glyphScale = stbtt_ScaleForPixelHeight(&fontInfo, ptSize);
	float textureScale = stbtt_ScaleForPixelHeight(&fontInfo, closestTextureHeight);
	int glyphAdvanceX, glyphLeftSideBearing, glyphX1, glyphY1, glyphX2, glyphY2;
	int textureAdvanceX, textureLeftSideBearing, textureX1, textureY1, textureX2, textureY2;
	stbtt_GetCodepointHMetrics(&fontInfo, c, &glyphAdvanceX, &glyphLeftSideBearing);
	stbtt_GetCodepointBitmapBox(&fontInfo, c, glyphScale, glyphScale, &glyphX1, &glyphY1, &glyphX2, &glyphY2);
	stbtt_GetCodepointHMetrics(&fontInfo, c, &textureAdvanceX, &textureLeftSideBearing);
	stbtt_GetCodepointBitmapBox(&fontInfo, c, textureScale, textureScale, &textureX1, &textureY1, &textureX2, &textureY2);

	INFO_LOG(c);
	INFO_LOG(std::to_string(glyphY2));
	INFO_LOG(std::to_string(glyphY1));
	Glyph toReturn;
	toReturn.setTexture(fontTexture);
	toReturn.setSize(glm::vec2(glyphX2 - glyphX1 + 2, glyphY2 - glyphY1));
	toReturn.setTextureSize(glm::vec2((float)(textureX2 - textureX1 + 2) / (float)fontTexture->getWidth(), (float)(textureY2 - textureY1) / (float)fontTexture->getHeight()));
	toReturn.setTexturePosition(glm::vec2(glyphTexturePositions[closestTextureHeight][c]));
	toReturn.setOffset(glm::vec2(roundf(glyphLeftSideBearing * glyphScale), (std::abs(roundf(descent * glyphScale)) - glyphY2)));
	toReturn.setAdvancement(roundf(glyphAdvanceX * glyphScale));
	return toReturn;
}
