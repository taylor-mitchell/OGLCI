#include "VertexArray.h"
#include "ErrorChecking.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_rendererId));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_rendererId));
}

void VertexArray::bind() const
{
	GLCall(glBindVertexArray(m_rendererId));
}

void VertexArray::unbind() const
{
	GLCall(glBindVertexArray(0));
}


void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	bind();
	vb.bind();
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size();++i)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (void*)offset));
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}
	
}
