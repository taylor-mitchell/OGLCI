#include "MLP.h"

void MLP::train()
{
	float averageError = 0.0f;
	for (int i = 0; i < trainingInputs.getHeight(); ++i)
	{
		//averageError += getError(trainingInputs.getRow(i), trainingOutputs.getRow(i));
	}

	averageError /= (float)trainingInputs.getHeight();
}

/*float MLP::getError(std::vector<float> trainingInput, std::vector<float> expectedOutput)
{
	float error = 0.0f;

	Array2D<float> output = process(trainingInput);
	for (int i = 0; i < expectedOutput.size(); ++i)
	{
		error += std::pow(output(0, i) - expectedOutput[i], 2);
	}
	return error;
}*/

Array2D<float> MLP::process(std::vector<float> inputs, std::vector<std::vector<float>>& nodeActivations)
{
	Array2D<float> resultVector;
	Array2D<float> inputVector;
	inputVector.setData(1, inputs.size(), inputs);

	for (int i = 0; i < layerSizes.size(); ++i)
	{
		Array2D<float> biasVector = layerBiases.getColumn(i);
		Matrix::multiply(layerWeights[i], inputVector, resultVector);
		Matrix::add(resultVector, biasVector, resultVector);


		//i should make an Array2D iterator
		for (int j = 0; j < resultVector.getHeight(); ++j)
		{
			resultVector(0, j) = std::max(0.0f, resultVector(0, j));
			nodeActivations[i][j] = resultVector(0, j);
		}
		inputVector = resultVector;
	}

	return resultVector;
}

void MLP::backpropagate()
{	
	Array2D<float> biasAdjustments(layerBiases.getWidth(), layerBiases.getHeight());
	biasAdjustments.setAll(0.0f);

	std::vector<Array2D<float>> weightAdjustments;
	for (int i = 0; i < layerWeights.size(); ++i)
	{
		weightAdjustments[i].setSize(layerWeights[i].getWidth(), layerWeights[i].getHeight());
		weightAdjustments[i].setAll(0.0f);
	}

	for (int i = 0; i < trainingInputs.getHeight(); ++i)
	{		
		std::vector<float> input = trainingInputs.getRow(i);
		std::vector<float> expectedOutput = trainingOutputs.getRow(i);

		std::vector<std::vector<float>> nodeActivations;
		nodeActivations.reserve(layerWeights.size());
		for (int i = 0; i < layerWeights.size(); ++i)
		{
			nodeActivations[i].reserve(layerWeights[i].getHeight());
		}

		Array2D<float> results = process(input, nodeActivations);
	}

}
