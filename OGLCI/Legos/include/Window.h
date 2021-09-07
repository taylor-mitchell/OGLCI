#pragma once
#include "GLIncludes.h"
#include <string>
#include <glm.hpp>
#include <unordered_map>
#include <map>

class Key
{
public:

	enum KeyEnum
	{
		UNKNOWN,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	Key() = default;
	constexpr Key(KeyEnum key) : value(key) {};

	operator KeyEnum() const { return value; }; 
	constexpr friend bool operator==(const Key& a, const Key& b) { return a.value == b.value; }
	constexpr friend bool operator!=(const Key& a, const Key& b) { return a.value != b.value; }

	static KeyEnum getKey(int key);


private:
	KeyEnum value;
};


struct MouseState
{
	double mouseX;
	double mouseY;
	bool leftDown = false;
	bool rightDown = false;
	bool inWindow = false;
	
};

struct KeyboardState
{
	bool stateChanged = false;
	std::map<Key, bool> keyStates;
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
	KeyboardState m_keyState;

	void init();
	void initKeyboard();

public:
	Window();
	Window(int w, int h);
	Window(int w, int h, std::string title);
	~Window();

	inline int getWidth() const { return m_width; };
	inline int getHeight() const { return m_height; };
	inline MouseState getMouseState() const { return m_mouseState; };
	inline KeyboardState getKeyboardState() const { return m_keyState; };

	glm::mat4 getProjectionMatrix() const { return m_projMatrix; };

	bool isOkay();
	bool shouldClose();
	void swap();
	void poll();

	void mousePosition(double xpos, double ypos);
	void mouseClick(int button, int action, int mods);
	void mouseEnter(int entered);
	void keyDown(int key);
	void keyUp(int key);

	static void mousePositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);
	static void mouseEnterCallback(GLFWwindow* window, int entered);
	static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);


};