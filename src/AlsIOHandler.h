#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include "../deps/tinyxml2/tinyxml2.h"
#include "AbletonLiveSet.h"

namespace avc {

	class AlsIOHandler
	{

	public:

		AlsIOHandler(std::string ip, std::string xmlPath);
		~AlsIOHandler();
		
		void decompress();
		void storeXmlData();
		void writeToXml();
	
	private:

		void getValues(tinyxml2::XMLNode* node);
		void getTracks(tinyxml2::XMLNode* node);
		void getMasterTrack(tinyxml2::XMLNode* node);
		void getViewStates(tinyxml2::XMLNode* node);

		const std::string inputPath, outputPath;
		std::string setName;

		std::unique_ptr<AbletonLiveSet> abletonLiveSet;

	};	// class AlsIOHandler

}	// namespace avc
