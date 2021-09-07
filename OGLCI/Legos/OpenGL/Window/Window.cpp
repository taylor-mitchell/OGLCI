#include "Window.h"
#include "Logger.h"

std::unordered_map<GLFWwindow*, Window*> Window::windowMap;

void Window::init()
{
	initKeyboard();

	/* Initialize the library */
	if (!glfwInit())
	{
		ERROR_LOG("glfw init failed");
		return;
	}

	INFO_LOG("creating window...");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
	if (!m_window)
	{
		ERROR_LOG("failed to create window");
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);

	windowMap.insert(std::pair<GLFWwindow*, Window*>(m_window, this));

	glfwSetCursorPosCallback(m_window, mousePositionCallback);
	glfwSetMouseButtonCallback(m_window, mouseClickCallback);
	glfwSetCursorEnterCallback(m_window, mouseEnterCallback);
	glfwSetKeyCallback(m_window, keyboardCallback);
	INFO_LOG("window created");
}

void Window::initKeyboard()
{
	m_keyState.keyStates[Key(Key::UNKNOWN)] = false;
	m_keyState.keyStates[Key(Key::A)] = false;
	m_keyState.keyStates[Key(Key::B)] = false;
	m_keyState.keyStates[Key(Key::C)] = false;
	m_keyState.keyStates[Key(Key::D)] = false;
	m_keyState.keyStates[Key(Key::E)] = false;
	m_keyState.keyStates[Key(Key::F)] = false;
	m_keyState.keyStates[Key(Key::G)] = false;
	m_keyState.keyStates[Key(Key::H)] = false;
	m_keyState.keyStates[Key(Key::I)] = false;
	m_keyState.keyStates[Key(Key::J)] = false;
	m_keyState.keyStates[Key(Key::K)] = false;
	m_keyState.keyStates[Key(Key::L)] = false;
	m_keyState.keyStates[Key(Key::M)] = false;
	m_keyState.keyStates[Key(Key::N)] = false;
	m_keyState.keyStates[Key(Key::O)] = false;
	m_keyState.keyStates[Key(Key::P)] = false;
	m_keyState.keyStates[Key(Key::Q)] = false;
	m_keyState.keyStates[Key(Key::R)] = false;
	m_keyState.keyStates[Key(Key::S)] = false;
	m_keyState.keyStates[Key(Key::T)] = false;
	m_keyState.keyStates[Key(Key::U)] = false;
	m_keyState.keyStates[Key(Key::V)] = false;
	m_keyState.keyStates[Key(Key::W)] = false;
	m_keyState.keyStates[Key(Key::X)] = false;
	m_keyState.keyStates[Key(Key::Y)] = false;
	m_keyState.keyStates[Key(Key::Z)] = false;
	m_keyState.keyStates[Key(Key::LEFT)] = false;
	m_keyState.keyStates[Key(Key::RIGHT)] = false;
	m_keyState.keyStates[Key(Key::UP)] = false;
	m_keyState.keyStates[Key(Key::DOWN)] = false;
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
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		m_mouseState.rightDown = false;
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

void Window::keyDown(int key)
{
	if (!m_keyState.keyStates[Key::getKey(key)])
	{
		m_keyState.keyStates[Key::getKey(key)] = true;
		m_keyState.stateChanged = true;
	}	
}

void Window::keyUp(int key)
{
	if (m_keyState.keyStates[Key::getKey(key)])
	{
		m_keyState.keyStates[Key::getKey(key)] = false;
		m_keyState.stateChanged = true;
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

void Window::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::unordered_map<GLFWwindow*, Window*>::const_iterator windowIt = windowMap.find(window);

	if (windowIt != windowMap.end())
	{
		if (action == GLFW_PRESS)
		{
			windowIt->second->keyDown(key);
		}
		else if (action == GLFW_RELEASE)
		{
			windowIt->second->keyUp(key);
		}		
	}
}

Key::KeyEnum Key::getKey(int key)
{
	switch (key)
	{
	case GLFW_KEY_UNKNOWN:
		return UNKNOWN;
	case GLFW_KEY_A:
		return A;
	case GLFW_KEY_B:
		return B;
	case GLFW_KEY_C:
		return C;
	case GLFW_KEY_D:
		return D;
	case GLFW_KEY_E:
		return E;
	case GLFW_KEY_F:
		return F;
	case GLFW_KEY_G:
		return G;
	case GLFW_KEY_H:
		return H;
	case GLFW_KEY_I:
		return I;
	case GLFW_KEY_J:
		return J;
	case GLFW_KEY_K:
		return K;
	case GLFW_KEY_L:
		return L;
	case GLFW_KEY_M:
		return M;
	case GLFW_KEY_N:
		return N;
	case GLFW_KEY_O:
		return O;
	case GLFW_KEY_P:
		return P;
	case GLFW_KEY_Q:
		return Q;
	case GLFW_KEY_R:
		return R;
	case GLFW_KEY_S:
		return S;
	case GLFW_KEY_T:
		return T;
	case GLFW_KEY_U:
		return U;
	case GLFW_KEY_V:
		return V;
	case GLFW_KEY_W:
		return W;
	case GLFW_KEY_X:
		return X;
	case GLFW_KEY_Y:
		return Y;
	case GLFW_KEY_Z:
		return Z;
	case GLFW_KEY_RIGHT:
		return RIGHT;
	case GLFW_KEY_LEFT:
		return LEFT;
	case GLFW_KEY_UP:
		return UP;
	case GLFW_KEY_DOWN:
		return DOWN;
	default:
		return UNKNOWN;
	}
}
