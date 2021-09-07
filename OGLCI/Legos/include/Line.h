#pragma once
#include "Drawable.h"
#include <glm.hpp>

class Line : public Drawable
{
private:
	float width;
	glm::vec2 pos1;
	glm::vec2 pos2;
	glm::vec3 color;

	void init();
	void updateVertices();

public:
	Line(glm::vec2 p1 = glm::vec2(0.0f), glm::vec2 p2 = glm::vec2(0.0f), float w = 0.0f, glm::vec3 c = glm::vec3(0.0f));
	Line(const Line& other);
	~Line();

	static std::shared_ptr<VertexBufferLayout> getBufferLayout();
	static std::shared_ptr<Shader> getShader();
	static unsigned int getVertexCount() { return 4; };
	static unsigned int getVertexSize() { return 5; };
	static std::vector<unsigned int> getIndicesLayout() { return { 0, 1, 2, 1, 2, 3 }; };
	inline float* getVertices() { return vertices; };

	void setPos1(glm::vec2 pos) { pos1 = pos; };
	glm::vec2 getPos1() { return pos1; };

	void setPos2(glm::vec2 pos) { pos2 = pos; };
	glm::vec2 getPos2() { return pos2; };

	void draw() override;
	void update() override;
};