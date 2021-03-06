#include "Line.h"

void Line::init()
{
	shader = ShaderFactory::getInstance()->getShader("resources/Shaders/basic");

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;

	ib = new IndexBuffer(indices, 6);
	vb = new VertexBuffer(nullptr, 4 * 5 * sizeof(float));
	va = new VertexArray();

	VertexBufferLayout layout;
	layout.push<float>(2);
	layout.push<float>(3);
	va->addBuffer(*vb, layout);
}

void Line::updateVertices()
{
	float deltaX; 
	float deltaY;
	float halfWidth = width / 2.0f;

	if ((pos1.x - pos2.x) == 0.0f && (pos1.y - pos2.y) == 0.0f)
	{
		deltaX = 0.0f;
		deltaY = 0.0f;
	}
	else if ((pos1.x - pos2.x) == 0.0f)
	{
		deltaX = halfWidth;
		deltaY = 0.0f;
	}
	else if ((pos1.y - pos2.y) == 0.0f)
	{
		deltaX = 0.0f;
		deltaY = halfWidth;
	}
	else
	{
		float dx = std::fabs(pos1.x - pos2.x);
		float dy = std::fabs(pos1.y - pos2.y);
		deltaX = std::sqrt(std::pow(halfWidth, 2) * std::pow(dy / dx, 2) / (1.0f + std::pow(dy / dx, 2)));
		deltaY = -1.0f * (pos1.x - pos2.x) / (pos1.y - pos2.y) * deltaX;
	}		

	vertices[0] = pos1.x - deltaX;
	vertices[1] = pos1.y - deltaY;
	vertices[2] = color.x;
	vertices[3] = color.y;
	vertices[4] = color.z;

	vertices[0 + 5] = pos1.x + deltaX;
	vertices[1 + 5] = pos1.y + deltaY;
	vertices[2 + 5] = color.x;
	vertices[3 + 5] = color.y;
	vertices[4 + 5] = color.z;

	vertices[0 + 10] = pos2.x - deltaX;
	vertices[1 + 10] = pos2.y - deltaY;
	vertices[2 + 10] = color.x;
	vertices[3 + 10] = color.y;
	vertices[4 + 10] = color.z;

	vertices[0 + 15] = pos2.x + deltaX;
	vertices[1 + 15] = pos2.y + deltaY;
	vertices[2 + 15] = color.x;
	vertices[3 + 15] = color.y;
	vertices[4 + 15] = color.z;
}

Line::Line()
	:width(1.0f),
	pos1(0.0f, 0.0f),
	pos2(0.0f, 0.0f),
	color(1.0f, 1.0f, 1.0f),
	vertices(),
	indices()
{
	if (drawMode == DrawMode::SINGLETON)
	{
		init();
	}
	updateVertices();
}

Line::Line(glm::vec2 p1, glm::vec2 p2)
	:width(1.0f),
	pos1(p1),
	pos2(p2),
	color(1.0f, 1.0f, 1.0f),
	vertices(),
	indices()
{
	if (drawMode == DrawMode::SINGLETON)
	{
		init();
	}
	updateVertices();
}

Line::Line(glm::vec2 p1, glm::vec2 p2, float w)
	:width(w),
	pos1(p1),
	pos2(p2),
	color(1.0f, 1.0f, 1.0f),
	vertices(),
	indices()
{
	if (drawMode == DrawMode::SINGLETON)
	{
		init();
	}
	updateVertices();
}

Line::Line(glm::vec2 p1, glm::vec2 p2, float w, glm::vec3 c)
	:width(w),
	pos1(p1),
	pos2(p2),
	color(c),
	vertices(),
	indices()
{
	if (drawMode == DrawMode::SINGLETON)
	{
		init();
	}
	updateVertices();
}

Line::~Line()
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

std::shared_ptr<VertexBufferLayout> Line::getBufferLayout()
{
	std::shared_ptr<VertexBufferLayout> layout(std::make_shared<VertexBufferLayout>());
	layout->push<float>(2);
	layout->push<float>(3);
	return layout;
}

std::shared_ptr<Shader> Line::getShader()
{
	return ShaderFactory::getInstance()->getShader("resources/Shaders/basic");
}

void Line::draw(Renderer& renderer)
{
	vb->bind();
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * 5 * sizeof(float), vertices));
	renderer.draw(*va, *ib, *shader);
}

void Line::update()
{
	updateVertices();
}
