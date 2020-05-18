#pragma once

class IndexBuffer
{
private:
	unsigned int m_rendererId;
	unsigned int m_count;

	void init(const unsigned int* data, unsigned int count);
public:
	IndexBuffer();
	//should extend this to support different index types e.g. short vs int 
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void addData(const unsigned int* data, unsigned int count);
	void bind() const;
	void unbind() const;
	void updateBuffer(const unsigned int* data, unsigned int count);

	inline unsigned int getCount() const { return m_count; };
};