#pragma once
#include <vector>
#include "Array2D.h"
#include "MatrixFunctions.h"
#include "WavyArray2D.h"


class MLP
{
private:
	int numInputs;
	int numOutputs;
	std::vector<unsigned int> layerSizes;
	std::vector<Array2D<float>> layerWeights;
	WavyArray2D<float> layerBiases;
	WavyArray2D<float> biasAdjustments;
	std::vector<Array2D<float>> averageWeightAdjustments;
	WavyArray2D<float> averageLayerBiasAdjustments;

public:
	MLP() : numInputs(0), numOutputs(0), layerSizes(), layerWeights(), layerBiases(), biasAdjustments(), averageWeightAdjustments(), averageLayerBiasAdjustments() {};
	MLP(int inputs, int outputs, std::vector<unsigned int> layers) : numInputs(inputs), numOutputs(outputs), layerSizes(layers), biasAdjustments(layers) { biasAdjustments.setAll(0.0f); };

	void initialize();
	void train(std::vector<Array2D<float>> trainingData, std::vector<float> labels);
	//float getError(std::vector<float> trainingInput, std::vector<float> expectedOutput);

	Array2D<float> process(Array2D<float> inputs, WavyArray2D<float>& nodeActivations);
	void backpropagate();
};