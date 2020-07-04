#pragma once
#include "Drawable.h"
#include <glm.hpp>

class Circle : public Drawable
{
private:
	glm::vec2 pos;	
	glm::vec2 oldPos;
	glm::vec3 color;
	glm::vec2 velocity;
	glm::mat2 affineVelocity;
	float density;
	float radius;

	float vertices[7 * 4];
	unsigned int indices[6];

	void init();
	void updateVertices();
public:
	Circle();
	Circle(glm::vec2 pos, float r);
	Circle(glm::vec2 pos, float r, glm::vec3 color);
	~Circle();

	void draw(Renderer& renderer) override;
	void update() override;	
	
	static std::shared_ptr<VertexBufferLayout> getBufferLayout();
	static std::shared_ptr<Shader> getShader();
	static unsigned int getVertexCount() { return 4; };
	static unsigned int getVertexSize() { return 7; };
	static std::vector<unsigned int> getIndicesLayout() { return { 0, 1, 2, 1, 2, 3 }; };
	inline glm::vec2 getPos() { return pos; };
	inline glm::vec2 getOldPos() { return oldPos; };
	inline glm::vec3 getColor() { return color; };
	inline glm::vec2 getVelocity() { return velocity; };
	inline glm::mat2 getAffineVelocity() { return affineVelocity; };
	inline float getRadius() { return radius; };
	inline float getDensity() { return density; };
	inline float* getVertices() { return vertices; };

	void setPos(glm::vec2 pos) { this->pos = pos; };
	void setOldPos(glm::vec2 oldPos) { this->oldPos = oldPos; };
	void setRadius(float r) { this->radius = r; };
	void setColor(glm::vec3 color) { this->color = color; };
	void setAffineVelocity(glm::mat2 affVel) { this->affineVelocity = affVel; };
	void setVelocity(glm::vec2 velocity) { this->velocity = velocity; };
	void setDensity(float density) { this->density = density; };
	
	void gravitateTowards(glm::vec2 pos, float g, float m2);
};