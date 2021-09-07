#pragma once
#include "Shader.h"
#include "Drawable.h"
#include "RendererFactory.h"
#include "Logger.h"
#include "Texture.h"

//TODO: add debug logging of vertex/index buffers
//TODO: clean up

template <class T>
class BatchBuffer
{
	static_assert(std::is_base_of<Drawable, T>::value);
private:
	unsigned int size;
	unsigned int verticesSize;
	unsigned int indicesSize;
	unsigned int cursor;
	std::shared_ptr<Shader> shader;
    std::unique_ptr<VertexArray> va;
	std::unique_ptr <VertexBuffer> vb;
	std::unique_ptr <IndexBuffer> ib;
	std::vector<unsigned int> indicesLayout;
	int rendererId;
	std::shared_ptr<Texture> texture;

	float* vertices;	
	unsigned int* indices;

	void resetLocalBuffers();

public:
	BatchBuffer();
	BatchBuffer(unsigned int size);
	BatchBuffer(const BatchBuffer<T>& rhs);
	~BatchBuffer();

	int addToBuffer(T& obj);
	bool updateObject(unsigned int, T& obj);
	void updateBuffer();
	void draw();
	void draw(std::shared_ptr<Texture> texture);

	void setRendererId(int rId) { rendererId = rId; };
	int getRendererId() { return rendererId; };

	void setTexture(std::string fileName) { texture = TextureFactory::getInstance()->getTexture(fileName); };
	void setTexture(std::shared_ptr<Texture> tex) { texture = tex; };
	std::shared_ptr<Texture> getTexture() { return texture; };
};

template<class T>
inline void BatchBuffer<T>::resetLocalBuffers()
{
	if (vertices)
	{
		delete vertices;
	}
	if (indices)
	{
		delete indices;
	}

	vertices = new float[T::getVertexCount() * T::getVertexSize() * size];
	indices = new unsigned int[T::getIndicesLayout().size() * size];
	cursor = 0;
}

template<class T>
BatchBuffer<T>::BatchBuffer()
	:size(1),
	verticesSize(T::getVertexCount() * T::getVertexSize() * sizeof(float)),
	indicesSize(static_cast<unsigned int>(T::getIndicesLayout().size())),
	cursor(0),
	shader(T::getShader()),
	va(std::make_unique<VertexArray>()),
	vb(std::make_unique<VertexBuffer>(nullptr, verticesSize, true)),
	ib(std::make_unique<IndexBuffer>(nullptr, static_cast<unsigned int>(T::getIndicesLayout().size()))),
	rendererId(0),
	texture(nullptr),
	vertices(nullptr),
	indices(nullptr)
{
	resetLocalBuffers();
	va->addBuffer(*vb, *T::getBufferLayout());
}

template<class T>
BatchBuffer<T>::BatchBuffer(unsigned int s)
	:size(s),
	verticesSize(T::getVertexCount()* T::getVertexSize()* s * sizeof(float)),
	indicesSize(static_cast<unsigned int>(T::getIndicesLayout().size()) * s),
	cursor(0),
	shader(T::getShader()),
	va(std::make_unique<VertexArray>()),
	vb(std::make_unique<VertexBuffer>(nullptr, verticesSize, true)),
	ib(std::make_unique<IndexBuffer>(nullptr, static_cast<unsigned int>(T::getIndicesLayout().size()) * s)),
	rendererId(0),
	texture(nullptr),
	vertices(nullptr),
	indices(nullptr)
{
	resetLocalBuffers();
	va->addBuffer(*vb, *T::getBufferLayout());
}

template<class T>
BatchBuffer<T>::BatchBuffer(const BatchBuffer<T>& rhs)
	:size(rhs.size),
	verticesSize(rhs.verticesSize),
	indicesSize(rhs.indicesSize),
	cursor(rhs.cursor),
	shader(T::getShader()),
	va(std::make_unique<VertexArray>()),
	vb(std::make_unique<VertexBuffer>(nullptr, verticesSize, true)),
	ib(std::make_unique<IndexBuffer>(nullptr, static_cast<unsigned int>(T::getIndicesLayout().size())* size)),
	rendererId(rhs.rendererId),
	texture(rhs.texture),
	vertices(nullptr),
	indices(nullptr)
{
	resetLocalBuffers();
	memcpy(vertices, rhs.vertices, rhs.cursor * T::getVertexSize() * T::getVertexCount() * sizeof(float));
	memcpy(indices, rhs.indices, rhs.cursor * T::getIndicesLayout().size() * sizeof(unsigned int));
	va->addBuffer(*vb, *T::getBufferLayout());
}

template<class T>
inline BatchBuffer<T>::~BatchBuffer()
{
	if (vertices)
	{
		delete vertices;
	}

	if (indices)
	{
		delete indices;
	}
}

template<class T>
int BatchBuffer<T>::addToBuffer(T& obj)
{
	static_assert(std::is_base_of<Drawable, T>::value);

	int toReturn = -1;
	if (!vertices)
	{
		resetLocalBuffers();
	}

	if (cursor >= size)
	{
		size *= 2;

		float* tempV = new float[T::getVertexCount() * T::getVertexSize() * size];
		if (tempV != nullptr)
		{
			memcpy(tempV, vertices, T::getVertexCount() * T::getVertexSize() * cursor * sizeof(float));
			delete vertices;
			vertices = tempV;
			vb.reset(new VertexBuffer(nullptr, T::getVertexCount() * T::getVertexSize() * size * sizeof(float)));
			va.reset(new VertexArray());
			va->addBuffer(*vb, *T::getBufferLayout());			
		}
		else
		{
			ERROR_LOG("failed to resize batchbuffer");
			return toReturn;
		}

		unsigned int* tempI = new unsigned int[T::getIndicesLayout().size() * size];
		if (tempI != nullptr)
		{
			memcpy(tempI, indices, T::getIndicesLayout().size() * cursor * sizeof(unsigned int));
			delete indices;
			indices = tempI;
			ib.reset(new IndexBuffer(nullptr, static_cast<unsigned int>(T::getIndicesLayout().size()) * size));
		}
		else
		{
			ERROR_LOG("failed to resize batchbuffer");
			return toReturn;
		}
	}

	memcpy(&vertices[cursor * T::getVertexCount() * T::getVertexSize()], obj.getVertices(),T::getVertexCount() * T::getVertexSize() * sizeof(float));
	std::vector<unsigned int> newIndices = obj.getIndicesLayout();
	unsigned int indicesLayoutSize = static_cast<unsigned int>(newIndices.size());
	for (unsigned int i = 0; i < static_cast<unsigned int>(newIndices.size()); ++i)
	{
		indices[cursor * indicesLayoutSize + i] = cursor * T::getVertexCount() + newIndices[i];
	}
	toReturn = cursor;
	++cursor;
	return toReturn;
}

template<class T>
bool BatchBuffer<T>::updateObject(unsigned int index, T& obj)
{
	static_assert(std::is_base_of<Drawable, T>::value);

	bool toReturn = false;
	if (vertices && indices && index < cursor)
	{
		memcpy(&vertices[index * T::getVertexCount() * T::getVertexSize()], obj.getVertices(), T::getVertexCount() * T::getVertexSize() * sizeof(float));
		toReturn = true;
	}
	return toReturn;
}

template<class T>
void BatchBuffer<T>::updateBuffer()
{
	for (int k = 0; k < cursor; ++k)
	{
		for (int i = 0; i < T::getVertexCount(); ++i)
		{
			std::string line;
			for (int j = 0; j < T::getVertexSize(); ++j)
			{
				line += logger::to_string<>(vertices[(k * T::getVertexSize() * T::getVertexCount()) + (i * T::getVertexSize()) + j]) + " ";
			}
			INFO_LOG(line);
		}
		INFO_LOG("\n");
	}
	vb->bind();
	vb->updateBuffer(vertices, T::getVertexCount() * T::getVertexSize() * cursor * sizeof(float));

	for (int k = 0; k < cursor; ++k)
	{
		std::string line;
		for (int i = 0; i < T::getIndicesLayout().size(); ++i)
		{
			line += logger::to_string<>(indices[(k * T::getIndicesLayout().size()) + i]) + " ";
		}
		INFO_LOG(line);
	}

	ib->bind();
	ib->updateBuffer(indices, static_cast<unsigned int>(T::getIndicesLayout().size()) * cursor);
}

template<class T>
void BatchBuffer<T>::draw()
{
	if (texture)
	{
		RendererFactory::getInstance()->getRenderer(rendererId)->draw(*va, *ib, *shader, *texture);
	}
	else
	{
		RendererFactory::getInstance()->getRenderer(rendererId)->draw(*va, *ib, *shader);
	}
}

template<class T>
void BatchBuffer<T>::draw(std::shared_ptr<Texture> texture)
{
	RendererFactory::getInstance()->getRenderer(rendererId)->draw(*va, *ib, *shader, *texture);
}



