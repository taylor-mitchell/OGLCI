#include "GLIncludes.h"
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
#include "Grid.h"
#include "FluidGrid.h"
#include "MatrixFunctions.h"
#include "Line.h"

void drawABunchOfCircles(Window* window);
void drawTwoCircles(Window* window);
//void drawFluidInCircle(Window* window);
void drawGrid(Window* window);
void drawFluid(Window* window);
void drawArray(Window* window, Array2D<float> data);
void drawLineTest(Window* window);

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
        INFO_LOG("glew init failed");
    };
    INFO_LOG(glGetString(GL_VERSION));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));
    {
        //drawLineTest(&window);
        INFO_LOG("shutting down");
    }
    glfwTerminate();
    return 0;
}

void drawABunchOfCircles(Window* window)
{
    //TODO: need to time difference between batch rendering and non batch rendering
     
    const unsigned int NUM_CIRCLES = 1000000;
    Circle* circles = new Circle[NUM_CIRCLES]();  
    BatchBuffer<Circle> batch(NUM_CIRCLES);

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
        circles[i].setPos(glm::vec2(x, y));
        circles[i].setRadius(r);
        circles[i].setColor(glm::vec3(red, green, blue));
        circles[i].setVelocity(glm::vec2(dx, dy));
        circles[i].setBatchIndex(batch.addToBuffer(circles[i]));
    }

    Camera camera(window->getWidth(), window->getHeight());
    Renderer renderer(&camera);

    INFO_LOG("beginning main loop");
    /* Loop until the user closes the window */
    while (!window->shouldClose())
    {
        /* Render here */
        renderer.clear();
        batch.updateBuffer();
        batch.draw(renderer);
        if (window->getMouseState().inWindow)
        {
            if (window->getMouseState().leftDown)
            {
                for (int i = 0; i < NUM_CIRCLES; i++)
                {

                    circles[i].gravitateTowards(glm::vec2(window->getMouseState().mouseX, window->getMouseState().mouseY), 0.01f, 1000.0f);
                }
                    
            }
        }
        for (int i = 0; i < NUM_CIRCLES; i++)
        {
            circles[i].update();
            batch.updateObject(circles[i].getBatchIndex(), circles[i]);
        }
        window->swap();
        window->poll();
    }

    if (circles)
    {
        delete[] circles;
    }
}

void drawTwoCircles(Window* window)
{
    const int NUM_CIRCLES = 2;
    Circle* circles = new Circle[NUM_CIRCLES]();
    BatchBuffer<Circle> batch(NUM_CIRCLES);

    circles[0].setPos(glm::vec2(window->getWidth() / 3.0f, window->getHeight() / 2.0f + 10.0f));
    circles[0].setRadius(50.0f);
    circles[0].setColor(glm::vec3((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX));
    circles[0].setVelocity(glm::vec2(1.0f, 0.0f));
    circles[0].setBatchIndex(batch.addToBuffer(circles[0]));

    circles[1].setPos(glm::vec2(2.0f * window->getWidth() / 3.0f, window->getHeight() / 2.0f - 10.0f));
    circles[1].setRadius(50.0f);
    circles[1].setColor(glm::vec3((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX));
    circles[1].setVelocity(glm::vec2(-1.0f, 0.0f));
    circles[1].setBatchIndex(batch.addToBuffer(circles[0]));

    Camera camera(window->getWidth(), window->getHeight());
    Renderer renderer(&camera);

    INFO_LOG("beginning main loop");
    /* Loop until the user closes the window */
    while (!window->shouldClose())
    {
        
        /* Render here */
        renderer.clear();
        batch.updateBuffer();
        batch.draw(renderer);

        for (int i = 0; i < NUM_CIRCLES; i++)
        {
            for (int j = i + 1; j < NUM_CIRCLES; ++j)
            {
                /*
                float deltaX = circles[i].getX() - circles[j].getX();
                float deltaY = circles[i].getY() - circles[j].getY();
                float distance = std::sqrt(glm::dot(glm::vec2(deltaX, deltaY), glm::vec2(deltaX, deltaY)));
                

                if (distance < circles[i].getR() + circles[j].getR())
                {
                    float angle = std::atan2(deltaY, deltaX);
                    float c1Angle = std::atan2(circles[i].getDY(), circles[i].getDX());
                    float c2Angle = std::atan2(circles[j].getDY(), circles[j].getDX());
                    float c1NewAngle = angle - (angle - c1Angle);
                    float c2NewAngle = angle - (angle - c2Angle);

                    circles[i].setDX(std::cos(c1NewAngle) * -1 * circles[i].getDX());
                    circles[i].setDY(std::sin(c1NewAngle) * -1 * circles[i].getDY());

                    circles[j].setDX(std::cos(c2NewAngle)  * circles[j].getDX());
                    circles[j].setDY(std::sin(c2NewAngle) * circles[j].getDY());
                }*/
            }
            circles[i].update();
            batch.updateObject(circles[i].getBatchIndex(), circles[i]);
        }
        window->swap();
        window->poll();
    }

    if (circles)
    {
        delete[] circles;
    }
}

/*
void drawFluidInCircle(Window* window)
{
    Camera camera(window->getWidth(), window->getHeight());
    Renderer renderer(&camera);
    int grid_resolution = 100;
    scalar timestep = 0.005;
    scalar grid_width = 100.0;
    
    FluidSim sim;
    Vector2s c0(50, 50), c1(70, 50), c2(30, 35), c3(50, 70);
    Vector2s s0(10, 5);
    scalar rad0 = 40, rad1 = 10, rad2 = 10, rad3 = 10;
    Vector2s o0(0.0, 0.0);

    //Set up the simulation
    sim.initialize(o0, grid_width, grid_resolution, grid_resolution, 1.0);

    sim.root_boundary = new FluidSim::Boundary(c0, Vector2s(rad0, 0.0), FluidSim::BT_CIRCLE, true);

    sim.root_sources = NULL;

    sim.update_boundary();
    sim.init_random_particles();
    while (!window->shouldClose())
    {
        renderer.clear();
        sim.render(renderer);
        sim.advance(timestep);
        window->swap();
        window->poll();
    }
}*/

void drawGrid(Window* window)
{
    Camera camera(window->getWidth(), window->getHeight());
    Renderer renderer(&camera);

    Grid grid(80, 60, 10.0f, 10.0f);

    while (!window->shouldClose())
    {
        renderer.clear();
        grid.draw(renderer);
        window->swap();
        window->poll();
    }
}

void drawFluid(Window* window)
{
    Camera camera(window->getWidth(), window->getHeight());
    Renderer renderer(&camera);

    const int NUM_PARTICLES = 10;
    Circle** circles = new Circle*[NUM_PARTICLES]();
    BatchBuffer<Circle> batch(NUM_PARTICLES);

    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        float x = (rand() % 100) + window->getWidth() / 2 - 50;
        float y = (rand() % 100) + window->getWidth() / 2 - 50;
        float r = 2.0f;
        float red = 1.0f;
        float blue = 1.0f;
        float green = 1.0f;
        float dx = 0.0f;
        float dy = 0.0f;
        circles[i] = new Circle();
        circles[i]->setPos(glm::vec2(x, y));
        circles[i]->setRadius(r);
        circles[i]->setColor(glm::vec3(red, green, blue));
        circles[i]->setVelocity(glm::vec2(dx, dy));
        circles[i]->setBatchIndex(batch.addToBuffer(*circles[i]));
        INFO_LOG(std::to_string(circles[i]->getPos().x) + ", " + std::to_string(circles[i]->getPos().y));
    }
    
    FluidGrid grid(4, 4, 200.0f, 150.0f);
    while (!window->shouldClose())
    {
        /* Render here */
        renderer.clear();
        batch.updateBuffer();
        grid.getGrid()->draw(renderer);
        batch.draw(renderer); 

        for (int i = 0; i < NUM_PARTICLES; i++)
        {
            grid.addParticle(circles[i]);
        }

        grid.update();       

        for (int i = 0; i < NUM_PARTICLES; i++)
        {
            circles[i]->update();
            INFO_LOG(std::to_string(circles[i]->getPos().x) + ", " + std::to_string(circles[i]->getPos().y));
            batch.updateObject(circles[i]->getBatchIndex(), *circles[i]);
        }

        window->swap();
        window->poll();
    }
    
    
    if (circles)
    {
        for (int i = 0; i < NUM_PARTICLES; i++)
        {
            delete circles[i];
        }
        delete[] circles;
    }
}

void drawLineTest(Window* window)
{
    Camera camera(window->getWidth(), window->getHeight());
    Renderer renderer(&camera);

   
    Line line(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    BatchBuffer<Line> batch(1);
    batch.addToBuffer(line);

    while (!window->shouldClose())
    {
        /* Render here */
        renderer.clear();
        
        
        if (window->getMouseState().inWindow)
        {
            if (window->getMouseState().leftDown)
            {
                line.setPos1(glm::vec2(window->getMouseState().mouseX, window->getMouseState().mouseY));
            }
            line.setPos2(glm::vec2(window->getMouseState().mouseX, window->getMouseState().mouseY));
            line.update();
            batch.updateObject(line.getBatchIndex(), line);
        }
        
        batch.updateBuffer();
        batch.draw(renderer);
        

        window->swap();
        window->poll();
    }
}





