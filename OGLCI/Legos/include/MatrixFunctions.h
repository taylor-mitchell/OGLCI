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
	Array2D<float> identity(unsigned int n);
	bool decomposeLU(Array2D<float>& matrix, Array2D<float>& lower, Array2D<float>& upper, Array2D<float>& permutation);
	void solveLU(Array2D<float>& permutation, Array2D<float>& lower, Array2D<float>& upper, Array2D<float>& rhs, Array2D<float>& result);
	void multiply(Array2D<float>& leftMatrix, Array2D<float>& rightMatrix, Array2D<float>& result);
	void cfMultiply(Array2D<float>& leftMatrix, Array2D<float>& rightMatrix, int cacheSizeBytes, Array2D<float>& result);
	void fastMultiply(Array2D<float>& leftMatrix, Array2D<float>& rightMatrix, Array2D<float>& result);
	void add(Array2D<float>& leftMatrix, Array2D<float>& rightMatrix, Array2D<float>& result);
	bool cudaMultiply(Array2D<float>& leftMatrix, Array2D<float>& rightMatrix, Array2D<float>& result);
	Array2D<glm::vec4> gradient(Array2D<glm::vec2>);
	float distance1(Array2D<float>& leftMatrix, Array2D<float>& rightMatrix);
	float norm1(Array2D<float>& matrix);
	float compare(Array2D<float>& leftMatrix, Array2D<float>& rightMatrix);
	Array2D<TwoValueTwoDimDerivativeResult> derivative(Array2D<glm::vec2>, float deltaX, float deltaY);
	Array2D<OneValueTwoDimDerivativeResult> derivative(Array2D<float>, float deltaX, float deltaY);
	Array2D<TwoValueTwoDimDerivativeResult> secondDerivative(Array2D<glm::vec2>, float deltaX, float deltaY);
	Array2D<OneValueTwoDimDerivativeResult> secondDerivative(Array2D<float>, float deltaX, float deltaY);
	Array2D<TwoValueTwoDimMixedDerivativeResult> mixedDerivative(Array2D<glm::vec2>, float deltaX, float deltaY);
	Array2D<OneValueTwoDimMixedDerivativeResult> mixedDerivative(Array2D<float>, float deltaX, float deltaY);


}