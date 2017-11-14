#include "IOManager.h"
#include <fstream>

namespace Engine {
	
bool IOManager::readFileToBuffer(const std::string& filePath, std::string& buffer)
{
	// Load the file
	std::ifstream file(filePath, std::ios::binary);
	if (file.fail()) {
		perror(filePath.c_str());
		return false;
	}

	// Seek to the end
	file.seekg(0, std::ios::end);

	// Get the file size
	int fileSize = static_cast<int>(file.tellg());

	// Seek back to the beginning
	file.seekg(0, std::ios::beg);

	// Remove the header from the file size now that we're in the beginning
	fileSize -= static_cast<int>(file.tellg());

	// Resize the buffer to the size of the file
	buffer.resize(fileSize);

	// Read the file
	file.read((char *)&(buffer[0]), fileSize);

	// Close the file
	file.close();

	return true;
}

}
