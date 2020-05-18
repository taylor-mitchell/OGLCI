#pragma once
#include <string>

class Texture
{
private:
	unsigned int m_rendererId;
	std::string m_filePath;
	unsigned char* m_localBuffer;
	int m_width;
	int m_height;
	int m_BPP;

public:
	Texture(const std::string& path);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int getWidth() const { return m_width; };
	inline int getHeight() const { return m_height; };
};