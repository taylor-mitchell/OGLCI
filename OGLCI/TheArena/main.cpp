#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Logger.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "ErrorChecking.h"

int main(void)
{
    GLFWwindow* window;    

    /* Initialize the library */
    if (!glfwInit())
    {
        ERROR("glfw init failed");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
           

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

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        INFO("glew init failed");
    };

    INFO(glGetString(GL_VERSION));
    {

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

        
        VertexArray va;
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));
        VertexBufferLayout layout;
        layout.push<float>(2);
        va.addBuffer(vb, layout);       

        IndexBuffer ib(indices, 6);


        INFO("loading shaders...");
        Shader shader("resources/Shaders/basic");

        if (shader.getError() != 0)
        {
            ERROR("failed to load shaders");
            return -1;
        }
        INFO("shaders loaded");

        shader.bind();

        unsigned int program = shader.getProgram();

        GLCall(int location = glGetUniformLocation(program, "u_color"));
        DEBUG_ASSERT(location != -1);

        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        float increment_r = 0.01f;
        float increment_g = 0.02f;
        float increment_b = 0.03f;
        INFO("beginning main loop");
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));
            GLCall(glUniform4f(location, r, g, b, 1.0f));

            va.bind();
            ib.bind();
            GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
            if (r > 1.0f || r < 0.0f)
            {
                increment_r *= -1;
            }
            if (g > 1.0f || g < 0.0f)
            {
                increment_g *= -1;
            }
            if (b > 1.0f || b < 0.0f)
            {
                increment_b *= -1;
            }

            r += increment_r;
            g += increment_g;
            b += increment_b;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        INFO("shutting down");
    }
    glfwTerminate();
    return 0;
}