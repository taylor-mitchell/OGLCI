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

	float vertices[4 * 5];
	unsigned int indices[6];

	void init();
	void updateVertices();

public:

	Line();
	Line(glm::vec2 p1, glm::vec2 p2);
	Line(glm::vec2 p1, glm::vec2 p2, float w);
	Line(glm::vec2 p1, glm::vec2 p2, float w, glm::vec3 c);
	~Line();

	static std::shared_ptr<VertexBufferLayout> getBufferLayout();
	static std::shared_ptr<Shader> getShader();
	static unsigned int getVertexCount() { return 4; };
	static unsigned int getVertexSize() { return 5; };
	static std::vector<unsigned int> getIndicesLayout() { return { 0, 1, 2, 1, 2, 3 }; };
	inline float* getVertices() { return vertices; };

	void setPos1(glm::vec2 pos) { pos1 = pos; };
	void setPos2(glm::vec2 pos) { pos2 = pos; };


	void draw(Renderer& renderer) override;
	void update() override;
};