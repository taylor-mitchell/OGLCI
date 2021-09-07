#include "TextBox.h"

void TextBox::init()
{
	if (font && fontSize > 0)
	{
		int x = 0;
		for (char c : text)
		{
			Glyph g = font->getChar(c, fontSize);
			g.setPos(glm::vec2(pos.x + x, pos.y));
			x += g.getAdvancement();
			glyphs.addToBuffer(g);
		}
		glyphs.updateBuffer();
		glyphs.setTexture(font->getTexture());
	}
	
}

void TextBox::updateVertices()
{
	
}

TextBox::TextBox()
	:Drawable(DrawMode::SINGLETON),
	pos(0.0f),
	size(0.0f),
	color(0.0f),
	text(),
	fontSize(0),
	wordWrap(false),
	font(),
	glyphs(),
	vertices(nullptr),
	indices(nullptr)
{
	init();
}

TextBox::TextBox(std::shared_ptr<Font> f, int fontSize, std::string s, glm::vec2 pos, glm::vec2 size)
	:Drawable(DrawMode::SINGLETON),
	pos(pos),
	size(size),
	color(0.0f),
	text(s),
	fontSize(fontSize),
	wordWrap(false),
	font(f),
	glyphs(),
	vertices(nullptr),
	indices(nullptr)
{
	init();
}

TextBox::TextBox(const TextBox& other)
	:Drawable(DrawMode::SINGLETON),
	pos(other.pos),
	size(other.size),
	color(other.color),
	text(other.text),
	fontSize(other.fontSize),
	wordWrap(other.wordWrap),
	font(other.font),
	glyphs(),
	vertices(nullptr),
	indices(nullptr)
{
	init();
}

TextBox::~TextBox()
{
}

void TextBox::draw()
{
	glyphs.draw();
}

void TextBox::update()
{
}
