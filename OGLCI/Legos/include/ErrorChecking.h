#pragma once

#include "GLIncludes.h"
#include <string>
#include <sstream>

#include "Logger.h"

#define DEBUG_ASSERT(x)  if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    DEBUG_ASSERT(GLCheckError(#x, __FILE__, __LINE__))

static std::string GetErrorString(GLenum error)
{
    std::string toReturn;
    switch (error)
    {
    case 0x0500:
        toReturn = "Invalid Enum";
        break;
    case 0x0501:
        toReturn = "Invalid Value";
        break;
    case 0x0502:
        toReturn = "Invalid Operation";
        break;
    case 0x0503:
        toReturn = "Stack Overflow";
        break;
    case 0x0504:
        toReturn = "Stack Underflow";
        break;
    case 0x0505:
        toReturn = "Out of Memory";
        break;
    default:
        std::stringstream ss;
        ss << "Unknown Error: " << error;
        toReturn = ss.str();
        break;
    }

    return toReturn;
}

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLCheckError(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        ERRORLOC_LOG("OpenGL Error - " + GetErrorString(error), function, file, line);
        return false;
    }
    return true;
}