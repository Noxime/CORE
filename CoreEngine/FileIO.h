#pragma once

#include "Platform.h"

class FileIO
{
public:

	static std::vector<char> loadBinaryFile(std::string path);

	static std::vector<char> loadAsciiFile(std::string path);

};

