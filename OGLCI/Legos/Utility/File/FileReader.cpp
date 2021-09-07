#include "FileReader.h"
#include "Logger.h"
#include <algorithm>

bool FileReader::open()
{
	close();
	file.open(fileName, std::ios::in);
	if (!file.is_open())
	{
		ERROR_LOG("failed to open file");
		return false;
	}
	return true;
}
bool FileReader::openBinary()
{
	close();
	file.open(fileName, std::ios::binary);
	if (!file.is_open())
	{
		ERROR_LOG("failed to open file");
		return false;
	}
	return true;
}

std::string FileReader::readLine()
{
	std::string toReturn;
	getline(file, toReturn);
	return toReturn;
}

int FileReader::getInt32()
{	
	int toReturn = 0;
	if (file.is_open())
	{
		char c[5];
		file.get(c, 5);
		std::reverse(c, c + 4);
		std::memcpy(&toReturn, c, 4);
	}
	else
	{
		ERROR_LOG("file is not open");
	}
	return toReturn;
}

char FileReader::getByte()
{
	char c;
	if (file.is_open())
	{		
		file.get(c);
	}
	else
	{
		ERROR_LOG("file is not open");
	}
	return c;
}

unsigned char FileReader::getUByte()
{
	char c;
	if (file.is_open())
	{
		file.get(c);
	}
	else
	{
		ERROR_LOG("file is not open");
	}
	return (unsigned char)c;
}


std::vector<std::vector<std::string>> FileReader::readStringCSV()
{
	std::string delimiter = ",";
	std::vector<std::vector<std::string>> toReturn;
	if (open())
	{
		std::string line;
		std::string token;

		try
		{

			while (!file.eof())
			{
				line = readLine();
				toReturn.emplace_back();
				size_t pos = 0;
				while ((pos = line.find(delimiter)) != std::string::npos)
				{
					token = line.substr(0, pos);
					toReturn[toReturn.size() - 1].push_back(token);
					line.erase(0, pos + delimiter.length());
				}
			}
		}
		catch (...)
		{
			ERROR_LOG("failed to read file");
		}
		close();
	}
	

	return toReturn;
}

std::vector<std::vector<float>> FileReader::readNumericalCSV()
{
	std::string delimiter = ",";
	std::vector<std::vector<float>> toReturn;
	if (open())
	{
		std::string line;
		std::string token;
		try
		{
			while (!file.eof())
			{
				line = readLine();
				toReturn.emplace_back();
				size_t pos = 0;
				while ((pos = line.find(delimiter)) != std::string::npos)
				{
					token = line.substr(0, pos);
					float value = std::stof(token);
					toReturn[toReturn.size() - 1].push_back(value);
					line.erase(0, pos + delimiter.length());
				}
			}
		}
		catch (std::invalid_argument e)
		{
			ERROR_LOG("could not read value as float");
		}
		catch (std::out_of_range e)
		{
			ERROR_LOG("could not read value as float");
		}
		catch (...)
		{
			ERROR_LOG("failed to read file");
		}
		close();
	}

	return toReturn;
}

void FileReader::close()
{
	if (file.is_open())
	{
		file.close();
	}
}

FileReader::~FileReader()
{
	close();
}
