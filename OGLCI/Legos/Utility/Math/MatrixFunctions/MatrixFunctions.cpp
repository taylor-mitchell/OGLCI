#include "MatrixFunctions.h"
#include "Logger.h"

namespace Matrix
{

	Array2D<float> identity(unsigned int n)
	{
		Array2D<float> toReturn(n, n);
		toReturn.setAll(0.0f);
		for (int i = 0; i < n; ++i)
		{
			toReturn.set(i, i, 1.0f);
		}
		return toReturn;
	}

	bool decomposeLU(Array2D<float>& matrix, Array2D<float>& lower, Array2D<float>& upper, Array2D<float>& permutation)
	{
		int count = 0;
		int n = matrix.getWidth();
		upper = matrix;

		//assuming square only for now
		lower.setSize(n, n);
		lower.setAll(0.0f);

		permutation = Matrix::identity(n);

		for (int i = 0; i < n; ++i)
		{
			float max = fabs(upper(i, i));
			int maxIndex = i;
			for (int j = i + 1; j < n; ++j)
			{
				if (fabs(upper(i, j)) > max)
				{
					max = fabs(upper(i, j));
					maxIndex = j;
				}
			}

			if (maxIndex != i)
			{
				upper.swapRows(i, maxIndex);
				permutation.swapRows(i, maxIndex);
			}

			Array2D<float> mMatrix = Matrix::identity(n);
			for (int j = i + 1; j < n; ++j)
			{
				if (upper(i, j) != 0.0f)
				{					
					mMatrix(i, j) = upper(i, j) / upper(i, i) * -1.0f;
				}
			}

			Array2D<float> tempMatrix;
			Matrix::multiply(mMatrix, upper, tempMatrix);
			upper = tempMatrix;
			INFO(std::to_string(count));
			count++;
		}

		INFO("other thing");
		count = 0;
		Array2D<float> tempMatrix;
		Matrix::multiply(permutation, matrix, tempMatrix);
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j <= i; ++j)
			{
				if (i == j)
				{
					lower(j, i) = 1.0f;
				}
				else
				{
					float sum = 0.0f;
					for (int k = 0; k < j; ++k)
					{
						sum += lower(k, i) * upper(j, k);
					}					
					lower(j, i) = (tempMatrix(j, i) - sum) / upper(j, j);
				}
			}
			INFO(std::to_string(count));
			count++;
		}
		return true;
	}

	void solveLU(Array2D<float>& permutation, Array2D<float>& lower, Array2D<float>& upper, Array2D<float>& rhs, Array2D<float>& result)
	{
		Array2D<float> substitutionRhs;
		Array2D<float> tempRhs;

		Matrix::multiply(permutation, rhs, tempRhs);
		rhs = tempRhs;

		int n = rhs.getHeight();
		substitutionRhs.setSize(1, n);
		substitutionRhs.setAll(0.0f);
		substitutionRhs(0, 0) = rhs(0, 0);
		for (int i = 1; i < n; ++i)
		{
			float total = rhs(0, i);
			for (int j = 0; j < i; ++j)
			{
				total -= lower(j, i) * substitutionRhs(0, j);
			}
			substitutionRhs(0, i) = total;
		}

		for (int i = 0; i < substitutionRhs.getHeight(); ++i)
		{
			INFO(std::to_string(substitutionRhs(0, i)));
		}

		result.setSize(1, n);
		result(0, n - 1) = substitutionRhs(0, n - 1) / upper(n - 1, n - 1);
		for (int i = n - 2; i >= 0; --i)
		{
			float total = substitutionRhs(0, i);
			for (int j = n - 1; j > i; --j)
			{
				total -= upper(j, i) * result(0, j);
			}
			result(0, i) = total / upper(i, i);
		}
	}

	void multiply(Array2D<float>& leftMatrix, Array2D<float>& rightMatrix, Array2D<float>& result)
	{		
		if (leftMatrix.getWidth() != rightMatrix.getHeight())
		{
			return;
		}

		result.setSize(rightMatrix.getWidth(), leftMatrix.getHeight());
		result.setAll(0.0f);

		for (int i = 0; i < leftMatrix.getHeight(); ++i)
		{
			for (int j = 0; j < rightMatrix.getWidth(); ++j)
			{
				float total = 0.0f;
				for (int k = 0; k < leftMatrix.getWidth(); ++k)
				{
					total += leftMatrix(k, i) * rightMatrix(j, k);
				}
				result(j, i) = total;
			}
		}
	}
}
