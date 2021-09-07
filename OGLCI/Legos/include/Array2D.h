#pragma once
#include <vector>
#include <algorithm>
#include <iterator>
#include "LoggingUtils.h"

//Row major

template <class T>
class Array2D
{
private:
	T* m_data;
	unsigned int m_width;
	unsigned int m_height;

public:
	typedef T* iterator;
	typedef const T* const_iterator;
	iterator begin() { return &m_data[0]; };
	iterator end() { return &m_data[m_width * m_height]; };

	Array2D() :m_data(nullptr), m_width(0), m_height(0) {};
	Array2D(unsigned int width, unsigned int height);
	Array2D(unsigned int width, unsigned int height, T* data) : m_width(width), m_height(height), m_data(data) {};
	Array2D(const std::vector<std::vector<T>> data);
	Array2D(const Array2D<T>& rhs);
	~Array2D();

	Array2D<T> operator=(const Array2D<T>& rhs);
	Array2D<T> operator=(const std::vector<std::vector<T>>& data);
	Array2D<T> operator=(const std::vector<T>& data);
	bool operator==(const Array2D<T>& rhs) const;

	bool set(unsigned int x, unsigned int y, const T& value);
	void setAndResize(unsigned int x, unsigned int y, const T& value);
	void setAll(T value);
	void swapRows(unsigned int r1, unsigned int r2);
	void swapColumns(unsigned int c1, unsigned int c2);
	void setSize(unsigned int x, unsigned int y);
	void setData(unsigned int width, unsigned int height, T* data);
	void setData(unsigned int width, unsigned int height, std::vector<T>& data);

	std::string logString();

	T& operator()(unsigned int x, unsigned int y);

	inline unsigned int getWidth() const { return m_width; };
	inline unsigned int getHeight() const { return m_height; };
	inline T* getData() { return m_data; };

	Array2D<T> getRow(unsigned int r);
	Array2D<T> getColumn(unsigned int c);
	void transpose();

};

template<class T>
Array2D<T>::Array2D(unsigned int width, unsigned int height)
	:m_data(nullptr),
	m_width(width),
	m_height(height)
{
	m_data = new T[height * width];
}

template<class T>
inline Array2D<T>::Array2D(const std::vector<std::vector<T>> data)
	:m_data(nullptr)
{
	m_height = static_cast<unsigned int>(data.size());
	if (m_height > 0)
	{
		m_width = static_cast<unsigned int>(data[0].size());
	}

	if (m_width > 0 && m_height > 0)
	{
		m_data = new T[m_height * m_width];

		for (unsigned int i = 0; i < m_height; ++i)
		{
			memcpy(&m_data[i * m_width], data[i].data(), sizeof(T) * m_width);
		}
	}
}

template<class T>
inline Array2D<T>::Array2D(const Array2D<T>& rhs)
{
	m_height = rhs.m_height;
	m_width = rhs.m_width;

	m_data = new T[m_height * m_width];
	memcpy(m_data, rhs.m_data, sizeof(T) * m_height * m_width);
}

template<class T>
inline Array2D<T>::~Array2D()
{
	if (m_data)
	{
		delete[] m_data;
	}
}

template<class T>
inline Array2D<T> Array2D<T>::operator=(const Array2D<T>& rhs)
{
	m_height = rhs.m_height;
	m_width = rhs.m_width;

	if (m_data)
	{
		delete[] m_data;
	}

	m_data = new T[m_height * m_width];
	memcpy(m_data, rhs.m_data, sizeof(T) * m_height * m_width);

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

	if (m_width > 0 && m_height > 0)
	{
		if (m_data)
		{
			delete[] m_data;
		}

		m_data = new T[m_height * m_width];

		for (int i = 0; i < m_height; ++i)
		{
			std::copy(data[i].begin(), data[i].end(), std::begin(&m_data[i * m_width]));
			//memcpy(m_data + i * m_width * sizeof(T), data[i].data(), sizeof(T) * m_width);
		}
	}
	

	return *this;
}

template<class T>
inline bool Array2D<T>::operator==(const Array2D<T>& rhs) const
{
	if (m_width != rhs.m_width || m_height != rhs.m_height)
	{
		return false;
	}

	for (int i = 0; i < m_width * m_height; ++i)
	{
		if (m_data[i] != rhs.m_data[i])
		{
			return false;
		}
	}
	return true;
}


//might need to change this so that the value isn't copied
template<class T>
bool Array2D<T>::set(unsigned int x, unsigned int y, const T& value)
{
	if (x >= m_width || y >= m_height)
	{
		return false;
	}

	m_data[y * m_width + x] = value;
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
void Array2D<T>::setAll(T value)
{
	for (unsigned int i = 0; i < m_width * m_height; ++i)
	{
		m_data[i] = value;
	}
}

template<class T>
void Array2D<T>::swapRows(unsigned int r1, unsigned int r2)
{
	T* temp = new T[m_width];
	memcpy(temp, &m_data[r1 * m_width], sizeof(T) * m_width);
	memcpy(&m_data[r1 * m_width], &m_data[r2 * m_width], sizeof(T) * m_width);
	memcpy(&m_data[r2 * m_width], temp, sizeof(T) * m_width);
	delete[] temp;
}

template<class T>
inline void Array2D<T>::swapColumns(unsigned int c1, unsigned int c2)
{
	for (unsigned int i = 0; i < m_height; ++i)
	{
		T temp = m_data[i * m_width + c1];
		m_data[i * m_width + c1] = m_data[i * m_width + c2];
		m_data[i * m_width + c2] = temp;
	}
}

template<class T>
inline void Array2D<T>::setSize(unsigned int x, unsigned int y)
{
	if (m_width == 0 && m_height == 0 && m_data == nullptr)
	{
		m_width = x;
		m_height = y;
		m_data = new T[m_width * m_height];
	}
	else if (x != m_width || y != m_height)
	{
		//def need to reallocate
		T* newData = new T[y * x];
		if (m_height != y && m_width == x)
		{
			//messing with rows
			//if new height is smaller, memcpy will truncate data for me
			m_height = y;
			memcpy(newData, m_data, sizeof(T) * m_height * m_width);
		}
		else if (m_height == y && m_width != x)
		{
			//messing with columns
			if (x > m_width)
			{
				//adding columns
				for (unsigned int i = 0; i < m_height; ++i)
				{				
					memcpy(&newData[i * x], &m_data[i * m_width], sizeof(T) * m_height * m_width);				
				}
			}
			else
			{
				//removing columns
				for (unsigned int i = 0; i < m_height; ++i)
				{
					memcpy(&newData[i * x], &m_data[i * m_width], sizeof(T) * m_height * x);
				}
			}
			m_width = x;
		}
		else
		{
			//messing with rows and columns
			if (y > m_height)
			{
				if (x > m_width)
				{
					for (unsigned int i = 0; i < m_height; ++i)
					{
						memcpy(&newData[i * x], &m_data[i * m_width], sizeof(T) * m_height * m_width);
					}
				}
				else
				{
					for (unsigned int i = 0; i < m_height; ++i)
					{
						memcpy(&newData[i * x], &m_data[i * m_width], sizeof(T) * m_height * x);
					}
				}
			}
			else
			{
				if (x > m_width)
				{
					for (unsigned int i = 0; i < y; ++i)
					{
						memcpy(&newData[i * x], &m_data[i * m_width], sizeof(T) * m_height * m_width);
					}
				}
				else
				{
					for (unsigned int i = 0; i < y; ++i)
					{
						memcpy(&newData[i * x], &m_data[i * m_width], sizeof(T) * m_height * x);
					}
				}
			}
			m_width = x;
			m_height = y;
		}
		delete[] m_data;
		m_data = newData;
	}	
}

template<class T>
inline void Array2D<T>::setData(unsigned int width, unsigned int height, T* data)
{
	m_width = width;
	m_height = height;
	if (m_data)
	{
		delete[] m_data;
	}
	m_data = data;
}

template<class T>
inline void Array2D<T>::setData(unsigned int width, unsigned int height, std::vector<T>& data)
{
	if (width * height != data.size())
	{
		return;
	}

	m_width = width;
	m_height = height;
	if (m_data)
	{
		delete[] m_data;
	}

	m_data = new T[m_width * m_height];

	//maybe play around with using memcpy here.  i am not sure if it would cause problems because the vector could reallocate outside of this function?
	for (int i = 0; i < m_width * m_height; ++i)
	{
		m_data[i] = data[i];
	}
}

template<class T>
inline std::string Array2D<T>::logString()
{
	std::string toReturn = "\n";

	for (unsigned int i = 0; i < m_height; ++i)
	{
		for (unsigned int j = 0; j < m_width; ++j)
		{
			toReturn += logger::to_string< >(m_data[i * m_width + j]) + " ";
		}
		toReturn += "\n";
	}
	return toReturn;
}

template<class T>
T& Array2D<T>::operator()(unsigned int x, unsigned int y)
{
	//may need to throw something here but i dont understand throws enough :(
	/*if (x >= m_width || y >= m_height)
	{
		throw Exc
	}*/

	return m_data[y * m_width + x];
}

template<class T>
inline Array2D<T> Array2D<T>::getRow(unsigned int r)
{
	Array2D<T> toReturn;	
	if (r >= m_width)
	{
		return toReturn;
	}
	toReturn.setData(m_width, 1, m_data + (r * m_width));
	return toReturn;
}

template<class T>
inline Array2D<T> Array2D<T>::getColumn(unsigned int c)
{
	Array2D<T> toReturn(1, m_height);
	for (int j = 0; j < m_height; ++j)
	{
		toReturn(1, j) = m_data[j * m_width + c];
	}
	return toReturn;
}

template<class T>
inline void Array2D<T>::transpose()
{
	if (m_data)
	{
		T* tempData = new T[m_height * m_width];
		for (int j = 0; j < m_height; ++j)
		{
			for (int i = j; i < m_width; ++i)
			{
				tempData[i * m_height + j] = m_data[j * m_width + i];
			}
		}
		delete[] m_data;
		m_data = tempData;
	}
	unsigned int temp = m_width;
	m_width = m_height;
	m_height = temp;
}


