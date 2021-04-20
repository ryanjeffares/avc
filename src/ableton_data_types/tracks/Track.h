#pragma once
#include <string>
#include <memory>
#include <map>

namespace avc {
	namespace ableton_data_types {
		namespace tracks {

			class Track
			{
			public:
				enum TrackType
				{
					MIDI_TRACK, AUDIO_TRACK, RETURN_TRACK, MASTER_TRACK
				};

				struct TrackDelay
				{
					const int value;
					const bool isValueSampleBased;

					TrackDelay(int v, bool s)
						: value(v), isValueSampleBased(s) {}
				};

				struct Name
				{
					const std::string effectiveName, userName, annotation, memorizedFirstClip;

					Name(std::string en, std::string un, std::string a, std::string m)
						: effectiveName(en), userName(un), annotation(a), memorizedFirstClip(m) {}
				};

				Track(int _id, TrackType _type) : id(_id), type(_type) {}

				int id;
				std::map<std::string, int> intValues;
				std::map<std::string, bool> boolValues;
				std::string viewData;
				std::unique_ptr<TrackDelay> trackDelay;
				std::unique_ptr<Name> name;
				TrackType type;

				virtual void createXmlNode() const {

				}

			};

		}	// namespace tracks
	}	// namespace ableton_data_types
}	// namespace avc