#pragma once
#include "Drawable.h"
#include <glm.hpp>
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

	float* vertices;
	unsigned int* indices;

	void init();

public:
	Grid() 
		:width(0), height(0), lineWidth(0.0f), cellWidth(0.0f), cellHeight(0.0f), pos(0.0f), color(0.0f), vertices(nullptr), indices(nullptr) {};
	Grid(int width, int height, float cellWidth, float cellHeight);
		

	void draw(Renderer& renderer) override;
	void update() override {};
};