#include "Circle.h"
#include "VertexBufferLayout.h"
#include <glm.hpp>
#include "Logger.h"

void Circle::init()
{
	shader = ShaderFactory::getInstance()->getShader("resources/Shaders/circle");
	
	updateVertices();

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;

	ib = new IndexBuffer(indices, 6);
	vb = new VertexBuffer(nullptr, 4 * 7 * sizeof(float));
	va = new VertexArray();

	VertexBufferLayout layout;
	layout.push<float>(2);
	layout.push<float>(2);
	layout.push<float>(3);
	va->addBuffer(*vb, layout);
}

void Circle::updateVertices()
{
	vertices[0] = x - r;
	vertices[1] = y - r;
	vertices[2] = -1.0f;
	vertices[3] = -1.0f;
	vertices[4] = red;
	vertices[5] = green;
	vertices[6] = blue;

	vertices[0 + 7] = x - r;
	vertices[1 + 7] = y + r;
	vertices[2 + 7] = -1.0f;
	vertices[3 + 7] = 1.0f;
	vertices[4 + 7] = red;
	vertices[5 + 7] = green;
	vertices[6 + 7] = blue;

	vertices[0 + 14] = x + r;
	vertices[1 + 14] = y - r;
	vertices[2 + 14] = 1.0f;
	vertices[3 + 14] = -1.0f;
	vertices[4 + 14] = red;
	vertices[5 + 14] = green;
	vertices[6 + 14] = blue;

	vertices[0 + 21] = x + r;
	vertices[1 + 21] = y + r;
	vertices[2 + 21] = 1.0f;
	vertices[3 + 21] = 1.0f;
	vertices[4 + 21] = red;
	vertices[5 + 21] = green;
	vertices[6 + 21] = blue;
}

Circle::Circle() 
	: Drawable(), 
	x(0), 
	y(0), 
	r(0), 
	red(1.0f), 
	green(1.0f), 
	blue(1.0f),
	dx(0),
	dy(0)
{
	if (drawMode == DrawMode::SINGLETON)
	{
		init();
	}
		
}

Circle::Circle(float x, float y, float r) 
	: Drawable(), 
	x(x), 
	y(y), 
	r(r),
	red(1.0f),
	green(1.0f),
	blue(1.0f),
	dx(0),
	dy(0)
{
	if (drawMode == DrawMode::SINGLETON)
	{
		init();
	}
}

Circle::Circle(float x, float y, float r, float red, float green, float blue)
	: Drawable(),
	x(x),
	y(y),
	r(r),
	red(red),
	green(green),
	blue(blue),
	dx(0),
	dy(0)
{
	if (drawMode == DrawMode::SINGLETON)
	{
		init();
	}
}

Circle::~Circle()
{
	if (va)
	{
		delete(va);
	}
	if (vb)
	{
		delete(vb);
	}
	if (ib)
	{
		delete(ib);
	}	
}

void Circle::draw(Renderer& renderer)
{
	vb->bind();
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * 7 * sizeof(float), vertices));
	renderer.draw(*va, *ib, *shader);
}

void Circle::update()
{
	x += dx;
	y += dy;
	
	updateVertices();
}

std::shared_ptr<VertexBufferLayout> Circle::getBufferLayout()
{
	std::shared_ptr<VertexBufferLayout> layout(std::make_shared<VertexBufferLayout>());
	layout->push<float>(2);
	layout->push<float>(2);
	layout->push<float>(3);
	return layout;
}

std::shared_ptr<Shader> Circle::getShader()
{
	return ShaderFactory::getInstance()->getShader("resources/Shaders/circle");
}

void Circle::gravitate(float x2, float y2, float g, float m2)
{
	float acc = g * m2 / glm::dot(glm::vec2(x, y), glm::vec2(x2, y2));
	float angle = std::atan2(y2 - y, x2 - x);

	dx += std::cos(angle) * acc;
	dy += std::sin(angle) * acc;
}
