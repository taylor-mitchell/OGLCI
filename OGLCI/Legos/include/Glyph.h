#pragma once
#include "Drawable.h"

struct GlyphInfo
{
public:
	glm::vec2 offset;
	glm::vec2 size;
	int kerning;
	float aspectRatio;
	float heightRatio;
	int advancement;
};

class Glyph: public Drawable
{
private:
	GlyphInfo info;
	glm::vec2 pos;
	glm::vec2 size;
	glm::vec3 color;
	glm::vec2 texPos;
	glm::vec2 texSize;
	glm::vec2 offset;
	int advancement;

	void init();
	void updateVertices();
public:
	Glyph(DrawMode dm = DrawMode::BUFFERED, glm::vec2 pos = glm::vec2(0.0f), glm::vec2 size = glm::vec2(0.0f), glm::vec3 color = glm::vec3(0.0f));
	Glyph(const Glyph& s);
	~Glyph();

	Glyph operator=(const Glyph& s);

	void draw() override;
	void update() override;
	void updateTexture();

	static std::shared_ptr<VertexBufferLayout> getBufferLayout();
	static std::shared_ptr<Shader> getShader();
	static unsigned int getVertexCount() { return 4; };
	static unsigned int getVertexSize() { return 4; };
	static std::vector<unsigned int> getIndicesLayout() { return { 0, 1, 2, 1, 2, 3 }; };

	inline float* getVertices() { return vertices; };

	void setGlyphInfo(GlyphInfo info) { this->info = info; };
	GlyphInfo getGlyphInfo() { return info; };

	void setPos(glm::vec2 pos) { this->pos = pos; updateVertices(); };
	inline glm::vec2 getPos() { return pos; };

	void setColor(glm::vec3 color) { this->color = color; dirtyTextureFlag = true; updateTexture(); };
	inline glm::vec3 getColor() { return color; };

	void setSize(glm::vec2 size) { this->size = size; updateVertices(); };
	inline glm::vec2 getSize() { return size; };

	void setAdvancement(int advancement) { this->advancement = advancement;};
	int getAdvancement() { return advancement + offset.x; };

	void setTexturePosition(glm::vec2 texPos) { this->texPos = texPos; };
	glm::vec2 getTexturePosition() { return texPos; };

	void setTextureSize(glm::vec2 texSize) { this->texSize = texSize; updateVertices(); };
	glm::vec2 getTextureSize() { return texSize; };

	void setOffset(glm::vec2 offset) { this->offset = offset; updateVertices(); }
	glm::vec2 getOffset() { return offset; };
};