#pragma once
#include "avc.h"

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
		void getValues();
		void getTracks();
		void getMasterTrack();
		void getViewStates();

		const std::string inputPath, outputPath;
		std::string setName;

	};	// class AlsIOHandler

}	// namespace avc
