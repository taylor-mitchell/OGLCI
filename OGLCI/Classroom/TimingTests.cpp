#include <catch.hpp>
#include "TestUtils.h"
#include "Logger.h"
#include "Array2D.h"
#include "MatrixFunctions.h"

TEST_CASE("MATRIX_LU_PROFILING", "[profiling]")
{
    INFO_LOG("\n[profiling] - MATRIX_LU_PROFILING\n");
    unsigned int totalReps = 100;
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

    std::chrono::duration<float, std::milli> totalDuration;
    for (unsigned int i = 0; i < totalReps; ++i)
    {
        totalDuration += measure<std::milli>::duration(Matrix::decomposeLU, mat, lower, upper, permutation);
    }

    float averageDuration = totalDuration.count() / static_cast<float>(totalReps);


    INFO_LOG("Average: " + std::to_string(averageDuration));

    REQUIRE(averageDuration < 1.0f);
}