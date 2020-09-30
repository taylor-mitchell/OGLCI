#include "Renderer.h"
#include "ErrorChecking.h"
#include <ext/matrix_clip_space.hpp>

const float M_PI = 3.14159265f;

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

void Renderer::draw_circle2d(const glm::vec2& centre, float rad, int segs) const
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_POLYGON);
	for (int i = 0; i < segs; i++) {
		float cosine = rad * cos(i * 2 * M_PI / (float)(segs));
		float sine = rad * sin(i * 2 * M_PI / (float)(segs));
		glm::vec2 tmp = (glm::vec2(cosine, sine) + centre);
		glVertex2f(tmp.x, tmp.y);
	}
	glEnd();
}

void Renderer::draw_box2d(const glm::vec2& origin, float width, float height) const
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glm::vec2 o1 = origin + glm::vec2(0, height);
	glm::vec2 o2 = origin + glm::vec2(width, height);
	glm::vec2 o3 = origin + glm::vec2(width, 0);

	glBegin(GL_POLYGON);
	glVertex2f(origin.x, origin.y);
	glVertex2f(o1.x, o1.y);
	glVertex2f(o2.x, o2.y);
	glVertex2f(o3.x, o3.y);
	glEnd();
}

void Renderer::draw_segmentset2d(const std::vector<glm::vec2>& vertices, const std::vector<glm::i8vec2>& edges) const
{
	glBegin(GL_LINES);
	for (unsigned int i = 0; i < edges.size(); ++i) {
		glVertex2f(vertices[edges[i][0]].x, vertices[edges[i][0]].y);
		glVertex2f(vertices[edges[i][1]].x, vertices[edges[i][1]].y);
	}
	glEnd();
}

void Renderer::draw_points2d(const std::vector<glm::vec2>& points) const
{
	glBegin(GL_POINTS);
	for (unsigned int i = 0; i < points.size(); ++i) {
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();
}

void Renderer::draw_polygon2d(const std::vector<glm::vec2>& vertices) const
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_POLYGON);
	for (unsigned int i = 0; i < vertices.size(); ++i)
		glVertex2f(vertices[i].x, vertices[i].y);
	glEnd();
}

void Renderer::draw_polygon2d(const std::vector<glm::vec2>& vertices, const std::vector<int>& order) const
{
	glBegin(GL_POLYGON);
	for (unsigned int i = 0; i < order.size(); ++i)
		glVertex2f(vertices[order[i]].x, vertices[order[i]].y);
	glEnd();
}

void Renderer::draw_segment2d(const glm::vec2& start, const glm::vec2& end) const
{
	glBegin(GL_LINES);
	glVertex2f(start.x, start.y);
	glVertex2f(end.x, end.y);
	glEnd();
}

void Renderer::draw_point(const glm::vec2 point) const
{
	glVertex2f(point.x, point.y);
}

