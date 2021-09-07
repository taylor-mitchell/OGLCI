#include "Glyph.h"
#include "VertexBufferLayout.h"
#include "Logger.h"

void Glyph::init()
{
	shader = ShaderFactory::getInstance()->getShader("resources/Shaders/basic");
	vertices = new float[4 * 4];
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
	vb = new VertexBuffer(nullptr, 4 * 4 * sizeof(float));
	va = new VertexArray();

	VertexBufferLayout layout;
	layout.push<float>(2);
	layout.push<float>(2);
	va->addBuffer(*vb, layout);
}

void Glyph::updateVertices()
{
	if (dirtyTextureFlag)
	{
		updateTexture();
	}
	vertices[0] = pos.x;// +offset.x;
	vertices[1] = pos.y + offset.y;
	vertices[2] = texPos.x;
	vertices[3] = texPos.y;

	vertices[0 + 4] = pos.x;// +offset.x;
	vertices[1 + 4] = pos.y + size.y + offset.y;
	vertices[2 + 4] = texPos.x;
	vertices[3 + 4] = texPos.y + texSize.y;

	vertices[0 + 8] = pos.x + size.x;// +offset.x;
	vertices[1 + 8] = pos.y + offset.y;
	vertices[2 + 8] = texPos.x + texSize.x;
	vertices[3 + 8] = texPos.y;

	vertices[0 + 12] = pos.x + size.x;// +offset.x;
	vertices[1 + 12] = pos.y + size.y + offset.y;
	vertices[2 + 12] = texPos.x + texSize.x;
	vertices[3 + 12] = texPos.y + texSize.y;
}

Glyph::Glyph(DrawMode dm, glm::vec2 pos, glm::vec2 size, glm::vec3 color)
	: Drawable(dm),
	info(),
	pos(pos),
	size(size),
	color(color)
{
	init();
};

Glyph::Glyph(const Glyph& s)
	: Drawable(s.drawMode),
	info(s.info),
	pos(s.pos),
	size(s.size),
	color(s.color)
{
	texture = s.texture;
	init();
}

Glyph::~Glyph()
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

Glyph Glyph::operator=(const Glyph& s)
{
	drawMode = s.drawMode;
	pos = s.pos;
	color = s.color;
	isSolidColor = s.isSolidColor;
	dirtyTextureFlag = s.dirtyTextureFlag;
	info = s.info;
	texture = s.texture;
	size = s.size;

	init();
	return *this;
}

void Glyph::draw()
{
	vb->bind();
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * 4 * sizeof(float), vertices));
	RendererFactory::getInstance()->getRenderer(rendererId)->draw(*va, *ib, *shader, *texture);
}

void Glyph::update()
{
	updateVertices();
}

void Glyph::updateTexture()
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

std::shared_ptr<VertexBufferLayout> Glyph::getBufferLayout()
{
	std::shared_ptr<VertexBufferLayout> layout(std::make_shared<VertexBufferLayout>());
	layout->push<float>(2);
	layout->push<float>(2);
	return layout;
}

std::shared_ptr<Shader> Glyph::getShader()
{
	return ShaderFactory::getInstance()->getShader("resources/Shaders/basic");
}