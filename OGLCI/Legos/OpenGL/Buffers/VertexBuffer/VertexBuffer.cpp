#include "VertexBuffer.h"
#include "ErrorChecking.h"
#include "LoggingUtils.h"

void VertexBuffer::init(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_rendererId));
    bind();
    if (m_dynamic)
    {
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
    }
    else
    {
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    }          
}

VertexBuffer::VertexBuffer()
    : m_rendererId(0),
    m_dynamic(true)
{
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
    : m_dynamic(true)
{
    init(data, size);
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size, bool dynamic)
    : m_dynamic(dynamic)
{
    init(data, size);
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_rendererId));
}

void VertexBuffer::addData(const void* data, unsigned int size)
{
    if (m_rendererId)
    {
        GLCall(glDeleteBuffers(1, &m_rendererId));
    }
    init(data, size);
}

void VertexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
}

void VertexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::updateBuffer(const void* data, unsigned int size)
{
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}
