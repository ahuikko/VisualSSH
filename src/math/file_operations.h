#pragma once
#include <string>

class FileOperations
{


public:
	FileOperations();
	~FileOperations();


	bool Browse(char* buffer);

	std::string fileToString(const std::string& filePath);
	void stringToFile(const std::string& filePath, const std::string& data);





};

