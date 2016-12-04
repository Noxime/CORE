#include "FileIO.h"

std::vector<char> FileIO::loadBinaryFile(std::string path)
{
	std::ifstream file(path, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		std::cout << "IO: Failed to open file: " << path << std::endl;
		return std::vector<char>();
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}


std::vector<char> FileIO::loadAsciiFile(std::string path)
{
	std::vector<char> vec;
	std::ifstream file(path);
	assert(file.is_open());
	if (!file.eof() && !file.fail())
	{
		file.seekg(0, std::ios_base::end);
		std::streampos fileSize = file.tellg();
		vec.resize(fileSize);

		file.seekg(0, std::ios_base::beg);
		file.read(&vec[0], fileSize);
	}

	return vec;
}