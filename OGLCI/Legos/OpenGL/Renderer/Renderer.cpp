#include "Renderer.h"
#include "ErrorChecking.h"
#include <ext/matrix_clip_space.hpp>

Renderer::Renderer()
	:m_MVP(1.0f),
	camera(nullptr)
{

}

Renderer::Renderer(Camera* camera)
	:m_MVP(1.0f),
	camera(camera)
{
}

Renderer::~Renderer()
{
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader)
{
	shader.bind();
	va.bind();
	ib.bind();
	if (camera)
	{
		m_MVP = camera->getViewMatrix();
	}
	else
	{
		m_MVP = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	}
	shader.setUniformMat4f("u_MVP", m_MVP);
	GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
