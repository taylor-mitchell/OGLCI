
#pragma once

class IndexBuffer
{
private:
	unsigned int m_rendererId;
	unsigned int m_count;
public:
	//should extend this to support different index types e.g. short vs int 
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return m_count; };
};