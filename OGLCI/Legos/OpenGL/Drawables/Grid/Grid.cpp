#include "Grid.h"
#include "VertexBufferLayout.h"

void Grid::init()
{
	shader = ShaderFactory::getInstance()->getShader("resources/Shaders/texture");	

	vertices = new float[(((width + 1) + (height + 1)) * 4) * 5]();
	indices = new unsigned int[6 * ((width + 1) + (height + 1))]();

	updateVertices();

	for (int i = 0; i < (height + 1 + width + 1); ++i)
	{
		indices[i * 6] = i * 4;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;

		indices[i * 6 + 3] = i * 4 + 1;
		indices[i * 6 + 4] = i * 4 + 2;
		indices[i * 6 + 5] = i * 4 + 3;
	}

	if (drawMode == DrawMode::SINGLETON)
	{
		ib = new IndexBuffer(indices, 6 * (height + 1 + width + 1));
		vb = new VertexBuffer(vertices, (((width + 1) + (height + 1)) * 4) * 5 * sizeof(float), false);
		va = new VertexArray();

		VertexBufferLayout layout;
		layout.push<float>(2);
		layout.push<float>(3);
		va->addBuffer(*vb, layout);
	}
}

void Grid::updateVertices()
{
	for (int i = 0; i < (width + 1); ++i)
	{
		vertices[i * 20] = pos.x + i * cellWidth;
		vertices[i * 20 + 1] = pos.y;
		vertices[i * 20 + 2] = color.x;
		vertices[i * 20 + 3] = color.y;
		vertices[i * 20 + 4] = color.z;

		vertices[i * 20 + 5] = pos.x + i * cellWidth;
		vertices[i * 20 + 6] = pos.y + height * cellHeight;
		vertices[i * 20 + 7] = color.x;
		vertices[i * 20 + 8] = color.y;
		vertices[i * 20 + 9] = color.z;

		vertices[i * 20 + 10] = pos.x + i * cellWidth + lineWidth;
		vertices[i * 20 + 11] = pos.y;
		vertices[i * 20 + 12] = color.x;
		vertices[i * 20 + 13] = color.y;
		vertices[i * 20 + 14] = color.z;

		vertices[i * 20 + 15] = pos.x + i * cellWidth + lineWidth;
		vertices[i * 20 + 16] = pos.y + height * cellHeight;
		vertices[i * 20 + 17] = color.x;
		vertices[i * 20 + 18] = color.y;
		vertices[i * 20 + 19] = color.z;
	}

	int start = (width + 1);
	for (int i = 0; i < height + 1; ++i)
	{
		vertices[(i + start) * 20] = pos.x;
		vertices[(i + start) * 20 + 1] = pos.y + i * cellHeight;
		vertices[(i + start) * 20 + 2] = color.x;
		vertices[(i + start) * 20 + 3] = color.y;
		vertices[(i + start) * 20 + 4] = color.z;

		vertices[(i + start) * 20 + 5] = pos.x;
		vertices[(i + start) * 20 + 6] = pos.y + i * cellHeight + lineWidth;
		vertices[(i + start) * 20 + 7] = color.x;
		vertices[(i + start) * 20 + 8] = color.y;
		vertices[(i + start) * 20 + 9] = color.z;

		vertices[(i + start) * 20 + 10] = pos.x + width * cellWidth;
		vertices[(i + start) * 20 + 11] = pos.y + i * cellHeight;
		vertices[(i + start) * 20 + 12] = color.x;
		vertices[(i + start) * 20 + 13] = color.y;
		vertices[(i + start) * 20 + 14] = color.z;

		vertices[(i + start) * 20 + 15] = pos.x + width * cellWidth;
		vertices[(i + start) * 20 + 16] = pos.y + i * cellHeight + lineWidth;
		vertices[(i + start) * 20 + 17] = color.x;
		vertices[(i + start) * 20 + 18] = color.y;
		vertices[(i + start) * 20 + 19] = color.z;
	}
}

Grid::Grid(int width, int height, float cellWidth, float cellHeight)
	:Drawable(DrawMode::SINGLETON),
	width(width), 
	height(height), 
	lineWidth(1.0f), 
	cellWidth(cellWidth), 
	cellHeight(cellHeight), 
	pos(0.0f), 
	color(1.0f)
{
	init();
}

Grid::Grid(const Grid& other)
	:Drawable(DrawMode::SINGLETON),
	width(other.width),
	height(other.height),
	lineWidth(other.lineWidth),
	cellWidth(other.cellWidth),
	cellHeight(other.cellHeight),
	pos(other.pos),
	color(other.color)
{
	init();
}

void Grid::draw()
{
	RendererFactory::getInstance()->getRenderer(rendererId)->draw(*va, *ib, *shader);
}
