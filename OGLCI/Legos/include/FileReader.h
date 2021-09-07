#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "Array2D.h"
#include <vector>

class FileReader
{
protected:
	std::string fileName;
	std::ifstream file;

public:
	FileReader() : fileName("") {};
	FileReader(std::string name) : fileName(name) {};

	void setName(std::string name)
	{
		close();
		fileName = name;
	}

	bool open();
	bool openBinary();
	std::string readLine();
	int getInt32();
	char getByte();
	unsigned char getUByte();
	std::vector<std::vector<std::string>> readStringCSV();
	std::vector<std::vector<float>> readNumericalCSV();
	void close();

	~FileReader();
};