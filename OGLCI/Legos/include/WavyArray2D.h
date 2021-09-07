#pragma once
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include "LoggingUtils.h"
#include "Array2D.h"

//Row major

template <class T>
class WavyArray2D
{
private:
	T** m_cols;
	unsigned int m_width;
	unsigned int* m_heights;

public:

	WavyArray2D() :m_cols(nullptr), m_width(0), m_heights(nullptr) {};
	WavyArray2D(unsigned int width);
	WavyArray2D(std::vector<unsigned int> heights);
	WavyArray2D(const WavyArray2D<T>& rhs);
	~WavyArray2D();

	WavyArray2D<T> operator=(const WavyArray2D<T>& rhs);
	bool operator==(const WavyArray2D<T>& rhs) const;
	T& operator()(unsigned int x, unsigned int y);

	void setColumnSize(unsigned int c, unsigned int s);
	void setColumn(unsigned int c, std::vector<T> data);
	void setColumn(unsigned int c, Array2D<T> data);
	void setWidth(unsigned int w);
	void setAll(T value);

	Array2D<T> getColumn(unsigned int c);
	inline unsigned int getWidth() const { return m_width; };
	inline unsigned int getHeight(unsigned int c) const 
	{ 
		if (!m_heights || c >= m_width)
		{
			return 0;
		}
		else
		{
			return m_heights[c];
		}
	};
};

template<class T>
inline WavyArray2D<T>::WavyArray2D(unsigned int width) : m_width(width)
{
	m_cols = new T*[width];
}

template<class T>
inline WavyArray2D<T>::WavyArray2D(std::vector<unsigned int> heights)
{
	m_width = heights.size();
	m_heights = new unsigned int[m_width];
	m_cols = new T * [m_width];
	for (unsigned int i = 0; i < heights.size(); ++i)
	{
		m_heights[i] = heights[i];
		m_cols[i] = new T[m_heights[i]];
	}
}

template<class T>
inline WavyArray2D<T>::WavyArray2D(const WavyArray2D<T>& rhs)
{
	m_width = rhs.m_width;
	m_heights = new unsigned int[m_width];
	m_cols = new T * [m_width];
	for (unsigned int i = 0; i < m_width; ++i)
	{
		m_heights[i] = rhs.m_heights[i];
		m_cols[i] = new T[m_heights[i]];
		memcpy(m_cols[i], rhs.m_cols[i], m_heights[i] * sizeof(T));
	}	
}

template<class T>
inline WavyArray2D<T>::~WavyArray2D()
{
	if (m_cols)
	{
		for (unsigned int i = 0; i < m_width; ++i)
		{
			if (m_cols[i])
			{
				delete[] m_cols[i];
			}
		}
		delete[] m_cols;
	}

	if (m_heights)
	{
		delete[] m_heights;
	}
}

template<class T>
inline WavyArray2D<T> WavyArray2D<T>::operator=(const WavyArray2D<T>& rhs)
{
	
	if (m_heights)
	{
		if (m_cols)
		{
			for (unsigned int i = 0; i < m_width; ++i)
			{
				if (m_cols[i])
				{
					delete[] m_cols[i];
				}
			}
			delete[] m_cols;
		}
		delete[] m_heights;
	}
	m_width = rhs.m_width;
	m_heights = new unsigned int[m_width];
	m_cols = new T * [m_width];
	for (unsigned int i = 0; i < m_width; ++i)
	{
		m_heights[i] = rhs.m_heights[i];
		m_cols[i] = new T[m_heights[i]];
		memcpy(m_cols[i], rhs.m_cols[i], m_heights[i] * sizeof(T));
	}
}

template<class T>
inline bool WavyArray2D<T>::operator==(const WavyArray2D<T>& rhs) const
{
	if (m_width != rhs.m_width)
	{
		return false;
	}

	//kinda undefined.  i just chose to make them unequal to simplify the null checking in the next step
	if (m_heights == nullptr || rhs.m_heights != nullptr || m_cols == nullptr || rhs.m_cols == nullptr)
	{
		return false;
	}

	for (unsigned int i = 0; i < m_width; ++i)
	{
		if (m_heights[i] != rhs.m_heights[i])
		{
			return false;
		}

		for (unsigned int j = 0; j < m_heights[i]; ++j)
		{
			if (m_cols[i][j] != rhs.m_cols[i][j])
			{
				return false;
			}
		}
	}
	return true;
}

template<class T>
inline T& WavyArray2D<T>::operator()(unsigned int x, unsigned int y)
{
	return m_cols[x][y];
}

template<class T>
inline void WavyArray2D<T>::setColumnSize(unsigned int c, unsigned int s)
{
	if (c >= m_width)
	{
		throw std::out_of_range("column does not exist");
	}

	if (!m_heights)
	{
		m_heights = new unsigned int [m_width];
		memset(m_heights, 0, m_width * sizeof(unsigned int));
	}
	m_heights[c] = s;

	if (!m_cols)
	{
		m_cols = new T * [m_width];
		for (unsigned int i = 0; i < m_width; ++i)
		{
			m_cols[i] = nullptr;
		}
	}

	if (m_cols[c])
	{
		delete[] m_cols[c];
	}

	m_cols[c] = new T[s];
}

template<class T>
inline void WavyArray2D<T>::setColumn(unsigned int c, std::vector<T> data)
{
	if (c >= m_width)
	{
		throw std::out_of_range("column does not exist");
	}

	if (!m_heights)
	{
		m_heights = new unsigned int[m_width];
		memset(m_heights, 0, m_width);
	}
	m_heights[c] = data.size();

	if (!m_cols)
	{
		m_cols = new T * [m_width];
		for (unsigned int i = 0; i < m_width; ++i)
		{
			m_cols[i] = nullptr;
		}
	}

	if (m_cols[c])
	{
		delete[] m_cols[c];
	}

	m_cols[c] = new T[data.size()];

	memcpy(m_cols[c], data.data(), data.size() * sizeof(T));
}

template<class T>
inline void WavyArray2D<T>::setColumn(unsigned int c, Array2D<T> data)
{
	if (c >= m_width)
	{
		throw std::out_of_range("column does not exist");
	}

	if (data.getWidth() != 1)
	{
		throw std::invalid_argument("input data needs to be a column vector");
	}

	if (!m_heights)
	{
		m_heights = new unsigned int[m_width];
		memset(m_heights, 0, m_width);
	}
	m_heights[c] = data.getHeight();

	if (!m_cols)
	{
		m_cols = new T * [m_width];
		for (unsigned int i = 0; i < m_width; ++i)
		{
			m_cols[i] = nullptr;
		}
	}

	if (m_cols[c])
	{
		delete[] m_cols[c];
	}

	m_cols[c] = new T[data.getHeight()];

	memcpy(m_cols[c], data.getData(), data.getHeight() * sizeof(T));
}

template<class T>
inline void WavyArray2D<T>::setWidth(unsigned int w)
{
	if (m_width == 0)
	{
		if (m_heights)
		{
			delete[] m_heights;
		}
		if (m_cols)
		{
			delete[] m_cols;
		}
		m_width = w;		
		m_heights = new unsigned int[m_width];
		memset(m_heights, 0, m_width);
	}
	else if (m_width > w)
	{
		unsigned int* temp = new unsigned int[w];
		memcpy(m_heights, temp, w);
		delete[] m_heights;
		m_heights = temp;

		T** temp2 = new T * [w];
		memcpy(temp2, m_cols, w * sizeof(T*));
		for (unsigned int i = 0; i < w; ++i)
		{
			memcpy(temp2, m_cols[i], m_heights[i] * sizeof(T));
			delete[] m_cols[i];
		}
		delete[] m_cols;

		m_cols = temp2;
		m_width = w;
	}
	else if (m_width < w)
	{
		unsigned int* temp = new unsigned int[w];
		memcpy(m_heights, temp, m_width);
		memset(m_heights + m_width, 0, w - m_width);
		delete[] m_heights;
		m_heights = temp;

		T** temp2 = new T * [w];
		memcpy(temp2, m_cols, m_width * sizeof(T*));
		for (unsigned int i = 0; i < m_width; ++i)
		{
			memcpy(temp2, m_cols[i], m_heights[i] * sizeof(T));
			delete[] m_cols[i];
		}
		delete[] m_cols;

		m_cols = temp2;
		m_width = w;
	}
	return;
}

template<class T>
void WavyArray2D<T>::setAll(T value)
{
	for (unsigned int i = 0; i < m_width; ++i)
	{
		for (unsigned int j = 0; j < m_heights[i]; ++j)
		{
			m_cols[i][j] = value;
		}		
	}
}

template<class T>
inline Array2D<T> WavyArray2D<T>::getColumn(unsigned int c)
{
	if (c >= m_width)
	{
		throw std::out_of_range("column does not exist");
	}

	Array2D<T> toReturn;
	if (m_heights[c] > 0)
	{
		toReturn.setData(1, m_heights[c], m_cols[c]);
	}	 
	
	return toReturn;
}
