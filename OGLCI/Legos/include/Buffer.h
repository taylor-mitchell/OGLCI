#ifndef BUFFER_H
#define BUFFER_H

#include "GLFW/glfw3.h"

class Buffer
{
public:
	Buffer();
	~Buffer();

	void setSize(unsigned int s) { size = s; };

private:
	unsigned int size = 0;
	float* buf = nullptr;
};

#endif