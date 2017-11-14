#pragma once

#include <string>
#include <iostream>
#include <GL/glew.h>
#include <SDL/SDL.h>

namespace Logger {
	static void Log(const std::string& message) {
		std::cout << "DEBUG :: " << message << std::endl;
	}

	static void Logf(const char *format, const GLubyte *arg) {
		std::cout << "DEBUG :: ";
		printf(format, arg);
		std::cout << std::endl;
	}

	static void Warning(const std::string& message) {
		std::cout << "WARNING :: " << message << std::endl;
	}

	// Logs the error message to console and exists the program
	static void Error(const std::string& message) {
		std::cout << "ERROR :: " << message << std::endl;
		std::cout << "Press any key to exit..." << std::endl;

		getchar();
		SDL_Quit();
		exit(69);
	}
}