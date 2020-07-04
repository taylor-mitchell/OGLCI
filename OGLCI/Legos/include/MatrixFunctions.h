#pragma once
#include "Array2D.h"

namespace Matrix {
	Array2D<float> identity(unsigned int n);
	bool decomposeLU(Array2D<float>& matrix, Array2D<float>& lower, Array2D<float>& upper, Array2D<float>& permutation);
	void solveLU(Array2D<float>& permutation, Array2D<float>& lower, Array2D<float>& upper, Array2D<float>& rhs, Array2D<float>& result);
	void multiply(Array2D<float>& leftMatrix, Array2D<float>& rightMatrix, Array2D<float>& result);
}