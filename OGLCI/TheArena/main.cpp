#include "GLIncludes.h"
#include <iostream>
#include "Shader.h"
#include "Logger.h"
#include "ErrorChecking.h"
#include "Renderer.h"
#include "Texture.h"
#include "Window.h"
#include <glm.hpp>
#include <ext/matrix_clip_space.hpp>
#include <ext/matrix_transform.hpp>
#include "Circle.h" 
#include <time.h>
#include "MatrixFunctions.h"
#include "FileReader.h"
#include "PixelArray.h"
#include "Grid.h"
#include "examples.h"
#include "FontFactory.h"
#include "Rectangle.h"
#include "BatchBuffer.h"
#include "TextBox.h"


void fileReadTest(Window* window);
void mlpTest(Window* window);
void test(Window* window);


std::vector<Array2D<float>> readIDXImageFile(std::string filename);
std::vector<int> readIDXLabelFile(std::string filename);

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
        test(&window);
        INFO_LOG("shutting down");
    }
    glfwTerminate();
    return 0;
}

void test(Window* window)
{
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(window->getWidth(), window->getHeight());
    std::shared_ptr<Renderer> renderer = RendererFactory::getInstance()->getRenderer(0);
    renderer->setCamera(camera);


    while (!window->shouldClose())
    {
        /* Render here */
        renderer->clear();

        window->swap();
        window->poll();
    }

}

void fileReadTest(Window* window)
{
    std::shared_ptr<Shader> shader = ShaderFactory::getInstance()->getShader("resources/Shaders/basic"); 

    std::vector<Array2D<float>> images = readIDXImageFile("Classroom//Test Data//t10k-images.idx3-ubyte");
    std::vector<Array2D<glm::vec3>> pixels;
    pixels.reserve(images.size());
    for (unsigned int i = 0; i < images.size(); ++i)
    {
        pixels.emplace_back(images[i].getWidth(), images[i].getHeight());
        for (int y = images[i].getHeight() - 1; y >= 0; --y)
        {
            for (unsigned int x = 0; x < images[i].getWidth(); ++x)
            {
                pixels[i](x, y) = glm::vec3(images[i](x, y), images[i](x, y), images[i](x, y));
            }
        }
    }
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(window->getWidth(), window->getHeight());
    Renderer renderer(camera);

    std::vector<PixelArray> pAs;
    pAs.reserve(10000);
    int width = pixels[0].getWidth();
    int height = pixels[0].getHeight();
    for (unsigned int i = 0; i < 100; ++i)
    {
        for (unsigned int j = 0; j < 100; ++j)
        {
            pAs.emplace_back(DrawMode::SINGLETON, glm::vec2(i * (width + 2), j * (height + 2)), pixels[j * 100 + i]);
        }   
    }
    glm::vec2 prev;
    bool unclicked = true;
    std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
    int fpsCount = 0;
    while (!window->shouldClose())
    {
        /* Render here */
        renderer.clear();
        
        for (unsigned int i = 0; i < 10000; ++i)
        {
            pAs[i].draw();
        }
        if (window->getMouseState().inWindow)
        {
            if (window->getMouseState().leftDown)
            {
                if (unclicked)
                {
                    prev = glm::vec2((float)window->getMouseState().mouseX, (float)window->getMouseState().mouseY);
                    unclicked = false;
                }
                else
                {
                    camera->move(prev.x - (float)window->getMouseState().mouseX, prev.y - (float)window->getMouseState().mouseY, 0.0f);
                    prev = glm::vec2((float)window->getMouseState().mouseX, (float)window->getMouseState().mouseY);
                }                    
            }
            else
            {
                unclicked = true;
            }
        }
            
        window->swap();
        window->poll();
        fpsCount++;
        if (std::chrono::duration<float, std::milli>(std::chrono::steady_clock::now() - start).count() > 1000)
        {
            start = std::chrono::steady_clock::now();
            INFO_LOG(std::to_string(fpsCount));
            fpsCount = 0;
        }
    }
}


void mlpTest(Window* window)
{
    std::vector<Array2D<float>> trainingData = readIDXImageFile("Classroom//Test Data//t10k-images.idx3-ubyte");
    std::vector<int> trainingLabels = readIDXLabelFile("Classroom//Test Data//t10k-labels.idx1-ubyte");


}

std::vector<Array2D<float>> readIDXImageFile(std::string filename)
{
    std::vector<Array2D<float>> toReturn;

    FileReader reader(filename);

    if (reader.openBinary())
    {
        int magicNum = reader.getInt32();
        int numImages = reader.getInt32();
        int height = reader.getInt32();
        int width = reader.getInt32();

        toReturn.reserve(numImages);


        for (unsigned int i = 0; i < numImages; ++i)
        {
            toReturn.emplace_back(width, height);

            for (int y = height - 1; y >= 0; --y)
            {
                for (unsigned int x = 0; x < width; ++x)
                {
                    toReturn[i](x, y) = (float)reader.getUByte();
                }
            }
        }
    }
    else
    {
        ERROR_LOG(filename + " did not open");
    }
    return toReturn;
}

std::vector<int> readIDXLabelFile(std::string filename)
{
    std::vector<int> toReturn;

    FileReader reader(filename);

    if (reader.openBinary())
    {
        int magicNum = reader.getInt32();
        int numLabels = reader.getInt32();

        for (int i = 0; i < numLabels; ++i)
        {
            toReturn.push_back(reader.getUByte());
        }
    }
    else
    {
        ERROR_LOG(filename + " did not open");
    }
    return toReturn;
}






