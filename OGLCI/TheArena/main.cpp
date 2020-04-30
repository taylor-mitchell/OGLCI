#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Logger.h"

int main(void)
{
    GLFWwindow* window;    

    /* Initialize the library */
    if (!glfwInit())
    {
        ERROR("glfw init failed");
        return -1;
    }
           

    /* Create a windowed mode window and its OpenGL context */
    INFO("creating window...");
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        ERROR("failed to create window");
        glfwTerminate();
        return -1;
    }
    INFO("window created");

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        INFO("glew init failed");
    };

    INFO(glGetString(GL_VERSION));
    
    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int biffer;
    glGenBuffers(1, &biffer);
    glBindBuffer(GL_ARRAY_BUFFER, biffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(0);

    unsigned int indexBiffer;
    glGenBuffers(1, &indexBiffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBiffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);


    

    INFO("loading shaders...");
    Shader shader("resources/Shaders/basic");
    
    if (shader.getError() != 0)
    {
        ERROR("failed to load shaders");
        return -1;
    }
    INFO("shaders loaded");

    shader.bind();

    INFO("beginning main loop");
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    INFO("shutting down");
    glfwTerminate();
    return 0;
}