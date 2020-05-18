#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Logger.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "ErrorChecking.h"
#include "Renderer.h"
#include "Texture.h"
#include "Window.h"
#include <glm.hpp>
#include <ext/matrix_clip_space.hpp>
#include <ext/matrix_transform.hpp>
#include "Circle.h" 
#include <time.h>
#include "BatchBuffer.h"

int main(void)
{
    srand(time(NULL));
    Window window;
    if (!window.isOkay())
    {
        return -1;
    }

    if (glewInit() != GLEW_OK)
    {
        INFO("glew init failed");
    };
    INFO(glGetString(GL_VERSION));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));
    {
        Camera camera(window.getWidth(), window.getHeight());
        Renderer renderer(&camera);
        
        Circle circle(400.0f, 400.0f, 50.0f);
        Circle circle2(400.0f, 300.0f, 50.0f);
        BatchBuffer<Circle> batch(2, circle.getShader());
        circle.setDX(1.0f);
        circle2.setDX(-1.0f);
        int id = batch.addToBuffer(circle);
        int id2 = batch.addToBuffer(circle2);

        INFO("beginning main loop");
        /* Loop until the user closes the window */
        while (!window.shouldClose())
        {
            /* Render here */
            renderer.clear();
            batch.updateBuffer();
            batch.draw(renderer);
            circle.update();
            circle2.update();
            batch.updateObject(id, circle);
            batch.updateObject(id2, circle2);

            window.swap();
            window.poll();
        }

        INFO("shutting down");
    }
    glfwTerminate();
    return 0;
}

void drawABunchOfCircles(Window* window)
{
    const int NUM_CIRCLES = 100;
    Circle circles[NUM_CIRCLES];    

    for (int i = 0; i < NUM_CIRCLES; i++)
    {
        float x = rand() % window->getWidth();
        float y = rand() % window->getHeight();
        float r = rand() % 10 + 1;
        float red = (float)rand() / (float)RAND_MAX;
        float blue = (float)rand() / (float)RAND_MAX;
        float green = (float)rand() / (float)RAND_MAX;
        float dx = (float)rand() / (float)RAND_MAX - 0.5f;
        float dy = (float)rand() / (float)RAND_MAX - 0.5f;
        circles[i].setX(x);
        circles[i].setY(y);
        circles[i].setR(r);
        circles[i].setRed(red);
        circles[i].setBlue(blue);
        circles[i].setGreen(green);
        circles[i].setDX(dx);
        circles[i].setDY(dy);
    }

    Camera camera(window->getWidth(), window->getHeight());
    Renderer renderer(&camera);

    INFO("beginning main loop");
    /* Loop until the user closes the window */
    while (!window->shouldClose())
    {
        /* Render here */
        renderer.clear();
        for (int i = 0; i < NUM_CIRCLES; i++)
            circles[i].draw(renderer);
        if (window->getMouseState().inWindow)
        {
            if (window->getMouseState().leftDown)
            {
                for (int i = 0; i < NUM_CIRCLES; i++)
                    circles[i].gravitate(window->getMouseState().mouseX, window->getMouseState().mouseY, 1.0f, 1000.0f);
            }
        }
        for (int i = 0; i < NUM_CIRCLES; i++)
            circles[i].update();
        window->swap();
        window->poll();
    }

    INFO("shutting down");
}