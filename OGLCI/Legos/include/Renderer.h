#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include <glm.hpp>
#include "Camera.h"
#include "Texture.h"
#include "Array2D.h"
#include <memory>


class Renderer
{
private:
	glm::mat4 m_MVP;
	std::shared_ptr<Camera> camera;
public:
	Renderer();
	Renderer(std::shared_ptr<Camera> camera);
	~Renderer();
	void draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader);
	void draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader, Texture& texture);
	void renderToTexture(Texture& texture, const VertexArray& va, const IndexBuffer& ib);
	void renderToTexture(Texture& texture, Array2D<glm::vec3>& pixels);
	void clear() const;
	void setCamera(std::shared_ptr<Camera> camera);
};