#pragma once

#include <string>

namespace Engine {

class IOManager
{
public:
	static bool readFileToBuffer(const std::string& filePath, std::string& buffer);
};

}