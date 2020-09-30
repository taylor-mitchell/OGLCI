#pragma once
#include "GLIncludes.h"
#include <string>
#include <glm.hpp>
#include <unordered_map>

struct MouseState
{
	double mouseX;
	double mouseY;
	bool leftDown = false;
	bool rightDown = false;
	bool inWindow = false;
};

class Window
{
private:
	int m_width;
	int m_height;
	std::string m_title;
	GLFWwindow* m_window;
	glm::mat4 m_projMatrix;

	static std::unordered_map<GLFWwindow*, Window*> windowMap;

	MouseState m_mouseState;

	void init();

public:
	Window();
	Window(int w, int h);
	Window(int w, int h, std::string title);
	~Window();

	inline int getWidth() const { return m_width; };
	inline int getHeight() const { return m_height; };
	inline MouseState getMouseState() const { return m_mouseState; };

	glm::mat4 getProjectionMatrix() const { return m_projMatrix; };

	bool isOkay();
	bool shouldClose();
	void swap();
	void poll();

	void mousePosition(double xpos, double ypos);
	void mouseClick(int button, int action, int mods);
	void mouseEnter(int entered);

	static void mousePositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);
	static void mouseEnterCallback(GLFWwindow* window, int entered);


};