#define CATCH_CONFIG_MAIN
#define GLEW_STATIC
#include <catch.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include "Shader.h"
#include "MatrixFunctions.h"
#include "Array2D.h"

/*
TEST_CASE("INIT", "[core]")
{
	REQUIRE(glfwInit());
	REQUIRE(glewInit() == GLEW_OK);
}
*/

/*TEST_CASE("SHADER_LOAD", "[shader]")
{
	Shader shader("resources/Shaders/basic");

}*/

TEST_CASE("ARRAY2D", "[array]")
{
    std::vector<std::vector<float>> matrixData =
        {
            { 1.0f, -1.0f, -1.0f,  0.0f},
            {-1.0f,  1.0f,  0.0f, -1.0f},
            {-1.0f,  0.0f,  1.0f, -1.0f},
            { 0.0f, -1.0f, -1.0f,  1.0f}
        };

    Array2D<float> lhs = matrixData;
    Array2D<float> rhs = matrixData;   

    REQUIRE(lhs == rhs);
}


TEST_CASE("MATRIX_LU", "[matrix]")
{
    std::vector<std::vector<float>> matrixData = 
        {   
            { 1.0f, -1.0f, -1.0f,  0.0f},
            {-1.0f,  1.0f,  0.0f, -1.0f},
            {-1.0f,  0.0f,  1.0f, -1.0f},
            { 0.0f, -1.0f, -1.0f,  1.0f} 
        };
    std::vector<std::vector<float>> lowerData =
        {
            { 1.0f,  0.0f,  0.0f,  0.0f},
            {-1.0f,  1.0f,  0.0f,  0.0f},
            {-1.0f,  0.0f,  1.0f,  0.0f},
            { 0.0f,  1.0f,  1.0f,  1.0f}
        };
    std::vector<std::vector<float>> upperData =
        {
            { 1.0f, -1.0f, -1.0f,  0.0f},
            { 0.0f, -1.0f,  0.0f, -1.0f},
            { 0.0f,  0.0f, -1.0f, -1.0f},
            { 0.0f,  0.0f,  0.0f,  3.0f}
        };

    Array2D<float> mat = matrixData;
    Array2D<float> lowerSolution = lowerData;
    Array2D<float> upperSolution = upperData;
    Array2D<float> upper;
    Array2D<float> lower;    
    Array2D<float> permutation;

    Matrix::decomposeLU(mat, lower, upper, permutation);    

    REQUIRE(lower == lowerSolution);
    REQUIRE(upper == upperSolution);
}

TEST_CASE("MATRIX_LU_SOLVE", "[matrix]")
{
    std::vector<std::vector<float>> matrixData =
        {
            { 1.0f, -1.0f, -1.0f,  0.0f},
            {-1.0f,  1.0f,  0.0f, -1.0f},
            {-1.0f,  0.0f,  1.0f, -1.0f},
            { 0.0f, -1.0f, -1.0f,  1.0f}
        };
    std::vector<std::vector<float>> rhsData = 
        { 
            { 10.0f}, 
            {5.0f}, 
            {7.0f}, 
            {2.0f} 
        };
    std::vector <std::vector<float>> resultSolutionData = 
        { 
            {-2.0f}, 
            {-7.0f}, 
            {-5.0f}, 
            {-10.0f} 
        };

    Array2D<float> rhs = rhsData;
    Array2D<float> mat = matrixData;
    Array2D<float> lower;
    Array2D<float> upper;
    Array2D<float> permutation;
    Array2D<float> result;
    Array2D<float> resultSolution = resultSolutionData;

    Matrix::decomposeLU(mat, lower, upper, permutation);

    Matrix::solveLU(permutation, lower, upper, rhs, result);

    REQUIRE(result == resultSolution);
}
