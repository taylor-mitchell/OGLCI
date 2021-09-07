#include "IndexBuffer.h"
#include "ErrorChecking.h"

void IndexBuffer::init(const unsigned int* data, unsigned int count)
{
    //because I am not using GL types, maybe need to set up a file with some asserts that
    //regular c++ types are the same size as GL types
    GLCall(glGenBuffers(1, &m_rendererId));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::IndexBuffer()
    :m_rendererId(0),
    m_count(0)
{
}

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    :m_count(count)
{
    init(data, count);
}

void IndexBuffer::addData(const unsigned int* data, unsigned int count)
{
    if (m_rendererId)
    {
        GLCall(glDeleteBuffers(1, &m_rendererId));
    }
    init(data, count);
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_rendererId));
}

void IndexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
}

void IndexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::updateBuffer(const unsigned int* data, unsigned int count)
{
    GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(unsigned int), data));
}
