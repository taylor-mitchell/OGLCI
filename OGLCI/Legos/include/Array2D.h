#pragma once
#include <vector>


//Row major

template <class T>
class Array2D
{
private:
	std::vector<std::vector<T>> m_data;
	unsigned int m_width;
	unsigned int m_height;

public:
	Array2D() :m_data(), m_width(0), m_height(0) {};
	Array2D(unsigned int m_width, unsigned int m_height);
	Array2D(const std::vector<std::vector<T>> data);

	Array2D<T> operator=(const Array2D<T>& rhs);
	Array2D<T> operator=(const std::vector<std::vector<T>>& data);
	bool operator==(const Array2D<T>& rhs) const;

	bool set(unsigned int x, unsigned int y, const T& value);
	void setAndResize(unsigned int x, unsigned int y, const T& value);
	void setAll(const T& value);
	void swapRows(unsigned int r1, unsigned int r2);
	void swapColumns(unsigned int c1, unsigned int c2);
	void setSize(unsigned int x, unsigned int y);
	void transpose();

	T& operator()(unsigned int x, unsigned int y);

	inline unsigned int getWidth() const { return m_width; };
	inline unsigned int getHeight() const { return m_height; };

};

template<class T>
Array2D<T>::Array2D(unsigned int width, unsigned int height)
	:m_data(),
	m_width(width),
	m_height(height)
{
	m_data.resize(height);
	for (int i = 0; i < height; ++i)
	{
		m_data[i].resize(width);
	}
}

template<class T>
inline Array2D<T>::Array2D(const std::vector<std::vector<T>> data)
{
	m_height = data.size();
	if (m_height > 0)
	{
		m_width = data[0].size();
	}
	m_data = data;
}

template<class T>
inline Array2D<T> Array2D<T>::operator=(const Array2D<T>& rhs)
{
	m_height = rhs.m_height;
	m_width = rhs.m_width;
	m_data = rhs.m_data;
	return *this;
}

template<class T>
inline Array2D<T> Array2D<T>::operator=(const std::vector<std::vector<T>>& data)
{
	m_height = data.size();
	if (m_height > 0)
	{
		m_width = data[0].size();
	}
	m_data = data;

	return *this;
}

template<class T>
inline bool Array2D<T>::operator==(const Array2D<T>& rhs) const
{
	return m_data == rhs.m_data;
}


//might need to change this so that the value isn't copied
template<class T>
bool Array2D<T>::set(unsigned int x, unsigned int y, const T& value)
{
	if (x >= m_width || y >= m_height)
	{
		return false;
	}

	m_data[y][x] = value;
	return true;
}

//might need to change this so that the value isn't copied
template<class T>
void Array2D<T>::setAndResize(unsigned int x, unsigned int y, const T& value)
{
	setSize(x, y);
	set(x, y, value);
}

template<class T>
void Array2D<T>::setAll(const T& value)
{
	for (int i = 0; i < m_width; ++i)
	{
		for (int j = 0; j < m_height; ++j)
		{
			set(i, j, value);
		}
	}
}

template<class T>
void Array2D<T>::swapRows(unsigned int r1, unsigned int r2)
{
	std::vector<T> temp = m_data[r1];
	m_data[r1] = m_data[r2];
	m_data[r2] = temp;
}

template<class T>
inline void Array2D<T>::swapColumns(unsigned int c1, unsigned int c2)
{
	for (int i = 0; i < m_height; ++i)
	{
		T temp = m_data[i][c1];
		m_data[i][c1] = m_data[i][c2];
		m_data[i][c2] = temp;
		m_data[i][c2] = temp;
	}
}

template<class T>
inline void Array2D<T>::setSize(unsigned int x, unsigned int y)
{
	
	if (x != m_width)
	{		
		m_width = x;
		for (int i = 0; i < m_height; ++i)
		{
			m_data[i].resize(m_width);
		}
	}

	if (y != m_height)
	{
		m_height = y;
		m_data.resize(y);
		for (int i = 0; i < y; ++i)
		{
			m_data[i].resize(m_width);
		}
	}
}

template<class T>
inline void Array2D<T>::transpose()
{
	std::vector<std::vector<T>> newData;
	newData.resize(m_width);
	for (int i = 0; i < m_width; ++i)
	{
		newData[i].resize(m_height);
	}

	for (int i = 0; i < m_height; ++i)
	{
		for (int j = 0; j < m_width; ++j)
		{
			newData[j][i] = m_data[i][j];
		}
	}

	m_width = m_height;
	m_height = newData.size();
	m_data = newData;
}

template<class T>
T& Array2D<T>::operator()(unsigned int x, unsigned int y)
{
	/*if (x >= m_width || y >= m_height)
	{
		return NULL;
	}*/

	return m_data[y][x];
}


