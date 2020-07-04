#include "Window.h"
#include "Logger.h"

std::unordered_map<GLFWwindow*, Window*> Window::windowMap;

void Window::init()
{
	/* Initialize the library */
	if (!glfwInit())
	{
		ERROR("glfw init failed");
		return;
	}

	INFO("creating window...");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
	if (!m_window)
	{
		ERROR("failed to create window");
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);

	windowMap.insert(std::pair<GLFWwindow*, Window*>(m_window, this));

	glfwSetCursorPosCallback(m_window, mousePositionCallback);
	glfwSetMouseButtonCallback(m_window, mouseClickCallback);
	glfwSetCursorEnterCallback(m_window, mouseEnterCallback);
	INFO("window created");
}

Window::Window()
	:m_width(800), m_height(600), m_title(""), m_window(nullptr)
{
	init();
}

Window::Window(int w, int h)
	: m_width(w), m_height(h), m_title(""), m_window(nullptr)
{
	init();
}

Window::Window(int w, int h, std::string title)
	: m_width(w), m_height(h), m_title(title), m_window(nullptr)
{
	init();
}

Window::~Window()
{
}

bool Window::isOkay()
{
	return m_window != nullptr;
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(m_window);
}

void Window::swap()
{
	glfwSwapBuffers(m_window);
}

void Window::poll()
{
	glfwPollEvents();
}

void Window::mousePosition(double xpos, double ypos)
{
	m_mouseState.mouseX = xpos;
	m_mouseState.mouseY = (double)m_height - ypos;
}

void Window::mouseClick(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		m_mouseState.leftDown = false;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		m_mouseState.leftDown = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		m_mouseState.rightDown = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		m_mouseState.rightDown = true;
	}
}

void Window::mouseEnter(int entered)
{
	if (entered)
	{
		m_mouseState.inWindow = true;
	}
	else
	{
		m_mouseState.inWindow = false;
	}
}

void Window::mousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	std::unordered_map<GLFWwindow*, Window*>::const_iterator windowIt = windowMap.find(window);
	
	if (windowIt != windowMap.end())
	{
		windowIt->second->mousePosition(xpos, ypos);
	}	
}

void Window::mouseClickCallback(GLFWwindow* window, int button, int action, int mods)
{
	std::unordered_map<GLFWwindow*, Window*>::const_iterator windowIt = windowMap.find(window);

	if (windowIt != windowMap.end())
	{
		windowIt->second->mouseClick(button, action, mods);
	}
}

void Window::mouseEnterCallback(GLFWwindow* window, int entered)
{
	std::unordered_map<GLFWwindow*, Window*>::const_iterator windowIt = windowMap.find(window);

	if (windowIt != windowMap.end())
	{
		windowIt->second->mouseEnter(entered);
	}
}
