#pragma once
#include "avc.h"
#include "ableton_data_types/AbletonHeader.h"
#include "ableton_data_types/Track.h"
#include "ableton_data_types/ViewStates.h"

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

		std::unique_ptr<ableton_data_types::ContentSplitterProperties> contentSplitterProperties;
		std::unique_ptr<ableton_data_types::SequencerNavigator> sequencerNavigator;
		std::unique_ptr<ableton_data_types::TimeSelection> timeSelection;
		std::unique_ptr<ableton_data_types::ScaleInformation> scaleInformation;
		std::unique_ptr<ableton_data_types::Grid> grid;
		std::unique_ptr<ableton_data_types::Transport> transport;
		std::unique_ptr<ableton_data_types::ViewStates> viewStates;

		std::vector<ableton_data_types::SendsPre> sendsPre;

		std::vector<ableton_data_types::Track> tracks;
		std::unique_ptr<ableton_data_types::Track> masterTrack, preHearTrack;

	private:
		std::string name, xmlVersion, xmlEncoding;

	};	// class AbletonLiveSet

}	// namespace avc