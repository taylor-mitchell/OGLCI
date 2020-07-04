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
	vertices[0] = pos.x - radius;
	vertices[1] = pos.y - radius;
	vertices[2] = -1.0f;
	vertices[3] = -1.0f;
	vertices[4] = color.x;
	vertices[5] = color.y;
	vertices[6] = color.z;

	vertices[0 + 7] = pos.x - radius;
	vertices[1 + 7] = pos.y + radius;
	vertices[2 + 7] = -1.0f;
	vertices[3 + 7] = 1.0f;
	vertices[4 + 7] = color.x;
	vertices[5 + 7] = color.y;
	vertices[6 + 7] = color.z;

	vertices[0 + 14] = pos.x + radius;
	vertices[1 + 14] = pos.y - radius;
	vertices[2 + 14] = 1.0f;
	vertices[3 + 14] = -1.0f;
	vertices[4 + 14] = color.x;
	vertices[5 + 14] = color.y;
	vertices[6 + 14] = color.z;

	vertices[0 + 21] = pos.x + radius;
	vertices[1 + 21] = pos.y + radius;
	vertices[2 + 21] = 1.0f;
	vertices[3 + 21] = 1.0f;
	vertices[4 + 21] = color.x;
	vertices[5 + 21] = color.y;
	vertices[6 + 21] = color.z;
}

Circle::Circle()
	: Drawable(),
	pos(0.0f),
	radius(0.0f),
	color(0.0f),
	velocity(0.0f),
	indices(),
	vertices()
{
	if (drawMode == DrawMode::SINGLETON)
	{
		init();
	}
		
}

Circle::Circle(glm::vec2 pos, float r) 
	: Drawable(),
	pos(pos),
	radius(r),
	color(1.0f),
	velocity(0.0f),
	indices(),
	vertices()
{
	if (drawMode == DrawMode::SINGLETON)
	{
		init();
	}
}

Circle::Circle(glm::vec2 pos, float r, glm::vec3 color)
	: Drawable(),
	pos(pos),
	radius(r),
	color(color),
	velocity(0.0f),
	indices(),
	vertices()
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
	if (vertices)
	{
		delete[] vertices;
	}
	if (indices)
	{
		delete[] indices;
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
	pos += velocity;	
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

void Circle::gravitateTowards(glm::vec2 pos2, float g, float m2)
{
	float deltaX = pos2.x - pos.x;
	float deltaY = pos2.y - pos.y;
	float acc = g * m2 / glm::dot(glm::vec2(deltaX, deltaY), glm::vec2(deltaX, deltaY));
	float angle = std::atan2(deltaY, deltaX);

	velocity.x += std::cos(angle) * acc;
	velocity.y += std::sin(angle) * acc;
}
