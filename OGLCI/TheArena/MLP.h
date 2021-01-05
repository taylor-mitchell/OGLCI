#pragma once
#include <vector>
#include "Array2D.h"
#include "MatrixFunctions.h"


class MLP
{
private:
	int numInputs;
	int numOutputs;
	std::vector<int> layerSizes;
	std::vector<Array2D<float>> layerWeights;
	Array2D<float> layerBiases;
	Array2D<float> trainingInputs;
	Array2D<float> trainingOutputs;

public:
	MLP() : numInputs(0), numOutputs(0), layerSizes(), layerWeights(), layerBiases(), trainingInputs(), trainingOutputs() {};
	MLP(int inputs, int outputs, std::vector<int> layers) : numInputs(inputs), numOutputs(outputs), layerSizes(layers), trainingInputs(), trainingOutputs() {};
	MLP(int inputs, int outputs, std::vector<int> layers, Array2D<float> trInputs, Array2D<float> trOutputs) : numInputs(inputs), numOutputs(outputs), layerSizes(layers), trainingInputs(trInputs), trainingOutputs(trOutputs) {};


	void train();
	//float getError(std::vector<float> trainingInput, std::vector<float> expectedOutput);

	Array2D<float> process(std::vector<float> inputs, std::vector<std::vector<float>>& nodeActivations);
	void backpropagate();
};