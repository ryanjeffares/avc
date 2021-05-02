#pragma once
#include "Mixer.h"

namespace avc {
	namespace ableton_data_types {

		using namespace tinyxml2;

		class DeviceChain
		{
		public:
			
			bool lanesFolded;
			std::vector<std::unique_ptr<AutomationLane>> automationLanes;
			std::unique_ptr<ClipEnvelopeChooserViewState> clipChooserViewState;
			std::unique_ptr<Routing> audioInputRouting, audioOutputRouting, midiInputRouting, midiOutputRouting;
			Mixer mixer;

			void createXmlNode(XMLDocument& doc, XMLNode* parent, bool isMaster) const {
				auto node = doc.NewElement("DeviceChain");
				auto automationLanesEl = doc.NewElement("AutomationLanes");
				auto automationLanesElInner = automationLanesEl->InsertNewChildElement("AutomationLanes");
				for (auto& al : automationLanes) {
					al->createXmlNode(doc, automationLanesElInner);
				}
				auto foldedEl = automationLanesEl->InsertNewChildElement("AreAdditionalAutomationLanesFolded");
				foldedEl->SetAttribute("Value", lanesFolded);
				node->InsertEndChild(automationLanesEl);

				clipChooserViewState->createXmlNode(doc, node);
				audioInputRouting->createXmlNode(doc, node);
				midiInputRouting->createXmlNode(doc, node);
				audioOutputRouting->createXmlNode(doc, node);
				midiOutputRouting->createXmlNode(doc, node);
				mixer.createXmlNode(doc, node, isMaster);

				parent->InsertEndChild(node);
			}

		private:


		};

	}	// namespace ableton_data_types
}	// namespace avc