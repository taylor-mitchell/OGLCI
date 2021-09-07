#pragma once

class VertexBuffer
{
private:
	unsigned int m_rendererId;
	bool m_dynamic;

	void init(const void* data, unsigned int size);
public:
	VertexBuffer();
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(const void* data, unsigned int size, bool dynamic);
	~VertexBuffer();

	void addData(const void* data, unsigned int size);
	void bind() const;
	void unbind() const;
	void updateBuffer(const void* data, unsigned int size);
};