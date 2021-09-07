#pragma once
#include "Drawable.h"
#include <glm.hpp>

//TODO: Grid wont work with batchBuffer yet because of the variable vertex count 
class Grid : public Drawable
{
private:
	int width;
	int height;
	float lineWidth;
	float cellWidth;
	float cellHeight;
	glm::vec3 pos;
	glm::vec3 color;

	void init();
	void updateVertices();

public:
	Grid(int width = 0, int height = 0, float cellWidth = 0, float cellHeight = 0);		
	Grid(const Grid& other);

	void draw() override;
	void update() override {};

	void setPos(glm::vec3 pos) { this->pos = pos; updateVertices(); };
	glm::vec3 getPos() { return pos; };
};