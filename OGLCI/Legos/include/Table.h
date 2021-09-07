#pragma once
#include "Array2D.h"
#include <map>
#include <string>
#include <vector>
#include "Logger.h"

template <class T>
class Table
{
private:
	Array2D<T> m_data;
	std::map<std::string, unsigned int> m_columnNames;
	std::vector<std::string>m_orderedNames;
	std::string m_name;


public:

	Table() : m_data(), m_columnNames(), m_name(){};
	Table(std::string name) : m_data(), m_columnNames(), m_orderedNames(), m_name(name){};
	Table(std::string name, std::vector<std::string> columnNames) : m_data(), m_columnNames(), m_orderedNames(columnNames), m_name(name)
	{
		for (int i = 0; i < columnNames.size(); ++i)
		{
			m_columnNames[columnNames[i]] = i;
		}
	};

	Table(std::vector<std::string> columnNames)
	{
		Table("", columnNames);
	};

	bool addRow(std::vector<T>& data);
	std::string logString();

};

template<class T>
inline bool Table<T>::addRow(std::vector<T>& data)
{
	if (m_orderedNames.size() == 0)
	{
		ERROR_LOG("there are no configured columns");
		return false;
	}

	if (m_orderedNames.size() != data.size())
	{
		ERROR_LOG("columns size does not match row size");
		return false;
	}

	m_data.setSize(m_orderedNames.size(), m_data.getHeight() + 1);
	for (int i = 0; i < data.size(); ++i)
	{
		m_data(i, m_data.getHeight() - 1) = data[i];
	}

	return true;
}

template<class T>
inline std::string Table<T>::logString()
{
	
	std::string toReturn = "\n";
	std::vector<int> columnLengths;
	columnLengths.reserve(m_orderedNames.size());

	for (unsigned int i = 0; i < m_orderedNames.size(); ++i)
	{
		columnLengths.push_back(m_orderedNames[i].size());
	}

	for (unsigned int i = 0; i < m_data.getWidth(); ++i)
	{		
		for (unsigned int j = 0; j < m_data.getHeight(); ++j)
		{
			columnLengths[i] = std::max(columnLengths[i], (int)logger::to_string< >(m_data(i, j)).size());
		}
	}

	//table name
	toReturn = toReturn + m_name + "\n";

	//row headers
	for (unsigned int i = 0; i < m_orderedNames.size(); ++i)
	{
		char* extra = new char[columnLengths[i] - m_orderedNames[i].size() + 1];
		for (int j = 0; j < columnLengths[i] - m_orderedNames[i].size(); ++j)
		{
			extra[j] = ' ';
		}
		extra[columnLengths[i] - m_orderedNames[i].size()] = '\0';

		std::string s;
		if (i == m_orderedNames.size() - 1)
		{
			s = m_orderedNames[i] + std::string(extra);
		}
		else
		{
			s = m_orderedNames[i] + std::string(extra) + "|";
		}
		toReturn += s;
		delete[] extra;
	}
	toReturn = toReturn + "\n";
	
	//seperator line
	for (unsigned int i = 0; i < columnLengths.size(); ++i)
	{
		char* extra = new char[columnLengths[i] + 1];
		for (int j = 0; j < columnLengths[i]; ++j)
		{
			extra[j] = '-';
		}
		extra[columnLengths[i]] = '\0';

		std::string s;
		if (i == columnLengths.size() - 1)
		{
			s = std::string(extra);
		}
		else
		{
			s = std::string(extra) + "|";
		}
		toReturn += s;
		delete[] extra;
	}
	toReturn = toReturn + "\n";

	//values
	for (unsigned int j = 0; j < m_data.getHeight(); ++j)
	{
		for (unsigned int i = 0; i < m_data.getWidth(); ++i)
		{
			std::string value = logger::to_string< >(m_data(i, j));
			char* extra = new char[columnLengths[i] - value.size() + 1];
			for (int j = 0; j < columnLengths[i] - value.size(); ++j)
			{
				extra[j] = ' ';
			}
			extra[columnLengths[i] - value.size()] = '\0';

			std::string s;
			if (i == m_data.getWidth() - 1)
			{
				s = std::string(extra) + value;
			}
			else
			{
				s = std::string(extra) + value  + "|";
			}
			toReturn += s;
			delete[] extra;
		}
		toReturn += "\n";
	}

	return toReturn;
}
