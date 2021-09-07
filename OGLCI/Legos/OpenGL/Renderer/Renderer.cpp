#include "Renderer.h"
#include "ErrorChecking.h"
#include <ext/matrix_clip_space.hpp>

const float M_PI = 3.14159265f;

Renderer::Renderer()
	:m_MVP(1.0f),
	camera(nullptr)
{

}

Renderer::Renderer(std::shared_ptr<Camera> c)
	:m_MVP(1.0f),
	camera(c)
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

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader, Texture& texture)
{
	texture.bind();
	shader.bind();
	shader.setUniform1i("u_Texture", 0);	
	if (camera)
	{
		m_MVP = camera->getViewMatrix();
	}
	else
	{
		m_MVP = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	}
	shader.setUniformMat4f("u_MVP", m_MVP);
	va.bind();
	ib.bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::renderToTexture(Texture& texture, const VertexArray& va, const IndexBuffer& ib)
{
	texture.clear();
	
	//i should make a frame buffer class
	unsigned int fbo;
	GLCall(glGenFramebuffers(1, &fbo));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
	texture.bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.getRendererId(), 0);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);
	unsigned int fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != (unsigned int)GL_FRAMEBUFFER_COMPLETE)
	{
		ERROR_LOG("failed to bind frame buffer for rendering to texture, status is " + std::to_string(fboStatus));
		return;
	}

	m_MVP = glm::ortho(0.0f, (float)texture.getWidth(), 0.0f, (float)texture.getHeight(), -1.0f, 1.0f);
	
	Texture::getShader()->bind();
	va.bind();
	ib.bind();
	Texture::getShader()->setUniformMat4f("u_MVP", m_MVP);
	GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));	
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	texture.unbind();
}

void Renderer::renderToTexture(Texture& texture, Array2D<glm::vec3>& pixels)
{
	int viewportData[4];
	GLCall(glGetIntegerv(GL_VIEWPORT, viewportData));
	texture.clear();

	float* pixelVertices = new float[pixels.getWidth() * pixels.getHeight() * 5];

	for (unsigned int i = 0; i < pixels.getWidth(); ++i)
	{
		for (unsigned int j = 0; j < pixels.getHeight(); ++j)
		{
			pixelVertices[j * pixels.getWidth() * 5 + (5 * i)] = i;
			pixelVertices[j * pixels.getWidth() * 5 + (5 * i) + 1] = j;
			pixelVertices[j * pixels.getWidth() * 5 + (5 * i) + 2] = pixels(i, j).r;
			pixelVertices[j * pixels.getWidth() * 5 + (5 * i) + 3] = pixels(i, j).g;
			pixelVertices[j * pixels.getWidth() * 5 + (5 * i) + 4] = pixels(i, j).b;
		}
	}

	VertexArray pVA;
	VertexBuffer pVB(pixelVertices, pixels.getWidth() * pixels.getHeight() * 5 * sizeof(float));
	VertexBufferLayout pLayout;
	pLayout.push<float>(2);
	pLayout.push<float>(3);
	pVA.addBuffer(pVB, pLayout);

	//i should make a frame buffer class
	unsigned int fbo;
	GLCall(glGenFramebuffers(1, &fbo));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
	texture.bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.getRendererId(), 0);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);
	unsigned int fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != (unsigned int)GL_FRAMEBUFFER_COMPLETE)
	{
		ERROR_LOG("failed to bind frame buffer for rendering to texture, status is " + std::to_string(fboStatus));
		return;
	}
	GLCall(glViewport(0, 0, texture.getWidth(), texture.getHeight()));
	m_MVP = glm::ortho(0.0f, (float)texture.getWidth(), 0.0f, (float)texture.getHeight(), -1.0f, 1.0f);

	Texture::getShader()->bind();
	Texture::getShader()->setUniformMat4f("u_MVP", m_MVP);
	pVA.bind();
	GLCall(glDrawArrays(GL_POINTS, 0, pixels.getWidth() * pixels.getHeight()));

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall(glViewport(viewportData[0], viewportData[1], viewportData[2], viewportData[3]));
	texture.unbind();
}

void Renderer::clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::setCamera(std::shared_ptr<Camera> c)
{
	camera = c;
}