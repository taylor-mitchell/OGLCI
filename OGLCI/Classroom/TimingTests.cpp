#include <catch.hpp>
#include "TestUtils.h"
#include "Logger.h"
#include "Array2D.h"
#include "MatrixFunctions.h"
#include <time.h>
#include "Table.h"

//TEST_CASE("MATRIX_LU_PROFILING", "[profiling]")
//{
//    INFO_LOG("\n[profiling] - MATRIX_LU_PROFILING\n");
//    unsigned int totalReps = 100;
//    std::vector<std::vector<float>> matrixData =
//    {
//        { 1.0f, -1.0f, -1.0f,  0.0f},
//        {-1.0f,  1.0f,  0.0f, -1.0f},
//        {-1.0f,  0.0f,  1.0f, -1.0f},
//        { 0.0f, -1.0f, -1.0f,  1.0f}
//    };
//    std::vector<std::vector<float>> lowerData =
//    {
//        { 1.0f,  0.0f,  0.0f,  0.0f},
//        {-1.0f,  1.0f,  0.0f,  0.0f},
//        {-1.0f,  0.0f,  1.0f,  0.0f},
//        { 0.0f,  1.0f,  1.0f,  1.0f}
//    };
//    std::vector<std::vector<float>> upperData =
//    {
//        { 1.0f, -1.0f, -1.0f,  0.0f},
//        { 0.0f, -1.0f,  0.0f, -1.0f},
//        { 0.0f,  0.0f, -1.0f, -1.0f},
//        { 0.0f,  0.0f,  0.0f,  3.0f}
//    };
//
//    Array2D<float> mat = matrixData;
//    Array2D<float> lowerSolution = lowerData;
//    Array2D<float> upperSolution = upperData;
//    Array2D<float> upper;
//    Array2D<float> lower;
//    Array2D<float> permutation;
//
//    std::chrono::duration<float, std::milli> totalDuration;
//    for (unsigned int i = 0; i < totalReps; ++i)
//    {
//        totalDuration += measure<std::milli>::duration(Matrix::decomposeLU, mat, lower, upper, permutation);
//    }
//
//    float averageDuration = totalDuration.count() / static_cast<float>(totalReps);
//
//
//    INFO_LOG("Average: " + std::to_string(averageDuration));
//
//    REQUIRE(averageDuration < 1.0f);
//}

TEST_CASE("MATRIX_MULT_ASYMPTOTIC_COMPARE", "[profiling]")
{
    INFO_LOG("\n[profiling] - MATRIX_MULT_ASYMPTOTIC_COMPARE\n");
    unsigned int totalReps = 100;
    unsigned int startSize = 5;
    unsigned int totalMagnitudes = 8;
    float maxValue = 100.0f;
    float minValue = -100.0f;

    Table<float> timingResults("matrix multiplication timing results", { "N", "average cpu duration(milliseconds)", "average gpu duration(milliseconds)", "average percent difference" });

    srand(time(NULL));

    for (int i = 0; i < totalMagnitudes; ++i)
    {        
        int currentSize = startSize * std::pow(2.0f, i);
        std::chrono::duration<float, std::milli> cpuTotalDuration;
        std::chrono::duration<float, std::milli> gpuTotalDuration;
        float comparison = 0.0f;

        for (int j = 0; j < totalReps; ++j)
        {

            Array2D<float> leftMatrix(currentSize, currentSize);
            Array2D<float> rightMatrix(currentSize, currentSize);
            Array2D<float> cpuMultResult;
            Array2D<float> gpuMultResult;

            //populate matrices with random numbers
            for (Array2D<float>::iterator it = leftMatrix.begin(); it != leftMatrix.end(); ++it)
            {
                *it = ((float)rand() / (float)RAND_MAX * (maxValue - minValue)) + minValue;
            }
            for (Array2D<float>::iterator it = rightMatrix.begin(); it != rightMatrix.end(); ++it)
            {
                *it = ((float)rand() / (float)RAND_MAX * (maxValue - minValue)) + minValue;
            }


            cpuTotalDuration += measure<std::milli>::duration(Matrix::fastMultiply, leftMatrix, rightMatrix, cpuMultResult);
            gpuTotalDuration += measure<std::milli>::duration(Matrix::cudaMultiply, leftMatrix, rightMatrix, gpuMultResult);
            comparison += Matrix::compare(cpuMultResult, gpuMultResult);
        }

        timingResults.addRow(std::vector<float>({ (float)currentSize, cpuTotalDuration.count() / static_cast<float>(totalReps), gpuTotalDuration.count() / static_cast<float>(totalReps),  comparison / static_cast<float>(totalReps)}));

        INFO_LOG("completed " + std::to_string(i + 1) + " / " + std::to_string(totalMagnitudes));
    }

    INFO_LOG(timingResults.logString());
}

