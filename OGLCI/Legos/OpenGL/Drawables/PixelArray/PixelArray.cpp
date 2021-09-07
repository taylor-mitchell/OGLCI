#include "PixelArray.h"
#include "VertexBufferLayout.h"
#include "Logger.h"

void PixelArray::init()
{
	shader = ShaderFactory::getInstance()->getShader("resources/Shaders/basic");
	texture = std::make_shared<Texture>(pixelArray.getWidth(), pixelArray.getHeight());
	vertices = new float[4 * 4];
	indices = new unsigned int[6];
	
	updateVertices();

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 0;

	ib = new IndexBuffer(indices, 6);
	vb = new VertexBuffer(vertices, 4 * 4 * sizeof(float));
	va = new VertexArray();

	VertexBufferLayout layout;
	layout.push<float>(2);
	layout.push<float>(2);
	va->addBuffer(*vb, layout);
}

void PixelArray::updateVertices()
{
	if (dirtyTextureFlag)
	{
		updateTexture();
	}
	vertices[0] = pos.x;
	vertices[1] = pos.y;
	vertices[2] = 0.0f;
	vertices[3] = 0.0f;

	vertices[0 + 4] = pos.x + pixelArray.getWidth();
	vertices[1 + 4] = pos.y;
	vertices[2 + 4] = 1.0f;
	vertices[3 + 4] = 0.0f;

	vertices[0 + 8] = pos.x + pixelArray.getWidth();
	vertices[1 + 8] = pos.y + pixelArray.getHeight();
	vertices[2 + 8] = 1.0f;
	vertices[3 + 8] = 1.0f;

	vertices[0 + 12] = pos.x;
	vertices[1 + 12] = pos.y + pixelArray.getHeight();
	vertices[2 + 12] = 0.0f;
	vertices[3 + 12] = 1.0f;
}

PixelArray::PixelArray(DrawMode dm, glm::vec2 pos, Array2D<glm::vec3>& pA)
	: Drawable(dm),
	pos(pos),
	pixelArray(pA)
{
	if (drawMode == DrawMode::SINGLETON)
	{
		init();
	}
}

PixelArray::PixelArray(const PixelArray& pA)
	: Drawable(pA.drawMode),
	pos(pA.pos),
	pixelArray(pA.pixelArray)
{
	if (drawMode == DrawMode::SINGLETON)
	{
		init();
	}
}



PixelArray::~PixelArray()
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

void PixelArray::draw()
{
	RendererFactory::getInstance()->getRenderer(rendererId)->draw(*va, *ib, *shader, *texture);
}

void PixelArray::update()
{
	updateVertices();
}

void PixelArray::updateTexture()
{
	RendererFactory::getInstance()->getRenderer(rendererId)->renderToTexture(*texture, pixelArray);
	dirtyTextureFlag = false;
}

std::shared_ptr<VertexBufferLayout> PixelArray::getBufferLayout()
{
	std::shared_ptr<VertexBufferLayout> layout(std::make_shared<VertexBufferLayout>());
	layout->push<float>(2);
	layout->push<float>(2);
	return layout;
}

std::shared_ptr<Shader> PixelArray::getShader()
{
	return ShaderFactory::getInstance()->getShader("resources/Shaders/texture");
}

void PixelArray::setColor(unsigned int x, unsigned int y, glm::vec3 color)
{
	dirtyTextureFlag = true;
	pixelArray(x, y) = color;
}
