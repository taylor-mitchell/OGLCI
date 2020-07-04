#pragma once
#include "Shader.h"
#include "Drawable.h"
#include "Renderer.h"
#include "Logger.h"

//TODO: add debug logging of vertex/index buffers
//TODO: clean up

template <class T>
class BatchBuffer
{
	static_assert(std::is_base_of<Drawable, T>::value);
private:
	int size;
	unsigned int verticesSize;
	unsigned int indicesSize;
	unsigned int cursor;
	std::shared_ptr<Shader> shader;
    std::unique_ptr<VertexArray> va;
	std::unique_ptr <VertexBuffer> vb;
	std::unique_ptr <IndexBuffer> ib;
	std::vector<unsigned int> indicesLayout;

	float* vertices;	
	unsigned int* indices;

	void resetLocalBuffers();

public:
	BatchBuffer(int size);
	~BatchBuffer();

	int addToBuffer(T& obj);
	bool updateObject(unsigned int, T& obj);
	void updateBuffer();
	void draw(Renderer& renderer);
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
BatchBuffer<T>::BatchBuffer(int size)
	:size(size),
	verticesSize(T::getVertexCount()* T::getVertexSize()* size * sizeof(float)),
	indicesSize(T::getIndicesLayout().size()* size),
	cursor(0),
	shader(T::getShader()),
	va(std::make_unique<VertexArray>()),
	vb(std::make_unique<VertexBuffer>(nullptr, verticesSize, true)),
	ib(std::make_unique<IndexBuffer>(nullptr, T::getIndicesLayout().size() * size)),
	vertices(nullptr),
	indices(nullptr)
{
	resetLocalBuffers();
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

	if (cursor < size)
	{
		memcpy(&vertices[cursor * T::getVertexCount() * T::getVertexSize()], obj.getVertices(),T::getVertexCount() * T::getVertexSize() * sizeof(float));
		std::vector<unsigned int> newIndices = obj.getIndicesLayout();
		unsigned int indicesLayoutSize = newIndices.size();
		for (unsigned int i = 0; i < newIndices.size(); ++i)
		{
			indices[cursor * indicesLayoutSize + i] = cursor * T::getVertexCount() + newIndices[i];
		}
		toReturn = cursor;
		++cursor;
	}
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
	vb->bind();
	vb->updateBuffer(vertices, verticesSize);

	ib->bind();
	ib->updateBuffer(indices, indicesSize);
}

template<class T>
void BatchBuffer<T>::draw(Renderer& renderer)
{
	renderer.draw(*va, *ib, *shader);
}



