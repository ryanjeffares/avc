#pragma once
#include "../avc.h"
#include "SmallNodes.h"

namespace avc {
	namespace ableton_data_types {

		using namespace tinyxml2;

		class Track
		{

		public:
			enum TrackType
			{
				MIDI_TRACK, AUDIO_TRACK, RETURN_TRACK, MASTER_TRACK, PRE_HEAR_TRACK
			};			

			Track(int _id, TrackType _type) : id(_id), type(_type) {}				
			Track(TrackType _type) : type(_type) {}				

			int id;
			std::map<std::string, int> intValues, intValuesLomId;
			std::map<std::string, bool> boolValues;
			std::string viewData;
			std::shared_ptr<TrackDelay> trackDelay;
			std::shared_ptr<Name> name;
			TrackType type;

			void createXmlNode(XMLDocument& doc, XMLNode* parent) const {
				const char* typeName;
				switch (type) {
					case MASTER_TRACK:
						typeName = "MasterTrack";
						break;
					case AUDIO_TRACK:
						typeName = "AudioTrack";
						break;
					case MIDI_TRACK:
						typeName = "MidiTrack";
						break;
					case RETURN_TRACK:
						typeName = "ReturnTrack";
						break;
					case PRE_HEAR_TRACK:
						typeName = "PreHearTrack";
						break;
				}
				auto node = doc.NewElement(typeName);
				if (type != MASTER_TRACK && type != PRE_HEAR_TRACK) {
					node->SetAttribute("Id", id);
				}
				for (auto& iVal : intValues) {
					auto el = node->InsertNewChildElement(iVal.first.c_str());
					el->SetAttribute("Value", iVal.second);
				}
				for (auto& iValL : intValuesLomId) {
					auto el = node->InsertNewChildElement(iValL.first.c_str());
					el->SetAttribute("LomId", iValL.second);
				}
				for (auto& bVal : boolValues) {
					auto el = node->InsertNewChildElement(bVal.first.c_str());
					el->SetAttribute("Value", bVal.second);
				}
				auto vdEl = node->InsertNewChildElement("ViewData");
				vdEl->SetAttribute("Value", viewData.c_str());
				trackDelay->createXmlNode(doc, node);
				name->createXmlNode(doc, node);
				parent->InsertEndChild(node);
			}

		};

	}	// namespace ableton_data_types
}	// namespace avc