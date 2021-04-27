#pragma once
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
		void getTrackInfo(Track* track, XMLNode* node);
		void getMasterTrack(XMLElement* node);
		void getPreHearTrack(XMLElement* node);

		const std::string inputPath, outputPath;
		std::string setName;

		std::unique_ptr<AbletonLiveSet> abletonLiveSet;

	};	// class AlsIOHandler

}	// namespace avc
