#include "MLP.h"
#include <time.h>

void MLP::initialize()
{
	int maxValue = 1.0f;
	int minValue = -1.0f;
	srand(time(NULL));
	for (unsigned int i = 0; i < layerWeights.size(); ++i)
	{
		for (Array2D<float>::iterator it = layerWeights[i].begin(); it != layerWeights[i].end(); ++it)
		{
			*it = ((float)rand() / (float)RAND_MAX * (maxValue - minValue)) + minValue;
		}
	}
	for (unsigned int i = 0; i < layerBiases.getWidth(); ++i)
	{
		for (unsigned int j = 0; j < layerBiases.getHeight(i); ++j)
		{
			layerBiases(i, j) = ((float)rand() / (float)RAND_MAX * (maxValue - minValue)) + minValue;
		}
	}
}

void MLP::train(std::vector<Array2D<float>> trainingData, std::vector<float> labels)
{

}


Array2D<float> MLP::process(Array2D<float> inputs, WavyArray2D<float>& nodeActivations)
{

	Array2D<float> resultVector;
	/*
	Array2D<float> inputVector(inputs);

	nodeActivations.setWidth(layerSizes.size());
	for (unsigned int i = 0; i < layerSizes.size(); ++i)
	{
		nodeActivations.setColumnSize(i, layerSizes[i]);
	}

	for (int i = 0; i < layerSizes.size(); ++i)
	{
		Array2D<float> biasVector = layerBiases.getColumn(i);
		Matrix::fastMultiply(layerWeights[i], inputVector, resultVector);
		Matrix::add(resultVector, biasVector, resultVector);

		for (int j = 0; j < resultVector.getHeight(); ++j)
		{
			resultVector(0, j) = std::max(0.0f, resultVector(0, j));
			nodeActivations(i, j) = resultVector(0, j);
		}
		inputVector = resultVector;
	}
	*/
	return resultVector;
}

void MLP::backpropagate()
{	
	WavyArray2D<float> biasAdjustments(layerSizes);
	biasAdjustments.setAll(0.0f);

	std::vector<Array2D<float>> averageWeightAdjustments;
	WavyArray2D<float> averageLayerBiasAdjustments;
	averageLayerBiasAdjustments.setAll(0.0f);
	for (int i = 0; i < layerWeights.size(); ++i)
	{
		averageWeightAdjustments[i].setSize(layerWeights[i].getWidth(), layerWeights[i].getHeight());
		averageWeightAdjustments[i].setAll(0.0f);
	}

	/*for (int i = 0; i < trainingInputs.getHeight(); ++i)
	{		
		Array2D<float> input = trainingInputs.getColumn(i);
		Array2D<float> expectedOutput = trainingOutputs.getColumn(i);
		WavyArray2D<float> nodeActivations(layerSizes);

		Array2D<float> results = process(input, nodeActivations);

		Array2D<float> costMatrix;
		Matrix::subtract(results, expectedOutput, costMatrix);
		Matrix::power(costMatrix, 2.0f, costMatrix);
		float cost = Matrix::summation(costMatrix);

	}*/

}
