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

	void draw_circle2d(const glm::vec2& centre, float rad, int segs) const;
	void draw_box2d(const glm::vec2& origin, float width, float height) const;
	void draw_segmentset2d(const std::vector<glm::vec2>& vertices, const std::vector<glm::i8vec2>& edges) const;
	void draw_points2d(const std::vector<glm::vec2>& points) const;
	void draw_polygon2d(const std::vector<glm::vec2>& vertices) const;
	void draw_polygon2d(const std::vector<glm::vec2>& vertices, const std::vector<int>& order) const;
	void draw_segment2d(const glm::vec2& start, const glm::vec2& end) const;
	void draw_point(const glm::vec2 point) const;
	void begin() const
	{
		glPointSize(5);
		glBegin(GL_POINTS);
	};

	void end() const
	{
		glEnd();
	}

	void setColor(float r, float g, float b) const
	{
		glColor3d(r, g, b);
	}

	void pushMatrix(float x, float y, float z) const
	{
		glPushMatrix();
		glScaled(x, y, z);
	}

	void popMatrix() const
	{
		glPopMatrix();
	}
};