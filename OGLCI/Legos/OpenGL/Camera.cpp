#include "Camera.h"
#include <ext/matrix_clip_space.hpp>

void Camera::init()
{
	if (orthogonal)
	{
		m_viewMatrix = glm::ortho(x, x + horz, y, y + vert, -1.0f, 1.0f);
	}
}

Camera::Camera()
{
	x = 0;
	y = 0;
	z = 0;
	horz = 0;
	vert = 0;
	orthogonal = true;
	init();
}

Camera::Camera(float width, float height)
	:x(0),
	y(0),
	z(0),
	horz(width),
	vert(height),
	orthogonal(true)
{
	init();
}

Camera::Camera(const Camera& c)
{
	x = c.x;
	y = c.y;
	z = c.z;
	horz = c.horz;
	vert = c.vert;
	orthogonal = true;
	init();
}

Camera& Camera::operator=(const Camera& c)
{
	x = c.x;
	y = c.y;
	z = c.z;
	horz = c.horz;
	vert = c.vert;
	return *this;
}

Camera::~Camera()
{
}

void Camera::move(float x1, float y1, float z1)
{
	x += x1;
	y += y1;
	z += z1;
	init();
}

void Camera::setCenter(float x1, float y1, float z1)
{
	x = x1 - (horz / 2.0f);
	y = y1 - (vert / 2.0f);
	init();
}
