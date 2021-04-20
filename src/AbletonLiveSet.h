#pragma once
#include "ableton_data_types/AbletonHeader.h"
#include "ableton_data_types/tracks/AudioTrack.h"
#include "ableton_data_types/tracks/MidiTrack.h"
#include "ableton_data_types/tracks/ReturnTrack.h"
#include "ableton_data_types/tracks/MasterTrack.h"
#include "ableton_data_types/SmallNodes.h"
#include "ableton_data_types/ViewStates.h"
#include <string>
#include <map>
#include <memory>
#include <vector>

namespace avc {

	class AbletonLiveSet
	{

	public:
		AbletonLiveSet(std::string n, std::string v, std::string e, std::shared_ptr<ableton_data_types::AbletonHeader> ah);
		~AbletonLiveSet();

		std::map<std::string, int> intValues;
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

		std::vector<ableton_data_types::tracks::MidiTrack> midiTracks;
		std::vector<ableton_data_types::tracks::AudioTrack> audioTracks;
		std::vector<ableton_data_types::tracks::ReturnTrack> returnTracks;
		std::vector<ableton_data_types::tracks::MasterTrack> masterTrack;

	private:
		std::string name, xmlVersion, xmlEncoding;

	};	// class AbletonLiveSet

}	// namespace avc