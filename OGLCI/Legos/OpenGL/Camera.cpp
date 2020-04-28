#include "Camera.h"

Camera::Camera()
{
	x = 0;
	y = 0;
	z = 0;
	horz = 0;
	vert = 0;
}

Camera::Camera(const Camera& c)
{
	x = c.x;
	y = c.y;
	z = c.z;
	horz = c.horz;
	vert = c.vert;
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