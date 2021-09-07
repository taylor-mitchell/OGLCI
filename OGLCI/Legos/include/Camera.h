#pragma once
#include <glm.hpp>

class Camera
{
private:
	float x;
	float y;
	float z;
	float horz;
	float vert;

	bool orthogonal;

	glm::mat4 m_viewMatrix;

	void init();

public:
	Camera();
	Camera(float width, float height);
	Camera(const Camera& c);
	Camera& operator= (const Camera& c);
	virtual ~Camera();

	glm::mat4 getViewMatrix() { return m_viewMatrix; };

	void setOrthogonal(bool ortho) { orthogonal = ortho; };

	void move(float x, float y, float z);
	void setCenter(float x, float y, float z);
};

