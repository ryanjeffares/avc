#pragma once
#include "avc.h"
#include "ableton_data_types/AbletonHeader.h"
#include <map>
#include <memory>

namespace avc {

	class AbletonLiveSet
	{

	public:
		AbletonLiveSet(std::string n, std::string v, std::string e);
		~AbletonLiveSet();

		std::map<std::string, int> intValues;
		std::map<std::string, bool> boolValues;
		std::map<std::string, int> videoRect;

		std::string viewData;
		int autoColourPickerPlayerTracks, autoColourPickerMasterTracks;

		std::unique_ptr<ableton_data_types::AbletonHeader> abletonHeader;

	private:
		std::string name, xmlVersion, xmlEncoding;

	};	// class AbletonLiveSet

}	// namespace avc