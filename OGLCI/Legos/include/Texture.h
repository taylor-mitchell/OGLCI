#pragma once
#include <string>
#include "Shader.h"

class Texture
{
private:
	unsigned int m_rendererId;
	int m_width;
	int m_height;
	std::shared_ptr<Shader> textureShader;
	std::string name;

public:
	Texture()
		:m_rendererId(0),
		m_width(0),
		m_height(0),
		textureShader(nullptr)
	{};
	Texture(const std::string& path);
	Texture(unsigned int width, unsigned int height);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void clear();
	void unbind() const;

	inline int getWidth() const { return m_width; };
	inline int getHeight() const { return m_height; };
	inline unsigned int getRendererId() const { return m_rendererId; };
	static std::shared_ptr<Shader> getShader();

	void setName(std::string name) { this->name = name; };
	std::string getName() { return name; };
};