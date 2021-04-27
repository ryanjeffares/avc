#pragma once
#include "../avc.h"

namespace avc {
	namespace ableton_data_types {

		using namespace tinyxml2;

		struct SmallNode 
		{
			virtual ~SmallNode() {}
			virtual void createXmlNode(XMLDocument& doc, XMLNode* parent) const = 0;
		};

		struct AbletonHeader : public virtual SmallNode
		{
			const int majorVersion, schemaChangeCount;
			const std::string minorVersion, creator, revision;

			AbletonHeader(int majVer, std::string minVer, int schem, std::string cr, std::string rev)
				: majorVersion(majVer), minorVersion(minVer), schemaChangeCount(schem), creator(cr), revision(rev) {}

			void createXmlNode(XMLDocument& doc, XMLNode* parent) const override {}
		};

		struct ContentSplitterProperties : public virtual SmallNode
		{
			const bool open;
			const double size;

			ContentSplitterProperties(bool o, double s)
				: open(o), size(s) {}

			void createXmlNode(XMLDocument& doc, XMLNode* parent) const override {
				auto node = doc.NewElement("ContentSplitterProperties");
				auto openEl = node->InsertNewChildElement("Open");
				openEl->SetAttribute("Value", open);
				auto sizeEl = node->InsertNewChildElement("Size");
				sizeEl->SetAttribute("Value", size);
				parent->InsertEndChild(node);
			}
		};

		struct SequencerNavigator : public virtual SmallNode
		{
			const double currentZoom;
			const std::pair<int, int> scrollerPos;
			const std::pair<int, int> clientSize;

			SequencerNavigator(double z, int spx, int spy, int csx, int csy)
				: currentZoom(z), scrollerPos(spx, spy), clientSize(spx, spy) {}

			void createXmlNode(XMLDocument& doc, XMLNode* parent) const override {
				auto node = doc.NewElement("SequencerNavigator");
				auto beatNode = doc.NewElement("BeatTimeHelper");
				auto zoomEl =beatNode->InsertNewChildElement("CurrentZoom");
				zoomEl->SetAttribute("Value", currentZoom);
				node->InsertEndChild(beatNode);
				auto scrollEl = node->InsertNewChildElement("ScrollerPos");
				scrollEl->SetAttribute("X", scrollerPos.first);
				scrollEl->SetAttribute("Y", scrollerPos.second);
				auto clientEl = node->InsertNewChildElement("ClientSize");
				clientEl->SetAttribute("X", clientSize.first);
				clientEl->SetAttribute("Y", clientSize.second);
				parent->InsertEndChild(node);
			}
		};

		struct TimeSelection : public virtual SmallNode
		{
			const int anchorTime;
			const int otherTime;

			TimeSelection(int a, int o)
				: anchorTime(a), otherTime(o) {}

			void createXmlNode(XMLDocument& doc, XMLNode* parent) const override {
				auto node = doc.NewElement("TimeSelection");
				auto anchorEl = node->InsertNewChildElement("AnchorTime");
				anchorEl->SetAttribute("Value", anchorTime);
				auto otherEl = node->InsertNewChildElement("OtherTime");
				otherEl->SetAttribute("Value", otherTime);
				parent->InsertEndChild(node);
			}
		};

		struct ScaleInformation : public virtual SmallNode
		{
			const int rootNote;
			const std::string name;

			ScaleInformation(int r, std::string n)
				: rootNote(r), name(n) {}

			void createXmlNode(XMLDocument& doc, XMLNode* parent) const override {
				auto node = doc.NewElement("ScaleInformation");
				auto rootEl = node->InsertNewChildElement("RootNote");
				rootEl->SetAttribute("Value", rootNote);
				auto nameEl = node->InsertNewChildElement("Name");
				nameEl->SetAttribute("Value", name.c_str());
				parent->InsertEndChild(node);
			}
		};

		struct Grid : public virtual SmallNode
		{
			const int fixedNumerator, fixedDenominator, gridIntervalPixel, nToles;
			const bool snapToGrid, fixed;

			Grid(int fn, int fd, int gip, int nt, bool s, bool f)
				: fixedNumerator(fn), fixedDenominator(fd), gridIntervalPixel(gip), nToles(nt), snapToGrid(s), fixed(f) {}

			void createXmlNode(XMLDocument& doc, XMLNode* parent) const override {
				auto node = doc.NewElement("Grid");
				auto fnEl = node->InsertNewChildElement("FixedNumerator");
				fnEl->SetAttribute("Value", fixedNumerator);
				auto fdEl = node->InsertNewChildElement("FixedDenominator");
				fdEl->SetAttribute("Value", fixedDenominator);
				auto gipEl = node->InsertNewChildElement("GridIntervalPixel");
				gipEl->SetAttribute("Value", gridIntervalPixel);
				auto ntEl = node->InsertNewChildElement("Ntoles");
				ntEl->SetAttribute("Value", nToles);
				auto snapEl = node->InsertNewChildElement("SnapToGrid");
				snapEl->SetAttribute("Value", snapToGrid);
				auto fixedEl = node->InsertNewChildElement("Fixed");
				fixedEl->SetAttribute("Value", fixed);
				parent->InsertEndChild(node);
			}
		};

		struct Transport : public virtual SmallNode
		{
			const double phaseNudgeTempo, loopStart, loopLength, currentTime;
			const int metronomeTickDuration;
			const bool loopOn, loopIsStart, punchIn, punchOut, drawMode;

			Transport(double p, double ls, double ll, double ct, int m, bool lo, bool lis, bool pi, bool po, bool dm)
				: phaseNudgeTempo(p), loopStart(ls), loopLength(ll), currentTime(ct), metronomeTickDuration(m), 
				loopOn(lo), loopIsStart(lis), punchIn(pi), punchOut(po), drawMode(dm) {}

			void createXmlNode(XMLDocument& doc, XMLNode* parent) const override {
				auto node = doc.NewElement("Transport");
				auto phaseEl = node->InsertNewChildElement("PhaseNudgeTempo");
				phaseEl->SetAttribute("Value", phaseNudgeTempo);
				auto loopOnEl = node->InsertNewChildElement("LoopOn");
				loopOnEl->SetAttribute("Value", loopOn);
				auto loopStartEl = node->InsertNewChildElement("LoopStart");
				loopStartEl->SetAttribute("Value", loopStart);
				auto loopLengthEl = node->InsertNewChildElement("LoopLength");
				loopLengthEl->SetAttribute("Value", loopLength);
				auto loopIsStartEl = node->InsertNewChildElement("LoopIsSongStart");
				loopIsStartEl->SetAttribute("Value", loopIsStart);
				auto timeEl = node->InsertNewChildElement("CurrentTime");
				timeEl->SetAttribute("Value", currentTime);
				auto punchInEl = node->InsertNewChildElement("PunchIn");
				punchInEl->SetAttribute("Value", punchIn);
				auto punchOutEl = node->InsertNewChildElement("PunchOut");
				punchOutEl->SetAttribute("Value", punchOut);
				auto metrEl = node->InsertNewChildElement("MetronomeClickDuration");
				metrEl->SetAttribute("Value", metronomeTickDuration);
				auto drawEl = node->InsertNewChildElement("DrawMode");
				drawEl->SetAttribute("Value", drawMode);
				parent->InsertEndChild(node);
			}
		};

		struct TrackDelay : public virtual SmallNode
		{
			const int value;
			const bool isValueSampleBased;

			TrackDelay(int v, bool s)
				: value(v), isValueSampleBased(s) {}

			void createXmlNode(XMLDocument& doc, XMLNode* parent) const override {
				auto node = doc.NewElement("TrackDelay");
				auto vEl = node->InsertNewChildElement("Value");
				vEl->SetAttribute("Value", value);
				auto sEl = node->InsertNewChildElement("IsValueSampleBased");
				sEl->SetAttribute("Value", isValueSampleBased);
				parent->InsertEndChild(node);
			}
		};

		struct Name : public virtual SmallNode
		{
			const std::string effectiveName, userName, annotation, memorizedFirstClip;

			Name(std::string en, std::string un, std::string a, std::string m)
				: effectiveName(en), userName(un), annotation(a), memorizedFirstClip(m) {}

			void createXmlNode(XMLDocument& doc, XMLNode* parent) const override {
				auto node = doc.NewElement("Name");
				auto enEl = node->InsertNewChildElement("EffectiveName");
				enEl->SetAttribute("Value", effectiveName.c_str());
				auto unEl = node->InsertNewChildElement("UserName");
				unEl->SetAttribute("Value", userName.c_str());
				auto anEl = node->InsertNewChildElement("Annotation");
				anEl->SetAttribute("Value", annotation.c_str());
				auto mnEl = node->InsertNewChildElement("MemorizedFirstClip");
				mnEl->SetAttribute("Value", memorizedFirstClip.c_str());
				parent->InsertEndChild(node);
			}
		};

		struct SendsPre : public virtual SmallNode
		{
			const int id;
			const bool value;

			SendsPre(int i, bool v)
				: id(i), value(v) {}

			void createXmlNode(XMLDocument& doc, XMLNode* parent) const override {
				auto el = doc.NewElement("SendsPreBool");
				el->SetAttribute("Id", id);
				el->SetAttribute("Value", value);
				parent->InsertEndChild(el);
			}
		};

		struct ViewStates : public virtual SmallNode
		{
			std::map<std::string, int> elements;

			void createXmlNode(XMLDocument& doc, XMLNode* parent) const override {
				auto node = doc.NewElement("ViewStates");
				for (auto& val : elements) {
					auto el = node->InsertNewChildElement(val.first.c_str());
					el->SetAttribute("Value", val.second);
				}
				parent->InsertEndChild(node);
			}
		};

		struct ClipEnvelopeChooserViewState : public virtual SmallNode
		{
			const int selectedDevice, selectedEnvelope;
			const bool modulationVisible;

			ClipEnvelopeChooserViewState(int sd, int se, bool m)
				: selectedDevice(sd), selectedEnvelope(se), modulationVisible(m) {}

			void createXmlNode(XMLDocument& doc, XMLNode* parent) const override {
				auto node = doc.NewElement("ClipEnvelopeChooserViewState");
				auto deviceEl = node->InsertNewChildElement("SelectedDevice");
				deviceEl->SetAttribute("Value", selectedDevice);
				auto envelopeEl = node->InsertNewChildElement("SelectedEnvelope");
				envelopeEl->SetAttribute("Value", selectedEnvelope);
				auto modEl = node->InsertNewChildElement("PreferModulationVisible");
				modEl->SetAttribute("Value", modulationVisible);
				parent->InsertEndChild(node);				
			}
		};

		struct Routing : public virtual SmallNode
		{
			enum RoutingType
			{
				AUDIO_INPUT = 0, MIDI_INPUT = 1, AUDIO_OUTPUT = 2, MIDI_OUTPUT = 3
			};

			const std::string target, upperDisplay, lowerDisplay;
			const RoutingType type;

			Routing(std::string t, std::string u, std::string l, RoutingType rt)
				: target(t), upperDisplay(u), lowerDisplay(l), type(rt) {}

			void createXmlNode(XMLDocument& doc, XMLNode* parent) const override {
				const char* name;
				switch (type) {
				case AUDIO_INPUT:
					name = "AudioInputRouting";
					break;
				case AUDIO_OUTPUT:
					name = "AudioOutputRouting";
					break;
				case MIDI_INPUT:
					name = "MidiInputRoutine";
					break;
				case MIDI_OUTPUT:
					name = "MidiOutputRouting";
					break;
				default:
					if (_DEBUG) {
						DBG("No RoutingType initialised, returning...");
					}					
					return;
				}
				auto node = doc.NewElement(name);
				auto targetEl = node->InsertNewChildElement("Target");
				targetEl->SetAttribute("Value", target.c_str());
				auto upperEl = node->InsertNewChildElement("UpperDisplayString");
				upperEl->SetAttribute("Value", upperDisplay.c_str());
				auto lowerEl = node->InsertNewChildElement("LowerDisplayString");
				lowerEl->SetAttribute("Value", lowerDisplay.c_str());
				parent->InsertEndChild(node);
			}
		};

		struct AutomationLane : public virtual SmallNode
		{
			const int id, selectedDevice, selectedEnvelope, laneHeight;
			const bool contentSelected;

			AutomationLane(int i, int sd, int se, int lh, bool cs)
				: id(i), selectedDevice(sd), selectedEnvelope(se), laneHeight(lh), contentSelected(cs) {}

			// the xml structure for these is 
			// <AutomationLanes>
			//		<AutomationLanes>
			//			<AutomationLane />
			//		</AutomationLanes>
			//		<AreAdditionalAutomationLanesFolded />
			// <AutomationLanes>
			// so in the Track class we're gonna want to create all the outer nodes, so this function will be passed the inner-most AutomationLanes node
			void createXmlNode(XMLDocument& doc, XMLNode* parent) const override {
				auto node = doc.NewElement("AutomationLane");
				node->SetAttribute("Id", id);
				auto deviceEl = node->InsertNewChildElement("SelectedDevice");
				deviceEl->SetAttribute("Value", selectedDevice);
				auto envelopeEl = node->InsertNewChildElement("SelectedEnvelope");
				envelopeEl->SetAttribute("Value", selectedEnvelope);
				auto selectedEl = node->InsertNewChildElement("IsContentSelected");
				selectedEl->SetAttribute("Value", contentSelected);
				auto laneEl = node->InsertNewChildElement("LaneHeight");
				laneEl->SetAttribute("Value", laneHeight);
				parent->InsertEndChild(node);
			}
		};

	}	// namespace ableton_data_types
}	// namespace avc