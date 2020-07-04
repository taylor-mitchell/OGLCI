#include "CholeskyFactorizer.h"

void CholeskyFactorizer::factorize(robertbridson::SparseMatrix<float>& mat)
{
	for (int i = 1; i <= mat.n; ++i)
	{
		mat.set_element(i, i, std::sqrt(mat(i, i)));
		for (int j = i + 1; j <= mat.n; ++j)
		{
			if (mat(j, i) != 0)
			{
				mat.set_element(j, i, mat(j, i) / mat(i, i));
			}
		}

		for (int j = i + 1; j <= mat.n; ++j)
		{
			for (int k = j; k <= mat.n; ++k)
			{
				if (mat(k, j) != 0)
				{
					mat.set_element(k, j, mat(k, j) - mat(k, i) * mat(j, i));
				}

			}
		}
	}

	for (int i = 1; i <= mat.n; ++i)
	{
		for (int j = i + 1; j <= mat.n; ++j)
		{
			mat.set_element(i, j, 0.0f);
		}
	}
}
