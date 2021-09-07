#pragma once
#include "Drawable.h"
#include <glm.hpp>
#include "Array2D.h"

class PixelArray : public Drawable
{
private:
	Array2D<glm::vec3> pixelArray;
	glm::vec2 pos;

	void init();
	void updateVertices();
public:
	PixelArray(DrawMode dm = DrawMode::SINGLETON, glm::vec2 pos = glm::vec2(0.0f), Array2D<glm::vec3>& pA = Array2D<glm::vec3>());
	PixelArray(const PixelArray& other);
	~PixelArray();

	void draw() override;
	void update() override;
	void updateTexture();

	static std::shared_ptr<VertexBufferLayout> getBufferLayout();
	static std::shared_ptr<Shader> getShader();
	static unsigned int getVertexCount() { return 4; };
	static unsigned int getVertexSize() { return 4; };
	static std::vector<unsigned int> getIndicesLayout() { return { 0, 1, 2, 2, 3, 0 }; };
	inline glm::vec2 getPos() { return pos; };
	inline float* getVertices() { return vertices; };

	void setPos(glm::vec2 pos) { this->pos = pos; };
	void setArray(Array2D <glm::vec3>& pA) { this->pixelArray = pA; };	
	void setColor(unsigned int x, unsigned int y, glm::vec3 color);
};