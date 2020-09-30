#pragma once
#include "GLIncludes.h"
#include "ErrorChecking.h"
#include <vector>


struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;	
	unsigned char normalized;

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		DEBUG_ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride;
public:
	VertexBufferLayout()
		:m_stride() {};
	VertexBufferLayout(const VertexBufferLayout& layout)
		:m_elements(layout.m_elements)
		, m_stride(layout.m_stride) {};

	~VertexBufferLayout() {};

	inline unsigned int getStride() const { return m_stride; };

	template<typename T>
	void push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void push<float>(unsigned int count)
	{
		//maybe define this myself so i dont have to include the whole glew header
		m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_stride += VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		//maybe define this myself so i dont have to include the whole glew header
		m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void push<unsigned char>(unsigned int count)
	{
		//maybe define this myself so i dont have to include the whole glew header
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<VertexBufferElement> getElements() const { return m_elements; }

};