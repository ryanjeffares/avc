#pragma once
#include "../avc.h"
#include "SmallNodes.h"

namespace avc {
	namespace ableton_data_types {

		using namespace tinyxml2;

		class Mixer
		{
		public:

			Mixer() : pan("Pan"), splitStereoL("SplitStereoPanL"), splitStereoR("SplitStereoPanR") {}

			struct SmallMixerNode : public virtual SmallNode
			{
				std::string name;
				int lomId, automationTarget, lockEnvelope;
				bool manualBool;
				int manualInt;

				void createXmlNode(XMLDocument& doc, XMLNode* parent) const override {
					auto node = doc.NewElement(name.c_str());
					auto lomEl = node->InsertNewChildElement("LomId");
					lomEl->SetAttribute("Value", lomId);
					auto manualEl = node->InsertNewChildElement("Manual");
					if (name == "On" || name == "Speaker") {
						manualEl->SetAttribute("Value", manualBool);
					}
					else {
						manualEl->SetAttribute("Value", manualInt);
					}					
					auto autoEl = node->InsertNewChildElement("AutomationTarget");
					autoEl->SetAttribute("Id", automationTarget);
					auto lockEl = autoEl->InsertNewChildElement("LockEnvelope");
					lockEl->SetAttribute("Value", lockEnvelope);
					createAdditionalXmlElements(node);
					parent->InsertEndChild(node);
				}

			protected:
				virtual void createAdditionalXmlElements(XMLElement* parent) const {}
			};

			struct On : public virtual SmallMixerNode
			{
				int midiCcMin, midiCcMax;

				On() {
					name = "On";
				}

			protected:
				void createAdditionalXmlElements(XMLElement* parent) const override {
					auto midiEl = parent->InsertNewChildElement("MidiCCOnOffThresholds");
					auto minEl = midiEl->InsertNewChildElement("Min");
					minEl->SetAttribute("Value", midiCcMin);
					auto maxEl = midiEl->InsertNewChildElement("Max");
					maxEl->SetAttribute("Value", midiCcMax);
				}
			};

			struct Speaker : public virtual SmallMixerNode
			{
				int midiCcMin, midiCcMax;

				Speaker() {
					name = "Speaker";
				}

			protected:
				void createAdditionalXmlElements(XMLElement* parent) const override {
					auto midiEl = parent->InsertNewChildElement("MidiCCOnOffThresholds");
					auto minEl = midiEl->InsertNewChildElement("Min");
					minEl->SetAttribute("Value", midiCcMin);
					auto maxEl = midiEl->InsertNewChildElement("Max");
					maxEl->SetAttribute("Value", midiCcMax);
				}
			};

			struct Pan : public virtual SmallMixerNode
			{
				int midiMin, midiMax, modulationTarget, modLockEnvelope;

				Pan(std::string n) {
					name = n;
				}

			protected:
				void createAdditionalXmlElements(XMLElement* parent) const override {
					auto midiEl = parent->InsertNewChildElement("MidiControllerRange");
					auto minEl = midiEl->InsertNewChildElement("Min");
					minEl->SetAttribute("Value", midiMin);
					auto maxEl = midiEl->InsertNewChildElement("Max");
					maxEl->SetAttribute("Value", midiMax);
					auto modTargetEl = parent->InsertNewChildElement("ModulationTarget");
					modTargetEl->SetAttribute("Id", modulationTarget);
					auto modLockEl = modTargetEl->InsertNewChildElement("LockEnvelope");
					modLockEl->SetAttribute("Value", modLockEnvelope);
				}
			};			

			struct Volume : public virtual SmallMixerNode
			{
				double midiMin, midiMax;
				int modTarget, modLockEnv;

				Volume() {
					name = "Volume";
				}

			protected:
				void createAdditionalXmlElements(XMLElement* parent) const override {
					auto midiEl = parent->InsertNewChildElement("MidiControllerRange");
					auto minEl = midiEl->InsertNewChildElement("Min");
					minEl->SetAttribute("Value", midiMin);
					auto maxEl = midiEl->InsertNewChildElement("Max");
					maxEl->SetAttribute("Value", midiMax);
					auto modTargetEl = parent->InsertNewChildElement("ModulationTarget");
					modTargetEl->SetAttribute("Id", modTarget);
					auto modLockEl = modTargetEl->InsertNewChildElement("LockEnvelope");
					modLockEl->SetAttribute("Value", modLockEnv);
				}
			};

			std::map<std::string, int> intValues, intValuesLomId;
			std::map<std::string, std::string> stringValues;
			std::map<std::string, bool> boolValues;
			On on;
			Speaker speaker;
			Pan pan, splitStereoL, splitStereoR;
			Volume volume;

			void createXmlNode(XMLDocument& doc, XMLNode* parent) const {
				auto node = doc.NewElement("Mixer");
				for (auto& i : intValues) {
					auto el = node->InsertNewChildElement(i.first.c_str());
					el->SetAttribute("Value", i.second);
				}
				for (auto& il : intValuesLomId) {
					auto el = node->InsertNewChildElement(il.first.c_str());
					el->SetAttribute("LomId", il.second);
				}
				for (auto& b : boolValues) {
					auto el = node->InsertNewChildElement(b.first.c_str());
					el->SetAttribute("Value", b.second);
				}
				for (auto& s : stringValues) {
					auto el = node->InsertNewChildElement(s.first.c_str());
					el->SetAttribute("Value", s.second.c_str());
				}
				on.createXmlNode(doc, node);
				speaker.createXmlNode(doc, node);
				pan.createXmlNode(doc, node);
				splitStereoL.createXmlNode(doc, node);
				splitStereoR.createXmlNode(doc, node);
				volume.createXmlNode(doc, node);						
				parent->InsertEndChild(node);
			}

		};	// class Mixer

	}	// namespace ableton_data_types
}	// namespace avc