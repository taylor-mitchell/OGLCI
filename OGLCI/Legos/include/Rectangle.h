#pragma once
#include "Drawable.h"
#include <glm.hpp>

class Rectangle : public Drawable
{
private:
	glm::vec2 pos;
	glm::vec2 size;
	glm::vec2 texPos;
	glm::vec2 texSize;
	glm::vec3 color;

	void init();
	void updateVertices();
public:
	Rectangle(DrawMode dm = DrawMode::BUFFERED, glm::vec2 pos = glm::vec2(0.0f), glm::vec2 size = glm::vec2(0.0f));
	Rectangle(const Rectangle& s);	
	~Rectangle();

	Rectangle operator=(const Rectangle& s);

	void draw() override;
	void update() override;
	void updateTexture();

	static std::shared_ptr<VertexBufferLayout> getBufferLayout();
	static std::shared_ptr<Shader> getShader();
	static unsigned int getVertexCount() { return 4; };
	static unsigned int getVertexSize() { return 4; };
	static std::vector<unsigned int> getIndicesLayout() { return { 0, 1, 2, 1, 2, 3 }; };	
	
	inline float* getVertices() { return vertices; };

	void setPos(glm::vec2 pos) { this->pos = pos; updateVertices(); };
	inline glm::vec2 getPos() { return pos; };

	void setColor(glm::vec3 color);
	inline glm::vec3 getColor() { return color; };

	void setSize(glm::vec2 size) { this->size = size; updateVertices(); };
	inline glm::vec2 getSize() { return size; };

	void setTexturePosition(glm::vec2 texPos) { this->texPos = texPos; updateVertices(); };

	void setTextureSize(glm::vec2 texSize) { this->texSize = texSize; updateVertices(); };
};
