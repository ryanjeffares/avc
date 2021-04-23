// Main.cpp : Defines the entry point for the application.
#include <string>
#include <iostream>
#include <chrono>
#include "AlsIOHandler.h"

int main(int argc, char* argv[])
{
#ifdef _DEBUG
	auto start = std::chrono::high_resolution_clock::now();
#endif
	if (argc != 2) {
		LOG("Expected 1 argument: path to .als file, stopping exection.");
		return 0;
	}
	LOG("Given path: %s", argv[1]);
	std::string path(argv[1]);
	if (path.compare(path.length() - 4, 4, ".als")) {
		LOG("Given path is not a .als file, stopping exection.");
		return 0;
	}
	else {
		auto output = path;
		output.erase(output.end() - 4, output.end());
		output += ".xml";
		LOG("Output: %s", output.c_str());
		avc::AlsIOHandler ioHandler(path, output);
		ioHandler.decompress();
		ioHandler.storeXmlData();
		ioHandler.writeToXml();
		ioHandler.writeToAls();
	}
#ifdef _DEBUG
	auto end = std::chrono::high_resolution_clock::now();
	auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	DBG("Completed in %Id ms", dur);
#endif
	std::cin.get();
	return 0;
}
