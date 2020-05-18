#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include <glm.hpp>
#include "Camera.h"


class Renderer
{
private:
	glm::mat4 m_MVP;
	Camera* camera;
public:
	Renderer();
	Renderer(Camera* camera);
	~Renderer();
	void draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader);
	void clear() const;
};