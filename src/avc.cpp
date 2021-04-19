﻿// avc.cpp : Defines the entry point for the application.
//

#include "avc.h"

int main(int argc, char* argv[])
{
	if (argc != 2) {
		std::cout << "Expected 1 argument: path to .als file, stopping exection." << std::endl;
		return 0;
	}
	std::cout << "Given path: " << argv[1] << std::endl;
	std::string path(argv[1]);
	if (path.compare(path.length() - 4, 4, ".als")) {
		std::cout << "Given path is not a .als file, stopping exection." << std::endl;
		return 0;
	}
	else {
		
	}
	std::cin.get();
	return 0;
}
