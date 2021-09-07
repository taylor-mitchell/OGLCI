#pragma once
#include "Drawable.h"
#include "Font.h"
#include "BatchBuffer.h"
#include "Glyph.h"
#include <glm.hpp>
#include <string>
#include <memory>

class TextBox : public Drawable
{
private:
	glm::vec2 pos;
	glm::vec2 size;
	glm::vec3 color;
	std::string text;
	int fontSize;
	bool wordWrap;
	std::shared_ptr<Font> font;
	BatchBuffer<Glyph> glyphs;

	float* vertices;
	unsigned int* indices;

	void init();
	void updateVertices();
public:
	TextBox();
	TextBox(std::shared_ptr<Font> f, int fontSize, std::string s = "", glm::vec2 pos = glm::vec2(0.0f), glm::vec2 size = glm::vec2(0.0f));
	TextBox(const TextBox& other);
	~TextBox();

	void draw() override;
	void update() override;

	void setText(std::string t) { this->text = t; };
	std::string getText() { return text; };

	void setFontSize(int fontSize) { this->fontSize = fontSize; };
	int getFontSize() { return fontSize; };

	void setFont(std::shared_ptr<Font> font) { this->font = font; };
	std::shared_ptr<Font> getFont() { return font; };

};