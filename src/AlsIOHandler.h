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

	using namespace tinyxml2;
	using namespace ableton_data_types;

	class AlsIOHandler
	{

	public:

		AlsIOHandler(std::string ip, std::string xmlPath);
		~AlsIOHandler();
		
		void decompress();
		void storeXmlData();
		void writeToXml();
		void writeToAls();
	
	private:		

		void getValues(XMLNode* node);
		void getTracks(XMLElement* node);
		void getTrackInfo(std::shared_ptr<Track>& track, XMLNode* node);
		void getMasterTrack(XMLElement* node);

		const std::string inputPath, outputPath;
		std::string setName;

		std::unique_ptr<AbletonLiveSet> abletonLiveSet;

	};	// class AlsIOHandler

}	// namespace avc
