#pragma once

class VertexBufferLayout;
class VertexBuffer;

class VertexArray
{
private:
	unsigned int m_rendererId;
public:
	VertexArray();
	~VertexArray();

	void bind() const;
	void unbind() const;

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
};