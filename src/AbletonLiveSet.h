#pragma once
#include "ableton_data_types/Track.h"

namespace avc {

	using namespace tinyxml2;

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

		std::vector<std::shared_ptr<ableton_data_types::Track>> tracks;
		std::unique_ptr<ableton_data_types::Track> masterTrack, preHearTrack;

		void createXmlNode(XMLDocument& doc) {
			auto header = doc.NewElement("Ableton");
			header->SetAttribute("MajorVersion", abletonHeader->majorVersion);
			header->SetAttribute("MinorVersion", abletonHeader->minorVersion.c_str());
			header->SetAttribute("SchemaChangeCount", abletonHeader->schemaChangeCount);
			header->SetAttribute("Creator", abletonHeader->creator.c_str());
			header->SetAttribute("Revision", abletonHeader->revision.c_str());
			auto liveSetNode = doc.NewElement("LiveSet");

			for (auto& i : intValues) {
				auto el = liveSetNode->InsertNewChildElement(i.first.c_str());
				el->SetAttribute("Value", i.second);
			}
			for (auto& il : intValuesLomId) {
				auto el = liveSetNode->InsertNewChildElement(il.first.c_str());
				el->SetAttribute("LomId", il.second);
			}
			for (auto& b : boolValues) {
				auto el = liveSetNode->InsertNewChildElement(b.first.c_str());
				el->SetAttribute("Value", b.second);
			}

			auto trackNode = doc.NewElement("Tracks");
			for (auto& t : tracks) {
				t->createXmlNode(doc, trackNode);
			}
			liveSetNode->InsertEndChild(trackNode);
			masterTrack->createXmlNode(doc, liveSetNode);
			preHearTrack->createXmlNode(doc, liveSetNode);

			auto viewDataEl = liveSetNode->InsertNewChildElement("ViewData");
			viewDataEl->SetAttribute("Value", viewData.c_str());
			auto annotationEl = liveSetNode->InsertNewChildElement("Annotation");
			annotationEl->SetAttribute("Value", annotation.c_str());

			auto videoRectEl = liveSetNode->InsertNewChildElement("VideoWindowRect");
			for (auto& v : videoRect) {
				videoRectEl->SetAttribute(v.first.c_str(), v.second);
			}

			contentSplitterProperties->createXmlNode(doc, liveSetNode);
			sequencerNavigator->createXmlNode(doc, liveSetNode);
			timeSelection->createXmlNode(doc, liveSetNode);
			scaleInformation->createXmlNode(doc, liveSetNode);
			grid->createXmlNode(doc, liveSetNode);
			viewStates->createXmlNode(doc, liveSetNode);
			transport->createXmlNode(doc, liveSetNode);

			auto playerColourPickerNode = doc.NewElement("AutoColorPickerForPlayerAndGroupTracks");
			auto pcpEl = playerColourPickerNode->InsertNewChildElement("NextColorIndex");
			pcpEl->SetAttribute("Value", autoColourPickerPlayerTracks);

			auto masterColourPickerNode = doc.NewElement("AutoColorPickerForReturnAndMasterTracks");
			auto mcpEl = masterColourPickerNode->InsertNewChildElement("NextColorIndex");
			mcpEl->SetAttribute("Value", autoColourPickerMasterTracks);

			auto sendsPreEl = liveSetNode->InsertNewChildElement("SendsPre");
			for (auto& sp : sendsPre) {
				sp.createXmlNode(doc, sendsPreEl);
			}

			header->InsertEndChild(liveSetNode);
			doc.InsertEndChild(header);
		}

	private:
		std::string name, xmlVersion, xmlEncoding;

	};	// class AbletonLiveSet

}	// namespace avc