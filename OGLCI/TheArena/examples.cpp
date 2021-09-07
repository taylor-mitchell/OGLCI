#include "examples.h"
#include "Camera.h"
#include "RendererFactory.h"
#include "FontFactory.h"

#include "Circle.h"
#include "Rectangle.h"
#include "Grid.h"
#include "PixelArray.h"
#include "TextBox.h"

#include "BatchBuffer.h"

void drawingExample(Window* window)
{
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(window->getWidth(), window->getHeight());
	std::shared_ptr<Renderer> renderer = RendererFactory::getInstance()->getRenderer(0);
	renderer->setCamera(camera);

	//a red circle 
	Circle circle(DrawMode::SINGLETON,
		glm::vec2(window->getWidth() / 12, 7 * window->getHeight() / 8),
		window->getWidth() / 12 > window->getHeight() / 8 ? window->getHeight() / 8 : window->getWidth() / 12);
	circle.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	TextBox circleText(FontFactory::getInstance()->getFont("resources/fonts/Pixellettersfull-BnJ5.ttf"),
		20,
		"circle",
		glm::vec2(circle.getPos().x - circle.getRadius(), circle.getPos().y - circle.getRadius() - 30),
		glm::vec2(0.0f));

	//a blue square
	Rectangle square(DrawMode::SINGLETON,
		glm::vec2(window->getWidth() / 6, 3 * window->getHeight() / 4),
		glm::vec2(window->getWidth() / 6, window->getHeight() / 4));
	square.setColor(glm::vec3(0.0f, 0.0f, 1.0f));
	TextBox squareText(FontFactory::getInstance()->getFont("resources/fonts/Pixellettersfull-BnJ5.ttf"),
		20,
		"square",
		glm::vec2(square.getPos().x, square.getPos().y - 30),
		glm::vec2(0.0f));


	//four textured squares drawn in batch
	Rectangle square1(DrawMode::BUFFERED,
		glm::vec2(4 * window->getWidth() / 12, 6 * window->getHeight() / 8),
		glm::vec2(window->getWidth() / 12, window->getHeight() / 8));
	Rectangle square2(DrawMode::BUFFERED,
		glm::vec2(5 * window->getWidth() / 12, 6 * window->getHeight() / 8),
		glm::vec2(window->getWidth() / 12, window->getHeight() / 8));
	Rectangle square3(DrawMode::BUFFERED,
		glm::vec2(4 * window->getWidth() / 12, 7 * window->getHeight() / 8),
		glm::vec2(window->getWidth() / 12, window->getHeight() / 8));
	Rectangle square4(DrawMode::BUFFERED,
		glm::vec2(5 * window->getWidth() / 12, 7 * window->getHeight() / 8),
		glm::vec2(window->getWidth() / 12, window->getHeight() / 8));
	BatchBuffer<Rectangle> batch;
	batch.addToBuffer(square1);
	batch.addToBuffer(square2);
	batch.addToBuffer(square3);
	batch.addToBuffer(square4);
	batch.updateBuffer();
	batch.setTexture(TextureFactory::getInstance()->getTexture("resources/textures/test.jpg"));
	TextBox batchText(FontFactory::getInstance()->getFont("resources/fonts/Pixellettersfull-BnJ5.ttf"),
		20,
		"batch squares",
		glm::vec2(square1.getPos().x, square1.getPos().y - 30),
		glm::vec2(0.0f));

	//an square of randomly colored pixels
	int arrayWidth = window->getWidth() / 6;
	int arrayHeight = window->getHeight() / 4;
	Array2D<glm::vec3> p(arrayWidth, arrayHeight);
	for (unsigned int i = 0; i < arrayWidth; ++i)
	{
		for (unsigned int j = 0; j < arrayHeight; ++j)
		{
			float red = (float)rand() / (float)RAND_MAX;
			float blue = (float)rand() / (float)RAND_MAX;
			float green = (float)rand() / (float)RAND_MAX;

			p(i, j) = glm::vec3(red, blue, green);
		}
	}
	PixelArray pixels(DrawMode::SINGLETON, glm::vec2(3 * window->getWidth() / 6, 3 * window->getHeight() / 4), p);
	TextBox pixelText(FontFactory::getInstance()->getFont("resources/fonts/Pixellettersfull-BnJ5.ttf"),
		20,
		"pixels",
		glm::vec2(pixels.getPos().x, pixels.getPos().y - 30),
		glm::vec2(0.0f));

	while (!window->shouldClose())
	{
		/* Render here */
		renderer->clear();

		circle.draw();
		circleText.draw();

		square.draw();
		squareText.draw();

		batch.draw();
		batchText.draw();

		pixels.draw();
		pixelText.draw();

		//some input handling
		if (window->getMouseState().inWindow)
		{
			INFO_LOG("mouse position: " + std::to_string(window->getMouseState().mouseX) + ", " + std::to_string(window->getMouseState().mouseY));
		}
		if (window->getKeyboardState().keyStates[Key::LEFT])
		{
			camera->move(-1.0f, 0.0f, 0.0f);
		}
		if (window->getKeyboardState().keyStates[Key::RIGHT])
		{
			camera->move(1.0f, 0.0f, 0.0f);
		}
		if (window->getKeyboardState().keyStates[Key::UP])
		{
			camera->move(0.0f, 1.0f, 0.0f);
		}
		if (window->getKeyboardState().keyStates[Key::DOWN])
		{
			camera->move(0.0f, -1.0f, 0.0f);
		}
		window->swap();
		window->poll();
	}

}
