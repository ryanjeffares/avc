#include "AlsIOHandler.h"

namespace avc {

	AlsIOHandler::AlsIOHandler(std::string ip, std::string xp)
		: inputPath(ip), outputPath(xp) {
		auto pos = inputPath.find("\\");
		auto cleanedName = inputPath.substr(pos);
		setName = cleanedName.substr(0, cleanedName.length() - 4);
	}

	AlsIOHandler::~AlsIOHandler() {}

	void AlsIOHandler::decompress() {
		using namespace boost::iostreams;
		std::ifstream alsFile(inputPath, std::ios_base::in | std::ios_base::binary);
		filtering_streambuf<input> input;
		input.push(gzip_decompressor());
		input.push(alsFile);
		std::ofstream outputXml;
		outputXml.open(outputPath);
		copy(input, outputXml);
		outputXml.close();
		LOG("Wrote .als data to %s", outputPath.c_str());
	}

	void AlsIOHandler::storeXmlData() {			
		XMLDocument xmlDoc;
		auto loaded = xmlDoc.LoadFile(outputPath.c_str());
		if (loaded == XML_SUCCESS) {
			LOG("Successfully loaded XML file.");
			auto rootNode = xmlDoc.FirstChildElement();
			int majorVersion, schema;
			const char* minorVersion;
			const char* creator;
			const char* revision;
			rootNode->QueryIntAttribute("MajorVersion", &majorVersion);
			rootNode->QueryIntAttribute("SchemaChangeCount", &schema);
			rootNode->QueryStringAttribute("MinorVersion", &minorVersion);
			rootNode->QueryStringAttribute("Creator", &creator);
			rootNode->QueryStringAttribute("Revision", &revision);
			std::shared_ptr<ableton_data_types::AbletonHeader> header = std::make_shared<ableton_data_types::AbletonHeader>(majorVersion, minorVersion, schema, creator, revision);
			abletonLiveSet = std::make_unique<AbletonLiveSet>(setName, "1.0", "UTF-8", header);
			getValues(rootNode->FirstChild());
			getTracks(rootNode->FirstChild()->FirstChildElement("Tracks"));
			getMasterTrack(rootNode->FirstChild()->FirstChildElement("MasterTrack"));
			getPreHearTrack(rootNode->FirstChild()->FirstChildElement("PreHearTrack"));
		}
		else {
			LOG("Problem loading XML File: %d", loaded);
		}		
	}

	void AlsIOHandler::getValues(XMLNode* node) {
		auto& intValues = abletonLiveSet->intValues;
		auto& intValuesLomId = abletonLiveSet->intValuesLomId;
		auto& boolValues = abletonLiveSet->boolValues;
		auto& videoRect = abletonLiveSet->videoRect;
		auto currentElement = node->FirstChildElement();
		std::vector<std::string> nodeNames = {
			"AutoColorPickerForPlayerAndGroupTracks",
			"AutoColorPickerForReturnAndMasterTracks",
			"ContentSplitterProperties",
			"SequencerNavigator",
			"TimeSelection",
			"ScaleInformation",
			"Grid",			
			"ViewStates",
			"Transport",
			"SendsPre"
		};
		while (currentElement != nullptr) {			
			if (currentElement->NoChildren()) {
				//else if (currentElement->Value() == "DetailClipKeyMidis") {
				//	// TO-DO - find out what DetailClipKeyMidis is and implement it
				//}			
				int intVal;
				bool boolVal;
				const char* strVal;
				if (currentElement->QueryIntAttribute("Value", &intVal) == XML_SUCCESS) {
					intValues.emplace(currentElement->Value(), intVal);
				}
				else if (currentElement->QueryIntAttribute("LomId", &intVal) == XML_SUCCESS) {
					intValuesLomId.emplace(currentElement->Value(), intVal);
				}
				else if (currentElement->QueryBoolAttribute("Value", &boolVal) == XML_SUCCESS) {
					boolValues.emplace(currentElement->Value(), boolVal);
				}		
				else if (currentElement->QueryStringAttribute("Value", &strVal) == XML_SUCCESS) {
					if (strcmp(currentElement->Name(), "ViewData") == 0) {
						abletonLiveSet->viewData = strVal;
					}
					else if (strcmp(currentElement->Name(), "Annotation") == 0) {
						abletonLiveSet->annotation = strVal;
					}					
				}
				else if (strcmp(currentElement->Name(), "VideoWindowRect") == 0) {
					int top, left, bottom, right;
					currentElement->QueryIntAttribute("Top", &top);
					currentElement->QueryIntAttribute("Bottom", &bottom);
					currentElement->QueryIntAttribute("Left", &left);
					currentElement->QueryIntAttribute("Right", &right);
					videoRect.emplace("Top", top);
					videoRect.emplace("Bottom", bottom);
					videoRect.emplace("Left", right);
					videoRect.emplace("Right", right);
				}
				else {
					LOG("Problem parsing element %s", currentElement->Name());
				}						
			}
			else {
				try {
					auto it = std::find(nodeNames.begin(), nodeNames.end(), std::string(currentElement->Name()));
					int idx = it - nodeNames.begin();
					switch (idx) {
						case 0: {
							currentElement->FirstChildElement()->QueryIntAttribute("Value", &(abletonLiveSet->autoColourPickerPlayerTracks));
							break;
						}
						case 1: {
							currentElement->FirstChildElement()->QueryIntAttribute("Value", &(abletonLiveSet->autoColourPickerMasterTracks));
							break;
						}							
						case 2: {
							bool open;
							currentElement->FirstChildElement("Open")->QueryBoolAttribute("Value", &open);
							int size;
							currentElement->FirstChildElement("Size")->QueryIntAttribute("Value", &size);
							abletonLiveSet->contentSplitterProperties = std::make_unique<ableton_data_types::ContentSplitterProperties>(open, size);
							break;
						}
						case 3: {
							double zoom;
							int spx, spy, csx, csy;
							currentElement->FirstChildElement("BeatTimeHelper")->FirstChildElement("CurrentZoom")->QueryDoubleAttribute("Value", &zoom);
							currentElement->FirstChildElement("ScrollerPos")->QueryIntAttribute("X", &spx);
							currentElement->FirstChildElement("ScrollerPos")->QueryIntAttribute("Y", &spy);
							currentElement->FirstChildElement("ClientSize")->QueryIntAttribute("X", &csx);
							currentElement->FirstChildElement("ClientSize")->QueryIntAttribute("Y", &csy);
							abletonLiveSet->sequencerNavigator = std::make_unique<ableton_data_types::SequencerNavigator>(zoom, spx, spy, csx, csy);
							break;
						}
						case 4: {
							int at, ot;
							currentElement->FirstChildElement("AnchorTime")->QueryIntAttribute("Value", &at);
							currentElement->FirstChildElement("OtherTime")->QueryIntAttribute("Value", &ot);
							abletonLiveSet->timeSelection = std::make_unique<ableton_data_types::TimeSelection>(at, ot);
							break;
						}
						case 5: {
							int r;
							const char* n;
							currentElement->FirstChildElement("RootNote")->QueryIntAttribute("Value", &r);
							currentElement->FirstChildElement("Name")->QueryStringAttribute("Value", &n);
							abletonLiveSet->scaleInformation = std::make_unique<ableton_data_types::ScaleInformation>(r, std::string(n));
							break;
						}
						case 6: {
							int fn, fd, gip, nt;
							bool s, f;
							currentElement->FirstChildElement("FixedNumerator")->QueryIntAttribute("Value", &fn);
							currentElement->FirstChildElement("FixedDenominator")->QueryIntAttribute("Value", &fd);
							currentElement->FirstChildElement("GridIntervalPixel")->QueryIntAttribute("Value", &gip);
							currentElement->FirstChildElement("Ntoles")->QueryIntAttribute("Value", &nt);
							currentElement->FirstChildElement("SnapToGrid")->QueryBoolAttribute("Value", &s);
							currentElement->FirstChildElement("Fixed")->QueryBoolAttribute("Value", &f);
							abletonLiveSet->grid = std::make_unique<ableton_data_types::Grid>(fn, fd, gip, nt, s, f);
							break;
						}
						case 7: {
							abletonLiveSet->viewStates = std::make_unique<ableton_data_types::ViewStates>();
							auto el = currentElement->FirstChildElement();
							while (el != nullptr) {
								int val;
								el->QueryIntAttribute("Value", &val);
								abletonLiveSet->viewStates->elements.emplace(el->Name(), val);
								el = el->NextSiblingElement();
							}
							break;
						}							
						case 8: {
							double pnt, ls, ll, ct;
							int m;
							bool lo, lis, pi, po, d;
							currentElement->FirstChildElement("PhaseNudgeTempo")->QueryDoubleAttribute("Value", &pnt);
							currentElement->FirstChildElement("LoopOn")->QueryBoolAttribute("Value", &lo);
							currentElement->FirstChildElement("LoopStart")->QueryDoubleAttribute("Value", &ls);
							currentElement->FirstChildElement("LoopLength")->QueryDoubleAttribute("Value", &ll);
							currentElement->FirstChildElement("LoopIsSongStart")->QueryBoolAttribute("Value", &lis);
							currentElement->FirstChildElement("CurrentTime")->QueryDoubleAttribute("Value", &ct);
							currentElement->FirstChildElement("PunchIn")->QueryBoolAttribute("Value", &pi);
							currentElement->FirstChildElement("PunchOut")->QueryBoolAttribute("Value", &po);
							currentElement->FirstChildElement("MetronomeTickDuration")->QueryIntAttribute("Value", &m);
							currentElement->FirstChildElement("DrawMode")->QueryBoolAttribute("Value", &d);
							abletonLiveSet->transport = std::make_unique<ableton_data_types::Transport>(pnt, ls, ll, ct, m, lo, lis, pi, po, d);
							break;
						}
						case 9: {
							auto el = currentElement->FirstChildElement();
							while (el != nullptr) {
								int i;
								bool v;
								el->QueryIntAttribute("Id", &i);
								el->QueryBoolAttribute("Value", &v);
								ableton_data_types::SendsPre sp(i, v);
								abletonLiveSet->sendsPre.push_back(sp);
								el = el->NextSiblingElement();
							}
							break;
						}
					}
				}
				catch (XMLError& e) {
					LOG("Error parsing element %s%s%d", currentElement->Name(), ", returned ", e);
				}
			}
			currentElement = currentElement->NextSiblingElement();
		}
	}

	void AlsIOHandler::getTracks(XMLElement* parent) {		
		auto currentElement = parent->FirstChildElement();
		while (currentElement != nullptr) {
			int id;
			currentElement->QueryIntAttribute("Id", &id);
			if (strcmp(currentElement->Name(), "MidiTrack") == 0) {
				auto track = std::make_shared<Track>(id, Track::TrackType::MIDI_TRACK);
				getTrackInfo(track.get(), currentElement);
				abletonLiveSet->tracks.push_back(track);
			}
			else if (strcmp(currentElement->Name(), "AudioTrack") == 0) {
				auto track = std::make_shared<Track>(id, Track::TrackType::MIDI_TRACK);
				getTrackInfo(track.get(), currentElement);
				abletonLiveSet->tracks.push_back(track);
			}
			else if (strcmp(currentElement->Name(), "ReturnTrack") == 0) {
				auto track = std::make_shared<Track>(id, Track::TrackType::MIDI_TRACK);
				getTrackInfo(track.get(), currentElement);
				abletonLiveSet->tracks.push_back(track);
			}
			else {
				LOG("Invalid track type found for %s", currentElement->Name());
			}
			currentElement = currentElement->NextSiblingElement();
		}
	}

	void AlsIOHandler::getMasterTrack(XMLElement* parent) {
		abletonLiveSet->masterTrack = std::make_unique<Track>(Track::TrackType::MASTER_TRACK);
		getTrackInfo(abletonLiveSet->masterTrack.get(), parent);
	}

	void AlsIOHandler::getPreHearTrack(XMLElement* parent) {
		abletonLiveSet->preHearTrack = std::make_unique<Track>(Track::TrackType::PRE_HEAR_TRACK);
		getTrackInfo(abletonLiveSet->preHearTrack.get(), parent);
	}

	void AlsIOHandler::getTrackInfo(Track* track, XMLNode* parent) {
		auto currentElement = parent->FirstChildElement();
		while (currentElement != nullptr) {
			if (currentElement->NoChildren()) {
				int intVal;
				bool boolVal;
				const char* stringVal;
				if (currentElement->QueryIntAttribute("LomId", &intVal) == XML_SUCCESS) {
					track->intValuesLomId.emplace(currentElement->Name(), intVal);
				}
				else if (currentElement->QueryIntAttribute("Value", &intVal) == XML_SUCCESS) {
					track->intValues.emplace(currentElement->Name(), intVal);
				}
				else if (currentElement->QueryBoolAttribute("Value", &boolVal) == XML_SUCCESS) {
					track->boolValues.emplace(currentElement->Name(), boolVal);
				}
				else if (currentElement->QueryStringAttribute("Value", &stringVal) == XML_SUCCESS && strcmp(currentElement->Name(), "ViewData") == 0) {
					track->viewData = std::string(stringVal);
				}
				else {
					LOG("Unable to parse element %s from track %s, id: %i", currentElement->Name(), parent->Value(), track->id);
				}
			}
			else {
				if (strcmp(currentElement->Name(), "TrackDelay") == 0) {
					int v;
					bool s;
					currentElement->FirstChildElement("Value")->QueryIntAttribute("Value", &v);
					currentElement->FirstChildElement("IsValueSampleBased")->QueryBoolAttribute("Value", &s);
					track->trackDelay = std::make_unique<TrackDelay>(v, s);
				}
				else if (strcmp(currentElement->Name(), "Name") == 0) {
					const char* en;
					const char* un;
					const char* an;
					const char* mn;
					currentElement->FirstChildElement("EffectiveName")->QueryStringAttribute("Value", &en);
					currentElement->FirstChildElement("UserName")->QueryStringAttribute("Value", &un);
					currentElement->FirstChildElement("Annotation")->QueryStringAttribute("Value", &an);
					currentElement->FirstChildElement("MemorizedFirstClipName")->QueryStringAttribute("Value", &mn);
					track->name = std::make_unique<Name>(std::string(en), std::string(un), std::string(an), std::string(mn));
				}
				else if (strcmp(currentElement->Name(), "DeviceChain") == 0) {
					std::vector<std::string> nodeNames = {
						"AutomationLanes", "ClipEnvelopeChooserViewState", "AudioInputRouting", "MidiInputRouting", "AudioOutputRouting", "MidiOutputRouting"
					};
					auto el = currentElement->FirstChildElement();
					while (el != nullptr) {
						auto it = std::find(nodeNames.begin(), nodeNames.end(), std::string(el->Name()));
						auto idx = it - nodeNames.begin();
						switch (idx) {
							case 0: {
								auto automationLanesNode = el->FirstChildElement("AutomationLanes");
								auto automationLane = automationLanesNode->FirstChildElement();
								while (automationLane != nullptr) {
									int id, sd, se, lh;
									bool cs;
									automationLane->QueryIntAttribute("Id", &id);
									automationLane->FirstChildElement("SelectedDevice")->QueryIntAttribute("Value", &sd);
									automationLane->FirstChildElement("SelectedEnvelope")->QueryIntAttribute("Value", &se);
									automationLane->FirstChildElement("IsContentSelected")->QueryBoolAttribute("Value", &cs);
									automationLane->FirstChildElement("LaneHeight")->QueryIntAttribute("Value", &lh);
									track->deviceChain.automationLanes.push_back(std::make_unique<AutomationLane>(id, sd, se, lh, cs));
									automationLane = automationLane->NextSiblingElement();
								}
								bool folded;
								el->FirstChildElement("AreAdditionalAutomationLanesFolded")->QueryBoolAttribute("Value", &folded);
								track->deviceChain.lanesFolded = folded;
								break;
							}
							case 1: {
								int sd, se;
								bool mv;
								el->FirstChildElement("SelectedDevice")->QueryIntAttribute("Value", &sd);
								el->FirstChildElement("SelectedEnvelope")->QueryIntAttribute("Value", &se);
								el->FirstChildElement("PreferModulationVisible")->QueryBoolAttribute("Value", &mv);
								track->deviceChain.clipChooserViewState = std::make_unique<ClipEnvelopeChooserViewState>(sd, se, mv);
								break;
							}
							case 2:
							case 3:
							case 4:
							case 5: {
								Routing::RoutingType type = (Routing::RoutingType)(idx - 2);
								const char* t;
								const char* u;
								const char* l;
								el->FirstChildElement("Target")->QueryStringAttribute("Value", &t);
								el->FirstChildElement("UpperDisplayString")->QueryStringAttribute("Value", &u);
								el->FirstChildElement("LowerDisplayString")->QueryStringAttribute("Value", &l);
								switch (idx) {
								case 2:
									track->deviceChain.audioInputRouting = std::make_unique<Routing>(t, u, l, type);
									break;
								case 3:
									track->deviceChain.midiInputRouting = std::make_unique<Routing>(t, u, l, type);
									break;
								case 4:
									track->deviceChain.audioOutputRouting = std::make_unique<Routing>(t, u, l, type);
									break;
								case 5:
									track->deviceChain.midiOutputRouting = std::make_unique<Routing>(t, u, l, type);
									break;
								}
								break;
							}
						}
						el = el->NextSiblingElement();
					}
				}
				else {
					LOG("Didn't parse node %s in track %s, ID %i, probably because you haven't implemented it.", currentElement->Name(), parent->Value(), track->id);
				}
			}
			currentElement = currentElement->NextSiblingElement();
		}
	}

	void AlsIOHandler::writeToXml() {
		XMLDocument xmlDoc;
		auto decl = xmlDoc.NewDeclaration();
		xmlDoc.InsertEndChild(decl);
		abletonLiveSet->createXmlNode(xmlDoc);
		xmlDoc.SaveFile((outputPath + ".new.xml").c_str());
	}

	void AlsIOHandler::writeToAls() {
		using namespace boost::iostreams;
		std::ifstream inFile(outputPath + ".new.xml", std::ios_base::in);	
		std::ofstream outFile(outputPath + ".new.als", std::ios_base::binary);
		filtering_streambuf<input> inStreamBuf;
		inStreamBuf.push(gzip_compressor());
		inStreamBuf.push(inFile);
		copy(inStreamBuf, outFile);
	}

}	// namespace avc
