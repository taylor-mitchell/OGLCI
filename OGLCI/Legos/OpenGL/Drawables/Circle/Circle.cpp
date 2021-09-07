#include "Circle.h"
#include "VertexBufferLayout.h"
#include "Logger.h"

void Circle::init()
{
	shader = ShaderFactory::getInstance()->getShader("resources/Shaders/circle");
	vertices = new float[6 * 4];
	indices = new unsigned int[6];

	if (isSolidColor)
	{
		texture = std::make_shared<Texture>(1, 1);
		dirtyTextureFlag = true;
	}
	else
	{
		dirtyTextureFlag = false;
	}

	updateVertices();

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;

	ib = new IndexBuffer(indices, 6);
	vb = new VertexBuffer(nullptr, 4 * 6 * sizeof(float));
	va = new VertexArray();

	VertexBufferLayout layout;
	layout.push<float>(2);
	layout.push<float>(2);
	layout.push<float>(2);
	va->addBuffer(*vb, layout);
}

void Circle::updateVertices()
{
	if (dirtyTextureFlag && isSolidColor)
	{
		updateTexture();
	}

	vertices[0] = pos.x - radius;
	vertices[1] = pos.y - radius;
	vertices[2] = -1.0f;
	vertices[3] = -1.0f;
	vertices[4] = 0.0f;
	vertices[5] = 0.0f;

	vertices[0 + 6] = pos.x - radius;
	vertices[1 + 6] = pos.y + radius;
	vertices[2 + 6] = -1.0f;
	vertices[3 + 6] = 1.0f;
	vertices[4 + 6] = 0.0f;
	vertices[5 + 6] = 1.0f;

	vertices[0 + 12] = pos.x + radius;
	vertices[1 + 12] = pos.y - radius;
	vertices[2 + 12] = 1.0f;
	vertices[3 + 12] = -1.0f;
	vertices[4 + 12] = 1.0f;
	vertices[5 + 12] = 0.0f;

	vertices[0 + 18] = pos.x + radius;
	vertices[1 + 18] = pos.y + radius;
	vertices[2 + 18] = 1.0f;
	vertices[3 + 18] = 1.0f;
	vertices[4 + 18] = 1.0f;
	vertices[5 + 18] = 1.0f;
}

Circle::Circle(DrawMode dm, glm::vec2 pos, float rad)
	: Drawable(dm, false),
	pos(pos),
	radius(rad),
	color(0.0f)
{
	if (drawMode == DrawMode::SINGLETON)
	{
		init();
	}
}

Circle::Circle(const Circle& other)
	: Drawable(other.drawMode),
	pos(other.pos),
	radius(other.radius),
	color(other.color)
{
	if (drawMode == DrawMode::SINGLETON)
	{
		init();
	}
}

Circle::~Circle()
{
	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}

	if (indices)
	{
		delete[] indices;
		indices = 0;
	}

	if (va)
	{
		delete va;
		va = 0;
	}

	if (vb)
	{
		delete vb;
		vb = 0;
	}

	if (ib)
	{
		delete ib;
		ib = 0;
	}	
}

void Circle::draw()
{
	vb->bind();
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * 6 * sizeof(float), vertices));
	RendererFactory::getInstance()->getRenderer(rendererId)->draw(*va, *ib, *shader, *texture);
}

void Circle::update()
{
	updateVertices();
}

void Circle::updateTexture()
{
	unsigned int texIndices[6];
	texIndices[0] = 0;
	texIndices[1] = 1;
	texIndices[2] = 2;

	texIndices[3] = 1;
	texIndices[4] = 2;
	texIndices[5] = 3;

	float texVertices[4 * 5];

	texVertices[0] = 0.0f;
	texVertices[1] = 0.0f;
	texVertices[2] = color.x;
	texVertices[3] = color.y;
	texVertices[4] = color.z;

	texVertices[0 + 5] = 0.0f;
	texVertices[1 + 5] = 1.0f;
	texVertices[2 + 5] = color.x;
	texVertices[3 + 5] = color.y;
	texVertices[4 + 5] = color.z;

	texVertices[0 + 10] = 1.0f;
	texVertices[1 + 10] = 0.0f;
	texVertices[2 + 10] = color.x;
	texVertices[3 + 10] = color.y;
	texVertices[4 + 10] = color.z;

	texVertices[0 + 15] = 1.0f;
	texVertices[1 + 15] = 1.0f;
	texVertices[2 + 15] = color.x;
	texVertices[3 + 15] = color.y;
	texVertices[4 + 15] = color.z;

	IndexBuffer indexBuf(texIndices, 6);

	VertexBuffer texVB(texVertices, 4 * 5 * sizeof(float));
	VertexArray texVA;

	VertexBufferLayout layout;
	layout.push<float>(2);
	layout.push<float>(3);
	texVA.addBuffer(texVB, layout);
	RendererFactory::getInstance()->getRenderer(rendererId)->renderToTexture(*texture, texVA, indexBuf);
	dirtyTextureFlag = false;
}

std::shared_ptr<VertexBufferLayout> Circle::getBufferLayout()
{
	std::shared_ptr<VertexBufferLayout> layout(std::make_shared<VertexBufferLayout>());
	layout->push<float>(2);
	layout->push<float>(2);
	layout->push<float>(2);
	return layout;
}

std::shared_ptr<Shader> Circle::getShader()
{
	return ShaderFactory::getInstance()->getShader("resources/Shaders/circle");
}

void Circle::setColor(glm::vec3 col)
{
	color = col; 
	dirtyTextureFlag = true; 
	isSolidColor = true; 
	texture = std::make_shared<Texture>(1, 1); 
	updateTexture();
}

