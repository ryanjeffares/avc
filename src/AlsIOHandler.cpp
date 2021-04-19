#include "AlsIOHandler.h"

namespace avc {

	AlsIOHandler::AlsIOHandler(std::string ip, std::string xp)
		: inputPath(ip), outputPath(xp) {
		auto cleanedName = inputPath.substr(inputPath.find_last_of("/"));
		cleanedName.erase(cleanedName.end() - 4, cleanedName.end());
		setName = cleanedName;
	}

}	// namespace avc