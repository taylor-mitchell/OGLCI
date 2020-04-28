#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
private:
	int x;
	int y;
	int z;
	int horz;
	int vert;

public:
	Camera();
	Camera(const Camera& c);
	Camera& operator= (const Camera& c);
	virtual ~Camera();

};


#endif