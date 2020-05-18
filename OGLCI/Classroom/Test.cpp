#define CATCH_CONFIG_MAIN
#define GLEW_STATIC
#include <catch.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

TEST_CASE("INIT", "[core]")
{
	REQUIRE(glfwInit());
	REQUIRE(glewInit() == GLEW_OK);
}

TEST_CASE("SHADER_LOAD", "[shader]")
{
	Shader shader("resources/Shaders/basic");

}
