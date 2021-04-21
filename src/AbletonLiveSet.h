#pragma once
#include "ableton_data_types/AbletonHeader.h"
#include "ableton_data_types/Track.h"
#include "ableton_data_types/SmallNodes.h"
#include "ableton_data_types/ViewStates.h"
#include "../deps/tinyxml2/tinyxml2.h"
#include <string>
#include <map>
#include <memory>
#include <vector>

namespace avc {

	class AbletonLiveSet
	{

	public:
		AbletonLiveSet(std::string n, std::string v, std::string e, std::shared_ptr<ableton_data_types::AbletonHeader> ah)
			: name(n), xmlVersion(v), xmlEncoding(e), abletonHeader(ah) {}

		~AbletonLiveSet() {}

		std::map<std::string, int> intValues;
		std::map<std::string, int> intValuesLomId;
		std::map<std::string, bool> boolValues;
		std::map<std::string, int> videoRect;

		std::string viewData, annotation;
		int autoColourPickerPlayerTracks, autoColourPickerMasterTracks;

		std::shared_ptr<ableton_data_types::AbletonHeader> abletonHeader;

		std::shared_ptr<ableton_data_types::ContentSplitterProperties> contentSplitterProperties;
		std::shared_ptr<ableton_data_types::SequencerNavigator> sequencerNavigator;
		std::shared_ptr<ableton_data_types::TimeSelection> timeSelection;
		std::shared_ptr<ableton_data_types::ScaleInformation> scaleInformation;
		std::shared_ptr<ableton_data_types::Grid> grid;
		std::shared_ptr<ableton_data_types::Transport> transport;
		std::shared_ptr<ableton_data_types::ViewStates> viewStates;

		std::vector<std::shared_ptr<ableton_data_types::Track>> tracks;
		std::shared_ptr<ableton_data_types::Track> masterTrack;

	private:
		std::string name, xmlVersion, xmlEncoding;

	};	// class AbletonLiveSet

}	// namespace avc