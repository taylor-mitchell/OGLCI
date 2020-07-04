#pragma once

#include "SparseMatrix.h"
class CholeskyFactorizer
{
private:

public:
	CholeskyFactorizer() {};

	void factorize(robertbridson::SparseMatrix<float>&);

};