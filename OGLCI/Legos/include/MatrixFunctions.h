#pragma once
#include "Array2D.h"

//extern bool cudaMultiply(Array2D<float>& left, Array2D<float>& right, Array2D<float>& result);

namespace Matrix {
	//lol there has to be a better way to do this
	struct TwoValueTwoDimDerivativeResult
	{
		float du_dx;
		float du_dy;
		float dv_dx;
		float dv_dy;
		TwoValueTwoDimDerivativeResult() : du_dx(0.0f), du_dy(0.0f), dv_dx(0.0f), dv_dy(0.0f) {};
		TwoValueTwoDimDerivativeResult(glm::vec4 vec)
		{
			du_dx = vec.x;
			du_dy = vec.z;
			dv_dx = vec.y;
			dv_dy = vec.w;
		}
	};
	struct OneValueTwoDimDerivativeResult
	{
		float du_dx;
		float du_dy;
		OneValueTwoDimDerivativeResult() : du_dx(0.0f), du_dy(0.0f) {};
		OneValueTwoDimDerivativeResult(glm::vec2 vec)
		{
			du_dx = vec.x;
			du_dy = vec.y;
		}
	};
	struct TwoValueTwoDimMixedDerivativeResult
	{
		float du_dxdy;
		float dv_dxdy;
		float du_dydx;
		float dv_dydx;
		TwoValueTwoDimMixedDerivativeResult() : du_dxdy(0.0f), dv_dxdy(0.0f), du_dydx(0.0f), dv_dydx(0.0f) {};
		TwoValueTwoDimMixedDerivativeResult(glm::vec4 vec)
		{
			du_dxdy = vec.x;
			dv_dxdy = vec.y;
			du_dydx = vec.z;
			dv_dydx = vec.w;
		};		
	};
	struct OneValueTwoDimMixedDerivativeResult
	{
		float du_dxdy;
		float du_dydx;
		OneValueTwoDimMixedDerivativeResult() : du_dxdy(0.0f), du_dydx(0.0f) {};
		OneValueTwoDimMixedDerivativeResult(glm::vec2 vec)
		{
			du_dxdy = vec.x;
			du_dydx = vec.y;
		}
	};
	Array2D<double> identity(unsigned int n);
	bool decomposeLU(Array2D<double>& matrix, Array2D<double>& lower, Array2D<double>& upper, Array2D<double>& permutation);
	void solveLU(Array2D<double>& permutation, Array2D<double>& lower, Array2D<double>& upper, Array2D<double>& rhs, Array2D<double>& result);
	void multiply(Array2D<double>& leftMatrix, Array2D<double>& rightMatrix, Array2D<double>& result);
	void cfMultiply(Array2D<double>& leftMatrix, Array2D<double>& rightMatrix, int cacheSizeBytes, Array2D<double>& result);
	void blasMultiply(Array2D<double>& leftMatrix, Array2D<double>& rightMatrix, Array2D<double>& result);
	void fastMultiply(Array2D<double>& leftMatrix, Array2D<double>& rightMatrix, Array2D<double>& result);
	void add(Array2D<double>& leftMatrix, Array2D<double>& rightMatrix, Array2D<double>& result);
	void subtract(Array2D<double>& leftMatrix, Array2D<double>& rightMatrix, Array2D<double>& result);
	void power(Array2D<double>& matrix, double exponent, Array2D<double>& result);
	double summation(Array2D<double>& matrix);
	bool cudaMultiply(Array2D<double>& leftMatrix, Array2D<double>& rightMatrix, Array2D<double>& result);
	Array2D<glm::vec4> gradient(Array2D<glm::vec2>);
	double distance1(Array2D<double>& leftMatrix, Array2D<double>& rightMatrix);
	double norm1(Array2D<double>& matrix);
	double compare(Array2D<double>& leftMatrix, Array2D<double>& rightMatrix);
	Array2D<double> standardDeviation(std::vector<Array2D<double>> data);
	Array2D<TwoValueTwoDimDerivativeResult> derivative(Array2D<glm::vec2>, float deltaX, float deltaY);
	Array2D<OneValueTwoDimDerivativeResult> derivative(Array2D<float>, float deltaX, float deltaY);
	Array2D<TwoValueTwoDimDerivativeResult> secondDerivative(Array2D<glm::vec2>, float deltaX, float deltaY);
	Array2D<OneValueTwoDimDerivativeResult> secondDerivative(Array2D<float>, float deltaX, float deltaY);
	Array2D<TwoValueTwoDimMixedDerivativeResult> mixedDerivative(Array2D<glm::vec2>, float deltaX, float deltaY);
	Array2D<OneValueTwoDimMixedDerivativeResult> mixedDerivative(Array2D<float>, float deltaX, float deltaY);


}