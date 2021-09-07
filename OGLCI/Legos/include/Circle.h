#pragma once
#include "Drawable.h"
#include <glm.hpp>

class Circle : public Drawable
{
private:
	glm::vec2 pos;	
	glm::vec3 color;
	float radius;

	void init();
	void updateVertices();
public:
	Circle(DrawMode dm = DrawMode::BUFFERED, glm::vec2 pos = glm::vec2(0.0f), float rad = 0.0f);
	Circle(const Circle& other);
	~Circle();

	void draw() override;
	void update() override;	
	void updateTexture();
	
	static std::shared_ptr<VertexBufferLayout> getBufferLayout();
	static std::shared_ptr<Shader> getShader();
	static unsigned int getVertexCount() { return 4; };
	static unsigned int getVertexSize() { return 6; };
	static std::vector<unsigned int> getIndicesLayout() { return { 0, 1, 2, 1, 2, 3 }; };

	void setPos(glm::vec2 pos) { this->pos = pos; };
	inline glm::vec2 getPos() { return pos; };

	void setColor(glm::vec3 color);
	inline glm::vec3 getColor() { return color; };

	void setRadius(float r) { this->radius = r; };
	inline float getRadius() { return radius; };
};