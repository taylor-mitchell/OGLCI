#pragma once
#include "Drawable.h"

class Circle : Drawable
{
private:
	float x;
	float y;
	float r;
	float red;
	float green;
	float blue;

	float dx;
	float dy;

	float vertices[7 * 4];
	unsigned int indices[6];

	void init();
	void updateVertices();
public:
	Circle();
	Circle(float x, float y, float r);
	Circle(float x, float y, float r, float red, float green, float blue);
	~Circle();
	void draw(Renderer& renderer) override;
	void update() override;
	static unsigned int getVertexCount() { return 4; };
	static unsigned int getVertexSize() { return 7; };
	static std::vector<unsigned int> getIndicesLayout() { return { 0, 1, 2, 1, 2, 3 }; };
	static std::shared_ptr<VertexBufferLayout> getBufferLayout();
	static std::shared_ptr<Shader> getShader();

	inline float getX() { return x; };
	inline float getY() { return y; };
	inline float getR() { return r; };
	inline float getRed() { return red; };
	inline float getGreen() { return green; };
	inline float getBlue() { return blue; };
	inline float getDX() { return dx; };
	inline float getDY() { return dy; };

	inline float* getVertices() { return vertices; };


	void setX(float x) { this->x = x; };
	void setY(float y) { this->y = y; };
	void setR(float r) { this->r = r; };
	void setRed(float red) { this->red = red; };
	void setGreen(float green) { this->green = green; };
	void setBlue(float blue) { this->blue = blue; };

	void setDX(float dx) { this->dx = dx; };
	void setDY(float dy) { this->dy = dy; };
	
	void gravitate(float x, float y, float g, float m2);
};