#include "MatrixFunctions.h"
#include "Logger.h"

extern bool cudaMultiply(Array2D<float>& left, Array2D<float>& right, Array2D<float>& result);
namespace Matrix
{
	Array2D<float> identity(unsigned int n)
	{
		Array2D<float> toReturn(n, n);
		toReturn.setAll(0.0f);
		for (unsigned int i = 0; i < n; ++i)
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
		}

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

		for (unsigned int i = 0; i < leftMatrix.getHeight(); ++i)
		{
			for (unsigned int j = 0; j < rightMatrix.getWidth(); ++j)
			{
				float total = 0.0f;
				for (unsigned int k = 0; k < leftMatrix.getWidth(); ++k)
				{
					total += leftMatrix(k, i) * rightMatrix(j, k);
				}
				result(j, i) = total;
			}
		}
	}
	Array2D<glm::vec4> gradient(Array2D<glm::vec2> arr)
	{
		Array2D<glm::vec4> toReturn(arr.getWidth(), arr.getHeight());

		for (int i = 0; i < toReturn.getWidth(); ++i)
		{
			toReturn(i, 0) = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}
		for (int i = 0; i < toReturn.getWidth(); ++i)
		{
			toReturn(i, toReturn.getHeight() - 1) = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}
		for (int i = 0; i < toReturn.getHeight(); ++i)
		{
			toReturn(0, i) = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}
		for (int i = 0; i < toReturn.getHeight(); ++i)
		{
			toReturn(toReturn.getWidth() - 1, i) = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}

		for (int i = 1; i < arr.getWidth() - 1; ++i)
		{
			for (int j = 1; j < arr.getHeight() - 1; ++j)
			{
				toReturn(i, j) = glm::vec4(0.5f * (arr(i - 1, j).x + arr(i + 1, j).x), 0.5f * (arr(i, j - 1).y + arr(i, j + 1).y), 0.5f * (arr(i, j - 1).x + arr(i, j + 1).x), 0.5f * (arr(i - 1, j).y + arr(i + 1, j).y));
			}
		}
		return toReturn;
	}

	/*
		returns a vec4 where the first two entries are dV/dx and the second two entries are dV/dy
	*/
	Array2D<TwoValueTwoDimDerivativeResult> derivative(Array2D<glm::vec2> arr, float deltaX, float deltaY)
	{
		Array2D<TwoValueTwoDimDerivativeResult> toReturn(arr.getWidth(), arr.getHeight());

		//corners
		toReturn(0, 0) = glm::vec4(
			(-3.0f * arr(0, 0) + 4.0f * arr(1, 0) - arr(2, 0)) / (2.0f * deltaX),
			(-3.0f * arr(0, 0) + 4.0f * arr(0, 1) - arr(0, 2)) / (2.0f * deltaY)		
		);
		toReturn(arr.getWidth() - 1, 0) = glm::vec4(
			(-3.0f * arr(toReturn.getWidth() - 1, 0) + 4.0f * arr(toReturn.getWidth() - 2, 0) - arr(toReturn.getWidth() - 3, 0)) / (2.0f * deltaX),
			(-3.0f * arr(toReturn.getWidth() - 1, 0) + 4.0f * arr(toReturn.getWidth() - 1, 1) - arr(toReturn.getWidth() - 1, 2)) / (2.0f * deltaY)
		);
		toReturn(0, arr.getHeight() - 1) = glm::vec4(
			(-3.0f * arr(0, arr.getHeight() - 1) + 4.0f * arr(1, arr.getHeight() - 1) - arr(2, arr.getHeight() - 1)) / (2.0f * deltaX),
			(-3.0f * arr(0, toReturn.getHeight() - 1) + 4.0f * arr(0, toReturn.getHeight() - 2) - arr(0, toReturn.getHeight() - 3)) / (2.0f * deltaY)
		);
		toReturn(arr.getWidth() - 1, arr.getHeight() - 1) = glm::vec4(
			(-3.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 1) + 4.0f * arr(toReturn.getWidth() - 2, toReturn.getHeight() - 1) - arr(toReturn.getWidth() - 3, toReturn.getHeight() - 1)) / (2.0f * deltaX),
			(-3.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 1) + 4.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 2) - arr(toReturn.getWidth() - 1, toReturn.getHeight() - 3)) / (2.0f * deltaY)
		);

		//edges		
		for (int i = 1; i < toReturn.getWidth() - 1; ++i)
		{
			toReturn(i, 0) = glm::vec4(
				(arr(i - 1, 0) - arr(i + 1, 0)) / (2.0f * deltaX),
				(-3.0f * arr(i, 0) + 4.0f * arr(i, 1) - arr(i, 2)) / (2.0f * deltaY)
			);	
			toReturn(i, toReturn.getHeight() - 1) = glm::vec4(
				(arr(i - 1, toReturn.getHeight() - 1) - arr(i + 1, toReturn.getHeight() - 1)) / (2.0f * deltaX),
				(-3.0f * arr(i, toReturn.getHeight() - 1) + 4.0f * arr(i, toReturn.getHeight() - 2) - arr(i, toReturn.getHeight() - 3)) / (2.0f * deltaY)
			);
		}
		for (int i = 1; i < toReturn.getHeight() - 1; ++i)
		{
			toReturn(0, i) = glm::vec4(
				(-3.0f * arr(0, i) + 4.0f * arr(1, i) - arr(2, i)) / (2.0f * deltaX),
				(arr(0, i - 1) - arr(0, i + 1)) / (2.0f * deltaY)
			);
			toReturn(toReturn.getWidth() - 1, i) = glm::vec4(
				(-3.0f * arr(toReturn.getWidth() - 1, i) + 4.0f * arr(toReturn.getWidth() - 2, i) - arr(toReturn.getWidth() - 3, i)) / (2.0f * deltaX),
				(arr(toReturn.getWidth() - 1, i - 1) - arr(toReturn.getWidth() - 1, i + 1)) / (2.0f * deltaY)
			);
		}

		//the rest of it
		for (int i = 1; i < arr.getWidth() - 1; ++i)
		{
			for (int j = 1; j < arr.getHeight() - 1; ++j)
			{
				toReturn(i, j) = glm::vec4(
					(arr(i - 1, j) - arr(i + 1, j)) / (2.0f * deltaX),
					(arr(i, j - 1) - arr(i, j + 1)) / (2.0f * deltaY)
				);
			}
		}

		return toReturn;
	}

	/*
		returns a vec2 where the entry is dF/dx and the second entry is dF/dy
	*/
	Array2D<OneValueTwoDimDerivativeResult> derivative(Array2D<float> arr, float deltaX, float deltaY)
	{
		Array2D<OneValueTwoDimDerivativeResult> toReturn(arr.getWidth(), arr.getHeight());

		//corners
		toReturn(0, 0) = glm::vec2(
			(-3.0f * arr(0, 0) + 4.0f * arr(1, 0) - arr(2, 0)) / (2.0f * deltaX),
			(-3.0f * arr(0, 0) + 4.0f * arr(0, 1) - arr(0, 2)) / (2.0f * deltaY)
		);
		toReturn(arr.getWidth() - 1, 0) = glm::vec2(
			(-3.0f * arr(toReturn.getWidth() - 1, 0) + 4.0f * arr(toReturn.getWidth() - 2, 0) - arr(toReturn.getWidth() - 3, 0)) / (2.0f * deltaX),
			(-3.0f * arr(toReturn.getWidth() - 1, 0) + 4.0f * arr(toReturn.getWidth() - 1, 1) - arr(toReturn.getWidth() - 1, 2)) / (2.0f * deltaY)
		);
		toReturn(0, arr.getHeight() - 1) = glm::vec2(
			(-3.0f * arr(0, arr.getHeight() - 1) + 4.0f * arr(1, arr.getHeight() - 1) - arr(2, arr.getHeight() - 1)) / (2.0f * deltaX),
			(-3.0f * arr(0, toReturn.getHeight() - 1) + 4.0f * arr(0, toReturn.getHeight() - 2) - arr(0, toReturn.getHeight() - 3)) / (2.0f * deltaY)
		);
		toReturn(arr.getWidth() - 1, arr.getHeight() - 1) = glm::vec2(
			(-3.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 1) + 4.0f * arr(toReturn.getWidth() - 2, toReturn.getHeight() - 1) - arr(toReturn.getWidth() - 3, toReturn.getHeight() - 1)) / (2.0f * deltaX),
			(-3.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 1) + 4.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 2) - arr(toReturn.getWidth() - 1, toReturn.getHeight() - 3)) / (2.0f * deltaY)
		);

		//edges		
		for (int i = 1; i < toReturn.getWidth() - 1; ++i)
		{
			toReturn(i, 0) = glm::vec2(
				(arr(i - 1, 0) - arr(i + 1, 0)) / (2.0f * deltaX),
				(-3.0f * arr(i, 0) + 4.0f * arr(i, 1) - arr(i, 2)) / (2.0f * deltaY)
			);
			toReturn(i, toReturn.getHeight() - 1) = glm::vec2(
				(arr(i - 1, toReturn.getHeight() - 1) - arr(i + 1, toReturn.getHeight() - 1)) / (2.0f * deltaX),
				(-3.0f * arr(i, toReturn.getHeight() - 1) + 4.0f * arr(i, toReturn.getHeight() - 2) - arr(i, toReturn.getHeight() - 3)) / (2.0f * deltaY)
			);
		}
		for (int i = 1; i < toReturn.getHeight() - 1; ++i)
		{
			toReturn(0, i) = glm::vec2(
				(-3.0f * arr(0, i) + 4.0f * arr(1, i) - arr(2, i)) / (2.0f * deltaX),
				(arr(0, i - 1) - arr(0, i + 1)) / (2.0f * deltaY)
			);
			toReturn(toReturn.getWidth() - 1, i) = glm::vec2(
				(-3.0f * arr(toReturn.getWidth() - 1, i) + 4.0f * arr(toReturn.getWidth() - 2, i) - arr(toReturn.getWidth() - 3, i)) / (2.0f * deltaX),
				(arr(toReturn.getWidth() - 1, i - 1) - arr(toReturn.getWidth() - 1, i + 1)) / (2.0f * deltaY)
			);
		}

		//the rest of it
		for (int i = 1; i < arr.getWidth() - 1; ++i)
		{
			for (int j = 1; j < arr.getHeight() - 1; ++j)
			{
				toReturn(i, j) = glm::vec2(
					(arr(i - 1, j) - arr(i + 1, j)) / (2.0f * deltaX),
					(arr(i, j - 1) - arr(i, j + 1)) / (2.0f * deltaY)
				);
			}
		}

		return toReturn;
	}

	/*
		returns a vec4 where the first two entries are d2V/dx2 and the second two entries are d2V/dy2
	*/
	Array2D<TwoValueTwoDimDerivativeResult> secondDerivative(Array2D<glm::vec2> arr, float deltaX, float deltaY)
	{
		Array2D<TwoValueTwoDimDerivativeResult> toReturn(arr.getWidth(), arr.getHeight());

		//corners
		toReturn(0, 0) = glm::vec4(
			(13.0f * arr(0, 0) - 13.0f * arr(1, 0) - arr(2, 0) + arr(3, 0)) / (4.0f * deltaX * deltaX),
			(13.0f * arr(0, 0) - 13.0f * arr(0, 1) - arr(0, 2) + arr(0, 3)) / (4.0f * deltaY * deltaY)
		);
		toReturn(arr.getWidth() - 1, 0) = glm::vec4(
			(13.0f * arr(toReturn.getWidth() - 1, 0) - 13.0f * arr(toReturn.getWidth() - 2, 0) - arr(toReturn.getWidth() - 3, 0) + arr(toReturn.getWidth() - 4, 0)) / (4.0f * deltaX * deltaX),
			(13.0f * arr(toReturn.getWidth() - 1, 0) - 13.0f * arr(toReturn.getWidth() - 1, 1) - arr(toReturn.getWidth() - 1, 2) + arr(3, 0)) / (4.0f * deltaY * deltaY)
		);
		toReturn(0, arr.getHeight() - 1) = glm::vec4(
			(13.0f * arr(0, toReturn.getHeight() - 1) - 13.0f * arr(1, toReturn.getHeight() - 1) - arr(2, toReturn.getHeight() - 1) + arr(3, toReturn.getHeight() - 1)) / (4.0f * deltaX * deltaX),
			(13.0f * arr(0, toReturn.getHeight() - 1) - 13.0f * arr(0, toReturn.getHeight() - 2) - arr(0, toReturn.getHeight() - 3) + arr(0, toReturn.getHeight() - 4)) / (4.0f * deltaY * deltaY)
		);
		toReturn(arr.getWidth() - 1, arr.getHeight() - 1) = glm::vec4(
			(13.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 1) - 13.0f *  arr(toReturn.getWidth() - 2, toReturn.getHeight() - 1) - arr(toReturn.getWidth() - 3, toReturn.getHeight() - 1) + arr(toReturn.getWidth() - 4, toReturn.getHeight() - 1)) / (4.0f * deltaX * deltaX),
			(13.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 1) - 13.0f *  arr(toReturn.getWidth() - 1, toReturn.getHeight() - 2) - arr(toReturn.getWidth() - 1, toReturn.getHeight() - 3) + arr(toReturn.getWidth() - 1, toReturn.getHeight() - 4)) / (4.0f * deltaY * deltaY)
		);

		//edges		
		for (int i = 1; i < toReturn.getWidth() - 1; ++i)
		{
			toReturn(i, 0) = glm::vec4(
				(arr(i - 1, 0) - 2.0f * arr(i, 0) + arr(i + 1, 0)) / (deltaX * deltaX),
				(13.0f * arr(i, 0) - 13.0f * arr(i, 1) - arr(i, 2) + arr(i, 3)) / (4.0f * deltaY * deltaY)
			);
			toReturn(i, toReturn.getHeight() - 1) = glm::vec4(
				(arr(i - 1, toReturn.getHeight() - 1) - 2.0f * arr(i, toReturn.getHeight() - 1) + arr(i + 1, toReturn.getHeight() - 1)) / (deltaX * deltaX),
				(13.0f * arr(i, toReturn.getHeight() - 1) - 13.0f * arr(i, toReturn.getHeight() - 2) - arr(i, toReturn.getHeight() - 3) + arr(i, toReturn.getHeight() - 4)) / (4.0f * deltaY * deltaY)
			);
		}
		for (int i = 1; i < toReturn.getHeight() - 1; ++i)
		{
			toReturn(0, i) = glm::vec4(
				(13.0f * arr(0, i) - 13.0f * arr(1, i) - arr(2, i) + arr(3, i)) / (4.0f * deltaX * deltaX),
				(arr(0, i - 1) - 2.0f * arr(0, i) + arr(0, i + 1)) / (deltaY * deltaY)
			);
			toReturn(toReturn.getWidth() - 1, i) = glm::vec4(
				(13.0f * arr(toReturn.getWidth() - 1, i) - 13.0f * arr(toReturn.getWidth() - 2, i) - arr(toReturn.getWidth() - 3, i) + arr(toReturn.getWidth() - 4, i)) / (4.0f * deltaX * deltaX),
				(arr(toReturn.getWidth() - 1, i - 1) - 2.0f * arr(toReturn.getWidth() - 1, i) + arr(toReturn.getWidth() - 1, i + 1)) / (deltaY * deltaY)
			);
		}

		//the rest of it
		for (int i = 1; i < arr.getWidth() - 1; ++i)
		{
			for (int j = 1; j < arr.getHeight() - 1; ++j)
			{
				toReturn(i, j) = glm::vec4(
					(arr(i - 1, j) - 2.0f * arr(i, j) + arr(i + 1, j)) / (deltaX * deltaX),
					(arr(i, j - 1) - 2.0f * arr(i, j) + arr(i, j + 1)) / (deltaY * deltaY)
				);
			}
		}
		return toReturn;
	}

	/*
		returns a vec4 where the first entry is d2F/dx2 and the second entry is d2F/dy2
	*/
	Array2D<OneValueTwoDimDerivativeResult> secondDerivative(Array2D<float> arr, float deltaX, float deltaY)
	{
		Array2D<OneValueTwoDimDerivativeResult> toReturn(arr.getWidth(), arr.getHeight());

		//corners
		toReturn(0, 0) = glm::vec2(
			(13.0f * arr(0, 0) - 13.0f * arr(1, 0) - arr(2, 0) + arr(3, 0)) / (4.0f * deltaX * deltaX),
			(13.0f * arr(0, 0) - 13.0f * arr(0, 1) - arr(0, 2) + arr(0, 3)) / (4.0f * deltaY * deltaY)
		);
		toReturn(arr.getWidth() - 1, 0) = glm::vec2(
			(13.0f * arr(toReturn.getWidth() - 1, 0) - 13.0f * arr(toReturn.getWidth() - 2, 0) - arr(toReturn.getWidth() - 3, 0) + arr(toReturn.getWidth() - 4, 0)) / (4.0f * deltaX * deltaX),
			(13.0f * arr(toReturn.getWidth() - 1, 0) - 13.0f * arr(toReturn.getWidth() - 1, 1) - arr(toReturn.getWidth() - 1, 2) + arr(3, 0)) / (4.0f * deltaY * deltaY)
		);
		toReturn(0, arr.getHeight() - 1) = glm::vec2(
			(13.0f * arr(0, toReturn.getHeight() - 1) - 13.0f * arr(1, toReturn.getHeight() - 1) - arr(2, toReturn.getHeight() - 1) + arr(3, toReturn.getHeight() - 1)) / (4.0f * deltaX * deltaX),
			(13.0f * arr(0, toReturn.getHeight() - 1) - 13.0f * arr(0, toReturn.getHeight() - 2) - arr(0, toReturn.getHeight() - 3) + arr(0, toReturn.getHeight() - 4)) / (4.0f * deltaY * deltaY)
		);
		toReturn(arr.getWidth() - 1, arr.getHeight() - 1) = glm::vec2(
			(13.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 1) - 13.0f * arr(toReturn.getWidth() - 2, toReturn.getHeight() - 1) - arr(toReturn.getWidth() - 3, toReturn.getHeight() - 1) + arr(toReturn.getWidth() - 4, toReturn.getHeight() - 1)) / (4.0f * deltaX * deltaX),
			(13.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 1) - 13.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 2) - arr(toReturn.getWidth() - 1, toReturn.getHeight() - 3) + arr(toReturn.getWidth() - 1, toReturn.getHeight() - 4)) / (4.0f * deltaY * deltaY)
		);

		//edges		
		for (int i = 1; i < toReturn.getWidth() - 1; ++i)
		{
			toReturn(i, 0) = glm::vec2(
				(arr(i - 1, 0) - 2.0f * arr(i, 0) + arr(i + 1, 0)) / (deltaX * deltaX),
				(13.0f * arr(i, 0) - 13.0f * arr(i, 1) - arr(i, 2) + arr(i, 3)) / (4.0f * deltaY * deltaY)
			);
			toReturn(i, toReturn.getHeight() - 1) = glm::vec2(
				(arr(i - 1, toReturn.getHeight() - 1) - 2.0f * arr(i, toReturn.getHeight() - 1) + arr(i + 1, toReturn.getHeight() - 1)) / (deltaX * deltaX),
				(13.0f * arr(i, toReturn.getHeight() - 1) - 13.0f * arr(i, toReturn.getHeight() - 2) - arr(i, toReturn.getHeight() - 3) + arr(i, toReturn.getHeight() - 4)) / (4.0f * deltaY * deltaY)
			);
		}
		for (int i = 1; i < toReturn.getHeight() - 1; ++i)
		{
			toReturn(0, i) = glm::vec2(
				(13.0f * arr(0, i) - 13.0f * arr(1, i) - arr(2, i) + arr(3, i)) / (4.0f * deltaX * deltaX),
				(arr(0, i - 1) - 2.0f * arr(0, i) + arr(0, i + 1)) / (deltaY * deltaY)
			);
			toReturn(toReturn.getWidth() - 1, i) = glm::vec2(
				(13.0f * arr(toReturn.getWidth() - 1, i) - 13.0f * arr(toReturn.getWidth() - 2, i) - arr(toReturn.getWidth() - 3, i) + arr(toReturn.getWidth() - 4, i)) / (4.0f * deltaX * deltaX),
				(arr(toReturn.getWidth() - 1, i - 1) - 2.0f * arr(toReturn.getWidth() - 1, i) + arr(toReturn.getWidth() - 1, i + 1)) / (deltaY * deltaY)
			);
		}

		//the rest of it
		for (int i = 1; i < arr.getWidth() - 1; ++i)
		{
			for (int j = 1; j < arr.getHeight() - 1; ++j)
			{
				toReturn(i, j) = glm::vec2(
					(arr(i - 1, j) - 2.0f * arr(i, j) + arr(i + 1, j)) / (deltaX * deltaX),
					(arr(i, j - 1) - 2.0f * arr(i, j) + arr(i, j + 1)) / (deltaY * deltaY)
				);
			}
		}
		return toReturn;
	}

	/*
		returns a vec4 where the first two entries are dV/dxdy and the second two entries are dV/dydx
	*/	
	Array2D<TwoValueTwoDimMixedDerivativeResult> mixedDerivative(Array2D<glm::vec2> arr, float deltaX, float deltaY)
	{
		Array2D<TwoValueTwoDimMixedDerivativeResult> toReturn(arr.getWidth(), arr.getHeight());

		//corners
		toReturn(0, 0) = glm::vec4(
			(9.0f * arr(0, 0) - 12.0f * arr(1, 0) + 3.0f * arr(2, 0) - 12.0f * arr(0, 1) + 16.0f * arr(1, 1) - 4.0f * arr(2, 1) + 3.0f * arr(0, 2) - 4.0f * arr(1, 2) + arr(2, 2)) / (4.0f * deltaX * deltaY),
			(9.0f * arr(0, 0) - 12.0f * arr(0, 1) + 3.0f * arr(0, 2) - 12.0f * arr(1, 0) + 16.0f * arr(1, 1) - 4.0f * arr(1, 2) + 3.0f * arr(2, 0) - 4.0f * arr(2, 1) + arr(2, 2)) / (4.0f * deltaX * deltaY)
		);
		toReturn(arr.getWidth() - 1, 0) = glm::vec4(
			(9.0f * arr(toReturn.getWidth() - 1, 0) - 12.0f * arr(toReturn.getWidth() - 2, 0) + 3.0f * arr(toReturn.getWidth() - 3, 0) - 12.0f * arr(toReturn.getWidth() - 1, 1) + 16.0f * arr(toReturn.getWidth() - 2, 1) - 4.0f * arr(toReturn.getWidth() - 3, 1) + 3.0f * arr(toReturn.getWidth() - 1, 2) - 4.0f * arr(toReturn.getWidth() - 2, 2) + arr(toReturn.getWidth() - 3, 2)) / (4.0f * deltaX * deltaY),
			(9.0f * arr(toReturn.getWidth() - 1, 0) - 12.0f * arr(toReturn.getWidth() - 1, 1) + 3.0f * arr(toReturn.getWidth() - 1, 2) - 12.0f * arr(toReturn.getWidth() - 2, 0) + 16.0f * arr(toReturn.getWidth() - 2, 1) - 4.0f * arr(toReturn.getWidth() - 2, 2) + 3.0f * arr(toReturn.getWidth() - 3, 0) - 4.0f * arr(toReturn.getWidth() - 3, 1) + arr(toReturn.getWidth() - 3, 2)) / (4.0f * deltaX * deltaY)
		);
		toReturn(0, arr.getHeight() - 1) = glm::vec4(
			(9.0f * arr(0, toReturn.getHeight() - 1) - 12.0f * arr(1, toReturn.getHeight() - 1) + 3.0f * arr(2, toReturn.getHeight() - 1) - 12.0f * arr(0, toReturn.getHeight() - 2) + 16.0f * arr(1, toReturn.getHeight() - 2) - 4.0f * arr(2, toReturn.getHeight() - 2) + 3.0f * arr(0, toReturn.getHeight() - 3) - 4.0f * arr(1, toReturn.getHeight() - 3) + arr(2, toReturn.getHeight() - 3)) / (4.0f * deltaX * deltaY),
			(9.0f * arr(0, toReturn.getHeight() - 1) - 12.0f * arr(0, toReturn.getHeight() - 2) + 3.0f * arr(0, toReturn.getHeight() - 3) - 12.0f * arr(1, toReturn.getHeight() - 1) + 16.0f * arr(1, toReturn.getHeight() - 2) - 4.0f * arr(1, toReturn.getHeight() - 3) + 3.0f * arr(2, toReturn.getHeight() - 1) - 4.0f * arr(2, toReturn.getHeight() - 2) + arr(2, toReturn.getHeight() - 3)) / (4.0f * deltaX * deltaY)
		);
		toReturn(arr.getWidth() - 1, arr.getHeight() - 1) = glm::vec4(
			(9.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 1) - 12.0f * arr(toReturn.getWidth() - 2, toReturn.getHeight() - 1) + 3.0f * arr(toReturn.getWidth() - 3, toReturn.getHeight() - 1) - 12.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 2) + 16.0f * arr(toReturn.getWidth() - 2, toReturn.getHeight() - 2) - 4.0f * arr(toReturn.getWidth() - 3, toReturn.getHeight() - 2) + 3.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 3) - 4.0f * arr(toReturn.getWidth() - 2, toReturn.getHeight() - 3) + arr(toReturn.getWidth() - 3, toReturn.getHeight() - 3)) / (4.0f * deltaX * deltaY),
			(9.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 1) - 12.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 2) + 3.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 3) - 12.0f * arr(toReturn.getWidth() - 2, toReturn.getHeight() - 1) + 16.0f * arr(toReturn.getWidth() - 2, toReturn.getHeight() - 2) - 4.0f * arr(toReturn.getWidth() - 2, toReturn.getHeight() - 3) + 3.0f * arr(toReturn.getWidth() - 3, toReturn.getHeight() - 1) - 4.0f * arr(toReturn.getWidth() - 3, toReturn.getHeight() - 2) + arr(toReturn.getWidth() - 3, toReturn.getHeight() - 3)) / (4.0f * deltaX * deltaY)
		);

		//edges		
		for (int i = 1; i < toReturn.getWidth() - 1; ++i)
		{
			toReturn(i, 0) = glm::vec4(
				(-3.0f * arr(i - 1, 0) + 3.0f * arr(i + 1, 0) + 4.0f * arr(i - 1, 1) - 4.0f * arr(i + 1, 1) - arr(i - 1, 2) + arr(i + 1, 2)) / (4.0f * deltaX * deltaY),
				(-3.0f * arr(i - 1, 0) + 4.0f * arr(i - 1, 1) - arr(i - 1, 2) + 3.0f * arr(i + 1, 0) - 4.0f * arr(i + 1, 1) + arr(i + 1, 2)) / (4.0f * deltaX * deltaY)
			);
			toReturn(i, toReturn.getHeight() - 1) = glm::vec4(
				(-3.0f * arr(i - 1, toReturn.getHeight() - 1) + 3.0f * arr(i + 1, toReturn.getHeight() - 1) + 4.0f * arr(i - 1, toReturn.getHeight() - 2) - 4.0f * arr(i + 1, toReturn.getHeight() - 2) - arr(i - 1, toReturn.getHeight() - 3) + arr(i + 1, toReturn.getHeight() - 3)) / (4.0f * deltaX * deltaY),
				(-3.0f * arr(i - 1, toReturn.getHeight() - 1) + 4.0f * arr(i - 1, toReturn.getHeight() - 2) - arr(i - 1, toReturn.getHeight() - 3) + 3.0f * arr(i + 1, toReturn.getHeight() - 1) - 4.0f * arr(i + 1, toReturn.getHeight() - 2) + arr(i + 1, toReturn.getHeight() - 3)) / (4.0f * deltaX * deltaY)
			);
		}
		for (int i = 1; i < toReturn.getHeight() - 1; ++i)
		{
			toReturn(0, i) = glm::vec4(
				(-3.0f * arr(0, i - 1) + 4.0f * arr(1, i - 1) - arr(2, i - 1) + 3.0f * arr(0, i + 1) - 4.0f * arr(1, i + 1) + arr(2, i + 1)) / (4.0f * deltaX * deltaY),
				(-3.0f * arr(0, i - 1) + 3.0f * arr(0, i + 1) + 4.0f * arr(1, i - 1) - 4.0f * arr(1, i - 1) - arr(2, i + 1) + arr(2, i + 1)) / (4.0f * deltaX * deltaY)
			);
			toReturn(toReturn.getWidth() - 1, i) = glm::vec4(
				(-3.0f * arr(toReturn.getWidth() - 1, i - 1) + 4.0f * arr(toReturn.getWidth() - 2, i - 1) - arr(toReturn.getWidth() - 3, i - 1) + 3.0f * arr(toReturn.getWidth() - 1, i + 1) - 4.0f * arr(toReturn.getWidth() - 2, i + 1) + arr(toReturn.getWidth() - 3, i + 1)) / (4.0f * deltaX * deltaY),
				(-3.0f * arr(toReturn.getWidth() - 1, i - 1) + 3.0f * arr(toReturn.getWidth() - 1, i + 1) + 4.0f * arr(toReturn.getWidth() - 2, i - 1) - 4.0f * arr(toReturn.getWidth() - 2, i - 1) - arr(toReturn.getWidth() - 3, i + 1) + arr(toReturn.getWidth() - 3, i + 1)) / (4.0f * deltaX * deltaY)
			);
		}

		//the rest of it
		for (int i = 1; i < arr.getWidth() - 1; ++i)
		{
			for (int j = 1; j < arr.getHeight() - 1; ++j)
			{
				//please double check when sober.  the symmetry is suspicious
				toReturn(i, j) = glm::vec4(
					(arr(i - 1, j - 1) - arr(i + 1, j - 1) + arr(i - 1, j + 1) - arr(i + 1, j + 1)) / (4.0f * deltaX * deltaY),
					(arr(i - 1, j - 1) - arr(i - 1, j + 1) + arr(i + 1, j - 1) - arr(i + 1, j + 1)) / (4.0f * deltaX * deltaY)
				);
			}
		}

		return toReturn;
	}
	/*
		returns a vec2 where the first entry is dF/dxdy and the second entry is dF/dydx
	*/
	Array2D<OneValueTwoDimMixedDerivativeResult> mixedDerivative(Array2D<float> arr, float deltaX, float deltaY)
	{
		Array2D<OneValueTwoDimMixedDerivativeResult> toReturn(arr.getWidth(), arr.getHeight());

		//corners
		toReturn(0, 0) = glm::vec2(
			(9.0f * arr(0, 0) - 12.0f * arr(1, 0) + 3.0f * arr(2, 0) - 12.0f * arr(0, 1) + 16.0f * arr(1, 1) - 4.0f * arr(2, 1) + 3.0f * arr(0, 2) - 4.0f * arr(1, 2) + arr(2, 2)) / (4.0f * deltaX * deltaY),
			(9.0f * arr(0, 0) - 12.0f * arr(0, 1) + 3.0f * arr(0, 2) - 12.0f * arr(1, 0) + 16.0f * arr(1, 1) - 4.0f * arr(1, 2) + 3.0f * arr(2, 0) - 4.0f * arr(2, 1) + arr(2, 2)) / (4.0f * deltaX * deltaY)
		);
		toReturn(arr.getWidth() - 1, 0) = glm::vec2(
			(9.0f * arr(toReturn.getWidth() - 1, 0) - 12.0f * arr(toReturn.getWidth() - 2, 0) + 3.0f * arr(toReturn.getWidth() - 3, 0) - 12.0f * arr(toReturn.getWidth() - 1, 1) + 16.0f * arr(toReturn.getWidth() - 2, 1) - 4.0f * arr(toReturn.getWidth() - 3, 1) + 3.0f * arr(toReturn.getWidth() - 1, 2) - 4.0f * arr(toReturn.getWidth() - 2, 2) + arr(toReturn.getWidth() - 3, 2)) / (4.0f * deltaX * deltaY),
			(9.0f * arr(toReturn.getWidth() - 1, 0) - 12.0f * arr(toReturn.getWidth() - 1, 1) + 3.0f * arr(toReturn.getWidth() - 1, 2) - 12.0f * arr(toReturn.getWidth() - 2, 0) + 16.0f * arr(toReturn.getWidth() - 2, 1) - 4.0f * arr(toReturn.getWidth() - 2, 2) + 3.0f * arr(toReturn.getWidth() - 3, 0) - 4.0f * arr(toReturn.getWidth() - 3, 1) + arr(toReturn.getWidth() - 3, 2)) / (4.0f * deltaX * deltaY)
		);
		toReturn(0, arr.getHeight() - 1) = glm::vec2(
			(9.0f * arr(0, toReturn.getHeight() - 1) - 12.0f * arr(1, toReturn.getHeight() - 1) + 3.0f * arr(2, toReturn.getHeight() - 1) - 12.0f * arr(0, toReturn.getHeight() - 2) + 16.0f * arr(1, toReturn.getHeight() - 2) - 4.0f * arr(2, toReturn.getHeight() - 2) + 3.0f * arr(0, toReturn.getHeight() - 3) - 4.0f * arr(1, toReturn.getHeight() - 3) + arr(2, toReturn.getHeight() - 3)) / (4.0f * deltaX * deltaY),
			(9.0f * arr(0, toReturn.getHeight() - 1) - 12.0f * arr(0, toReturn.getHeight() - 2) + 3.0f * arr(0, toReturn.getHeight() - 3) - 12.0f * arr(1, toReturn.getHeight() - 1) + 16.0f * arr(1, toReturn.getHeight() - 2) - 4.0f * arr(1, toReturn.getHeight() - 3) + 3.0f * arr(2, toReturn.getHeight() - 1) - 4.0f * arr(2, toReturn.getHeight() - 2) + arr(2, toReturn.getHeight() - 3)) / (4.0f * deltaX * deltaY)
		);
		toReturn(arr.getWidth() - 1, arr.getHeight() - 1) = glm::vec2(
			(9.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 1) - 12.0f * arr(toReturn.getWidth() - 2, toReturn.getHeight() - 1) + 3.0f * arr(toReturn.getWidth() - 3, toReturn.getHeight() - 1) - 12.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 2) + 16.0f * arr(toReturn.getWidth() - 2, toReturn.getHeight() - 2) - 4.0f * arr(toReturn.getWidth() - 3, toReturn.getHeight() - 2) + 3.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 3) - 4.0f * arr(toReturn.getWidth() - 2, toReturn.getHeight() - 3) + arr(toReturn.getWidth() - 3, toReturn.getHeight() - 3)) / (4.0f * deltaX * deltaY),
			(9.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 1) - 12.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 2) + 3.0f * arr(toReturn.getWidth() - 1, toReturn.getHeight() - 3) - 12.0f * arr(toReturn.getWidth() - 2, toReturn.getHeight() - 1) + 16.0f * arr(toReturn.getWidth() - 2, toReturn.getHeight() - 2) - 4.0f * arr(toReturn.getWidth() - 2, toReturn.getHeight() - 3) + 3.0f * arr(toReturn.getWidth() - 3, toReturn.getHeight() - 1) - 4.0f * arr(toReturn.getWidth() - 3, toReturn.getHeight() - 2) + arr(toReturn.getWidth() - 3, toReturn.getHeight() - 3)) / (4.0f * deltaX * deltaY)
		);

		//edges		
		for (int i = 1; i < toReturn.getWidth() - 1; ++i)
		{
			toReturn(i, 0) = glm::vec2(
				(-3.0f * arr(i - 1, 0) + 3.0f * arr(i + 1, 0) + 4.0f * arr(i - 1, 1) - 4.0f * arr(i + 1, 1) - arr(i - 1, 2) + arr(i + 1, 2)) / (4.0f * deltaX * deltaY),
				(-3.0f * arr(i - 1, 0) + 4.0f * arr(i - 1, 1) - arr(i - 1, 2) + 3.0f * arr(i + 1, 0) - 4.0f * arr(i + 1, 1) + arr(i + 1, 2)) / (4.0f * deltaX * deltaY)
			);
			toReturn(i, toReturn.getHeight() - 1) = glm::vec2(
				(-3.0f * arr(i - 1, toReturn.getHeight() - 1) + 3.0f * arr(i + 1, toReturn.getHeight() - 1) + 4.0f * arr(i - 1, toReturn.getHeight() - 2) - 4.0f * arr(i + 1, toReturn.getHeight() - 2) - arr(i - 1, toReturn.getHeight() - 3) + arr(i + 1, toReturn.getHeight() - 3)) / (4.0f * deltaX * deltaY),
				(-3.0f * arr(i - 1, toReturn.getHeight() - 1) + 4.0f * arr(i - 1, toReturn.getHeight() - 2) - arr(i - 1, toReturn.getHeight() - 3) + 3.0f * arr(i + 1, toReturn.getHeight() - 1) - 4.0f * arr(i + 1, toReturn.getHeight() - 2) + arr(i + 1, toReturn.getHeight() - 3)) / (4.0f * deltaX * deltaY)
			);
		}
		for (int i = 1; i < toReturn.getHeight() - 1; ++i)
		{
			toReturn(0, i) = glm::vec2(
				(-3.0f * arr(0, i - 1) + 4.0f * arr(1, i - 1) - arr(2, i - 1) + 3.0f * arr(0, i + 1) - 4.0f * arr(1, i + 1) + arr(2, i + 1)) / (4.0f * deltaX * deltaY),
				(-3.0f * arr(0, i - 1) + 3.0f * arr(0, i + 1) + 4.0f * arr(1, i - 1) - 4.0f * arr(1, i - 1) - arr(2, i + 1) + arr(2, i + 1)) / (4.0f * deltaX * deltaY)
			);
			toReturn(toReturn.getWidth() - 1, i) = glm::vec2(
				(-3.0f * arr(toReturn.getWidth() - 1, i - 1) + 4.0f * arr(toReturn.getWidth() - 2, i - 1) - arr(toReturn.getWidth() - 3, i - 1) + 3.0f * arr(toReturn.getWidth() - 1, i + 1) - 4.0f * arr(toReturn.getWidth() - 2, i + 1) + arr(toReturn.getWidth() - 3, i + 1)) / (4.0f * deltaX * deltaY),
				(-3.0f * arr(toReturn.getWidth() - 1, i - 1) + 3.0f * arr(toReturn.getWidth() - 1, i + 1) + 4.0f * arr(toReturn.getWidth() - 2, i - 1) - 4.0f * arr(toReturn.getWidth() - 2, i - 1) - arr(toReturn.getWidth() - 3, i + 1) + arr(toReturn.getWidth() - 3, i + 1)) / (4.0f * deltaX * deltaY)
			);
		}

		//the rest of it
		for (int i = 1; i < arr.getWidth() - 1; ++i)
		{
			for (int j = 1; j < arr.getHeight() - 1; ++j)
			{
				//please double check when sober.  the symmetry is suspicious
				toReturn(i, j) = glm::vec2(
					(arr(i - 1, j - 1) - arr(i + 1, j - 1) + arr(i - 1, j + 1) - arr(i + 1, j + 1)) / (4.0f * deltaX * deltaY),
					(arr(i - 1, j - 1) - arr(i - 1, j + 1) + arr(i + 1, j - 1) - arr(i + 1, j + 1)) / (4.0f * deltaX * deltaY)
				);
			}
		}

		return toReturn;
	}
}
