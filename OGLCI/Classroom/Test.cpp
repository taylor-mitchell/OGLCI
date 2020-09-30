#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "GLIncludes.h"
#include <glm.hpp>
#include "Logger.h"
//#include "Shader.h"
#include "MatrixFunctions.h"
#include "Array2D.h"

/*
TEST_CASE("INIT", "[core]")
{
	REQUIRE(glfwInit() != 0);
	REQUIRE(glewInit() == GLEW_OK);
}

TEST_CASE("SHADER_LOAD", "[shader]")
{
	Shader shader("resources/Shaders/basic");
}
*/
TEST_CASE("ARRAY2D", "[array]")
{
    INFO_LOG("\n[array] - ARRAY2D\n");
    std::vector<std::vector<float>> matrixData =
        {
            { 1.0f, -1.0f, -1.0f,  0.0f},
            {-1.0f,  1.0f,  0.0f, -1.0f},
            {-1.0f,  0.0f,  1.0f, -1.0f},
            { 0.0f, -1.0f, -1.0f,  1.0f}
        };

    Array2D<float> lhs = matrixData;

    INFO_LOG(lhs.logString());

    for (int i = 0; i < matrixData.size(); ++i)
    {
        for (int j = 0; j < matrixData[0].size(); ++j)
        {
            REQUIRE(matrixData[i][j] == lhs(j, i));
        }
    }
    
}

TEST_CASE("SWAP_ROWS", "[array]")
{
    INFO_LOG("\n[array] - SWAP_ROWS\n");
    std::vector<std::vector<float>> matrixData =
    {
        { 1.0f,  2.0f,  3.0f,  4.0f},
        { 5.0f,  6.0f,  7.0f,  8.0f},
        { 9.0f, 10.0f, 11.0f, 12.0f},
        {13.0f, 14.0f, 15.0f, 16.0f}
    };

    std::vector<std::vector<float>> matrixSolution =
    {
        { 9.0f, 10.0f, 11.0f, 12.0f},
        { 5.0f,  6.0f,  7.0f,  8.0f},        
        { 1.0f,  2.0f,  3.0f,  4.0f},
        {13.0f, 14.0f, 15.0f, 16.0f}
    };

    Array2D<float> lhs = matrixData;
    Array2D<float> rhs = matrixSolution;

    lhs.swapRows(0, 2);

    INFO_LOG("expected:" + rhs.logString());
    INFO_LOG("actual:" + lhs.logString());

    REQUIRE(lhs == rhs);
}

TEST_CASE("SWAP_COLUMNS", "[array]")
{
    INFO_LOG("\n[array] - SWAP_COLUMNS\n");
    std::vector<std::vector<float>> matrixData =
    {
        { 1.0f,  2.0f,  3.0f,  4.0f},
        { 5.0f,  6.0f,  7.0f,  8.0f},
        { 9.0f, 10.0f, 11.0f, 12.0f},
        {13.0f, 14.0f, 15.0f, 16.0f}
    };

    std::vector<std::vector<float>> matrixSolution =
    {
        { 3.0f,  2.0f,  1.0f,  4.0f},
        { 7.0f,  6.0f,  5.0f,  8.0f},
        {11.0f, 10.0f,  9.0f, 12.0f},
        {15.0f, 14.0f, 13.0f, 16.0f}
    };

    Array2D<float> lhs = matrixData;
    Array2D<float> rhs = matrixSolution;

    lhs.swapColumns(0, 2);

    INFO_LOG("expected:" + rhs.logString());
    INFO_LOG("actual:" + lhs.logString());

    REQUIRE(lhs == rhs);
}

TEST_CASE("MATRIX_MULTIPLY", "[matrix]")
{
    INFO_LOG("\n[matrix] - MATRIX_MULTIPLY\n");


}


TEST_CASE("MATRIX_LU", "[matrix]")
{
    INFO_LOG("\n[matrix] - MATRIX_LU\n");
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

    INFO_LOG("expected lower:" + lowerSolution.logString());
    INFO_LOG("actual lower:" + lower.logString());
    INFO_LOG("expected upper:" + upperSolution.logString());
    INFO_LOG("actual upper:" + lower.logString());

    REQUIRE(lower == lowerSolution);
    REQUIRE(upper == upperSolution);
}

TEST_CASE("MATRIX_LU_SOLVE", "[matrix]")
{
    INFO_LOG("\n[matrix] - MATRIX_LU_SOLVE\n");
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

    INFO_LOG("expected:" + resultSolution.logString());
    INFO_LOG("actual:" + result.logString());

    REQUIRE(result == resultSolution);
}
