#include <catch.hpp>
#include "TestUtils.h"
#include "Logger.h"
#include "Array2D.h"
#include "MatrixFunctions.h"
#include <time.h>
#include "Table.h"
#include "SystemHelper.h"
#include <armadillo>

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


TEST_CASE("MATRIX_MULT_ARMADILLO_OPENBLAS_COMPARE", "[profiling]")
{
    INFO_LOG("\n[profiling] - MATRIX_MULT_ARMADILLO_OPENBLAS_COMPARE\n");
    srand(time(NULL));
    unsigned int totalReps = 100;
    unsigned int startSize = 5;
    unsigned int totalMagnitudes = 8;
    float maxValue = 100.0f;
    float minValue = -100.0f;
    int cacheSizeBytes = cacheSize(1);
    Table<float> timingResults("matrix multiplication timing results", { "N", "avg lego time(ms)", "avg cache friendly lego time(ms)", "avg armadillo time(ms)", "avg openblas time(ms)"});

    for (int i = 0; i < totalMagnitudes; ++i)
    {
        int currentSize = startSize * std::pow(2.0f, i);
        std::chrono::duration<float, std::milli> legoTotalDuration;
        std::chrono::duration<float, std::milli> armadilloTotalDuration;
        std::chrono::duration<float, std::milli> legoCFTotalDuration;
        std::chrono::duration<float, std::milli> openBlasTotalDuration;

        for (int j = 0; j < totalReps; ++j)
        {
            Array2D<double> leftMatrix(currentSize, currentSize);
            Array2D<double> rightMatrix(currentSize, currentSize);
            arma::fmat armaLeft(currentSize, currentSize, arma::fill::randu);
            arma::fmat armaRight(currentSize, currentSize, arma::fill::randu);
            Array2D<double> cpuMultResult(currentSize, currentSize);
            Array2D<double> cpuCFMultResult(currentSize, currentSize);
            Array2D<double> openBlasMultResult(currentSize, currentSize);
            arma::fmat armaMultResult;

            //populate input matrices with random numbers
            for (Array2D<double>::iterator it = leftMatrix.begin(); it != leftMatrix.end(); ++it)
            {
                *it = ((double)rand() / (double)RAND_MAX * (maxValue - minValue)) + minValue;
            }
            for (Array2D<double>::iterator it = rightMatrix.begin(); it != rightMatrix.end(); ++it)
            {
                *it = ((double)rand() / (double)RAND_MAX * (maxValue - minValue)) + minValue;
            }
            std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
            armaMultResult = armaLeft * armaRight;
            armadilloTotalDuration += std::chrono::duration<float, std::milli>(std::chrono::steady_clock::now() - start);
            legoTotalDuration += measure<std::milli>::duration(Matrix::multiply, leftMatrix, rightMatrix, cpuMultResult);
            legoCFTotalDuration += measure<std::milli>::duration(Matrix::cfMultiply, leftMatrix, rightMatrix, cacheSizeBytes, cpuCFMultResult);
            openBlasTotalDuration += measure<std::milli>::duration(Matrix::blasMultiply, leftMatrix, rightMatrix, openBlasMultResult);
        }
        timingResults.addRow(std::vector<float>({ (float)currentSize, legoTotalDuration.count() / static_cast<float>(totalReps), legoCFTotalDuration.count() / static_cast<float>(totalReps), armadilloTotalDuration.count() / static_cast<float>(totalReps), openBlasTotalDuration.count() / static_cast<float>(totalReps) }));
        INFO_LOG("completed " + std::to_string(i + 1) + " / " + std::to_string(totalMagnitudes));
    }
    INFO_LOG(timingResults.logString());

}

/*

TEST_CASE("MATRIX_MULT_ASYMPTOTIC_COMPARE", "[profiling]")
{
    INFO_LOG("\n[profiling] - MATRIX_MULT_ASYMPTOTIC_COMPARE\n");
    srand(time(NULL));
    unsigned int totalReps = 100;
    unsigned int startSize = 5;
    unsigned int totalMagnitudes = 8;
    float maxValue = 100.0f;
    float minValue = -100.0f;
    int cacheSizeBytes = cacheSize(1);
    Table<float> timingResults("matrix multiplication timing results", { "N", "avg cpu time(ms)", "avg cache friendly cpu time(ms)", "avg gpu time(ms)", "avg result std dev", "max result std dev" });    

    for (int i = 0; i < totalMagnitudes; ++i)
    {        
        int currentSize = startSize * std::pow(2.0f, i);
        std::chrono::duration<float, std::milli> cpuTotalDuration;
        std::chrono::duration<float, std::milli> cpuCFTotalDuration;
        std::chrono::duration<float, std::milli> gpuTotalDuration;
        float maxStdDev = 0.0f;
        float comparison = 0.0f;

        for (int j = 0; j < totalReps; ++j)
        {
            Array2D<float> leftMatrix(currentSize, currentSize);
            Array2D<float> rightMatrix(currentSize, currentSize);
            Array2D<float> cpuMultResult(currentSize, currentSize);
            Array2D<float> cpuCFMultResult(currentSize, currentSize);
            Array2D<float> gpuMultResult(currentSize, currentSize);

            //populate input matrices with random numbers
            for (Array2D<float>::iterator it = leftMatrix.begin(); it != leftMatrix.end(); ++it)
            {
                *it = ((float)rand() / (float)RAND_MAX * (maxValue - minValue)) + minValue;
            }
            for (Array2D<float>::iterator it = rightMatrix.begin(); it != rightMatrix.end(); ++it)
            {
                *it = ((float)rand() / (float)RAND_MAX * (maxValue - minValue)) + minValue;
            }

            cpuTotalDuration += measure<std::milli>::duration(Matrix::multiply, leftMatrix, rightMatrix, cpuMultResult);
            cpuCFTotalDuration += measure<std::milli>::duration(Matrix::cfMultiply, leftMatrix, rightMatrix, cacheSizeBytes, cpuCFMultResult);
            gpuTotalDuration += measure<std::milli>::duration(Matrix::cudaMultiply, leftMatrix, rightMatrix, gpuMultResult);

            //calculates the standard deviation for each entry between the three result matrices
            Array2D<float> stdDev = Matrix::standardDeviation({ cpuMultResult , cpuCFMultResult, gpuMultResult });

            float avgStdDev = 0.0f;
            for (int ii = 0; ii < currentSize; ++ii)
            {
                for (int jj = 0; jj < currentSize; ++jj)
                {
                    if (stdDev(ii, jj) > maxStdDev)
                    {
                        maxStdDev = stdDev(ii, jj);
                    }

                    avgStdDev += stdDev(ii, jj);
                }
            }
            avgStdDev /= std::pow(currentSize, 2.0f);
            comparison += avgStdDev;
        }
        timingResults.addRow(std::vector<float>({ (float)currentSize, cpuTotalDuration.count() / static_cast<float>(totalReps), cpuCFTotalDuration.count() / static_cast<float>(totalReps), gpuTotalDuration.count() / static_cast<float>(totalReps),  comparison / static_cast<float>(totalReps), maxStdDev }));
        INFO_LOG("completed " + std::to_string(i + 1) + " / " + std::to_string(totalMagnitudes));
    }
    INFO_LOG(timingResults.logString());
}

TEST_CASE("ARRAY2D_ACCESS_TIMING", "[profiling]")
{
    INFO_LOG("\n[profiling] - ARRAY2D_ACCESS_TIMING\n");
    srand(time(NULL));
    Table<float> timingResults("array2d access timing results", { "N", "array2d", "bare array" });
    int currentSize = 50;
    float maxValue = 100.0f;
    float minValue = -100.0f;
    unsigned int totalReps = 100;

    Array2D<float> leftMatrix(currentSize, currentSize);
    Array2D<float> rightMatrix(currentSize, currentSize);
    Array2D<float> resultMatrix(currentSize, currentSize);

    float* leftArray = new float[currentSize * currentSize];
    float* rightArray = new float[currentSize * currentSize];
    float* resultArray = new float[currentSize * currentSize];

    for (int i = 0; i < currentSize; ++i)
    {
        for (int j = 0; j < currentSize; ++j)
        {
            float leftValue = ((float)rand() / (float)RAND_MAX * (maxValue - minValue)) + minValue;
            float rightValue = ((float)rand() / (float)RAND_MAX * (maxValue - minValue)) + minValue;
            leftMatrix(i, j) = leftValue;
            leftArray[j * currentSize + i] = leftValue;
            rightMatrix(i, j) = rightValue;
            rightArray[j * currentSize + i] = rightValue;
        }
    }

    std::chrono::duration<float, std::milli> array2DTotalDuration;
    std::chrono::duration<float, std::milli> bareArrayTotalDuration;

    for (int k = 0; k < totalReps; ++k)
    {
        for (int i = 0; i < currentSize; ++i)
        {
            for (int j = 0; j < currentSize; ++j)
            {
                std::chrono::time_point<std::chrono::steady_clock> array2Dstart = std::chrono::steady_clock::now();
                resultMatrix(i, j) = leftMatrix(i, j) * rightMatrix(i, j);
                array2DTotalDuration += std::chrono::duration<float, std::milli>(std::chrono::steady_clock::now() - array2Dstart);

                std::chrono::time_point<std::chrono::steady_clock> bareArraystart = std::chrono::steady_clock::now();
                resultArray[j * currentSize + i] = leftArray[j * currentSize + i] * rightArray[j * currentSize + i];
                bareArrayTotalDuration += std::chrono::duration<float, std::milli>(std::chrono::steady_clock::now() - bareArraystart);
            }
        }
    }

    timingResults.addRow(std::vector<float>({ (float)currentSize, array2DTotalDuration.count() / static_cast<float>(totalReps), bareArrayTotalDuration.count() / static_cast<float>(totalReps) }));
    INFO_LOG(timingResults.logString());
}

*/

