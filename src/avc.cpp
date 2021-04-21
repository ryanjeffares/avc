// avc.cpp : Defines the entry point for the application.
//

#include "avc.h"

int main(int argc, char* argv[])
{
	auto start = std::chrono::high_resolution_clock::now();
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
		auto output = path;
		output.erase(output.end() - 4, output.end());
		output += ".xml";
		std::cout << "Output: " << output << std::endl;
		avc::AlsIOHandler ioHandler(path, output);
		ioHandler.decompress();
		ioHandler.storeXmlData();
		ioHandler.writeToXml();
		ioHandler.writeToAls();
	}
	auto end = std::chrono::high_resolution_clock::now();
	auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << "Completed in " << dur << " ms" << std::endl;
	std::cin.get();
	return 0;
}
