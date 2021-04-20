#include "AlsIOHandler.h"

namespace avc {

	using namespace tinyxml2;

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
		outputXml.open(outputPath + ".copy.xml");
		copy(input, outputXml);
		outputXml.close();
		std::cout << "Wrote .als data to " << outputPath << std::endl;
	}

	void AlsIOHandler::storeXmlData() {			
		XMLDocument xmlDoc;
		auto loaded = xmlDoc.LoadFile(outputPath.c_str());
		if (loaded == XML_SUCCESS) {
			std::cout << "Successfully loaded XML file." << std::endl;
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
		}
		else {
			std::cout << "Problem loading XML File: " << loaded << std::endl;
		}		
	}

	void AlsIOHandler::getValues(XMLNode* node) {
		auto& intValues = abletonLiveSet->intValues;
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
			"Grid"
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
					intValues.emplace(currentElement->Value(), intVal);
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
					std::cout << "Problem parsing element " << currentElement->Name() << std::endl;
				}						
			}
			else {
				try {
					auto it = std::find(nodeNames.begin(), nodeNames.end(), std::string(currentElement->Name()));
					int idx = it - nodeNames.begin();
					switch (idx) {
						case 0:
							currentElement->FirstChildElement()->QueryIntAttribute("Value", &(abletonLiveSet->autoColourPickerPlayerTracks));
							break;
						case 1:
							currentElement->FirstChildElement()->QueryIntAttribute("Value", &(abletonLiveSet->autoColourPickerMasterTracks));
							break;
						case 2:
							bool open;
							currentElement->FirstChildElement("Open")->QueryBoolAttribute("Value", &open);
							int size;
							currentElement->FirstChildElement("Size")->QueryIntAttribute("Value", &size);
							abletonLiveSet->contentSplitterProperties = std::make_shared<ableton_data_types::ContentSplitterProperties>(open, size);
							break;
						case 3:
							double zoom;
							int spx, spy, csx, csy;
							currentElement->FirstChildElement("BeatTimeHelper")->FirstChildElement("CurrentZoom")->QueryDoubleAttribute("Value", &zoom);
							currentElement->FirstChildElement("ScrollerPos")->QueryIntAttribute("X", &spx);
							currentElement->FirstChildElement("ScrollerPos")->QueryIntAttribute("Y", &spy);
							currentElement->FirstChildElement("ClientSize")->QueryIntAttribute("X", &csx);
							currentElement->FirstChildElement("ClientSize")->QueryIntAttribute("Y", &csy);
							abletonLiveSet->sequencerNavigator = std::make_shared<ableton_data_types::SequencerNavigator>(zoom, spx, spy, csx, csy);
							break;
						case 4:
							int at, ot;
							currentElement->FirstChildElement("AnchorTime")->QueryIntAttribute("Value", &at);
							currentElement->FirstChildElement("OtherTime")->QueryIntAttribute("Value", &ot);
							abletonLiveSet->timeSelection = std::make_shared<ableton_data_types::TimeSelection>(at, ot);
							break;
						case 5:
							int r;
							const char* n;
							currentElement->FirstChildElement("RootNote")->QueryIntAttribute("Value", &r);
							currentElement->FirstChildElement("Name")->QueryStringAttribute("Value", &n);
							abletonLiveSet->scaleInformation = std::make_shared<ableton_data_types::ScaleInformation>(r, std::string(n));
							break;
						case 6:
							int fn, fd, gip, nt;
							bool s, f;
							currentElement->FirstChildElement("FixedNumerator")->QueryIntAttribute("Value", &fn);
							currentElement->FirstChildElement("FixedDenominator")->QueryIntAttribute("Value", &fd);
							currentElement->FirstChildElement("GridIntervalPixel")->QueryIntAttribute("Value", &gip);
							currentElement->FirstChildElement("Ntoles")->QueryIntAttribute("Value", &nt);
							currentElement->FirstChildElement("SnapToGrid")->QueryBoolAttribute("Value", &s);
							currentElement->FirstChildElement("Fixed")->QueryBoolAttribute("Value", &f);
							abletonLiveSet->grid = std::make_shared<ableton_data_types::Grid>(fn, fd, gip, nt, s, f);
							break;
					}
				}
				catch (XMLError& e) {
					std::cout << "Error parsing element " << currentElement->Name() << ", returned " << e << std::endl;
				}
			}
			currentElement = currentElement->NextSiblingElement();
		}
	}

	void AlsIOHandler::writeToXml() {

	}
}	// namespace avc
