#pragma once
#include <utility>

namespace avc {
	namespace ableton_data_types {

		struct SmallNode 
		{
			virtual ~SmallNode() {}
			virtual void createXmlNode() const = 0;
		};

		struct ContentSplitterProperties : public virtual SmallNode
		{
			const bool open;
			const double size;

			ContentSplitterProperties(bool o, double s)
				: open(o), size(s) {}

			void createXmlNode() const override {

			}
		};

		struct SequencerNavigator : public virtual SmallNode
		{
			const double currentZoom;
			const std::pair<int, int> scrollerPos;
			const std::pair<int, int> clientSize;

			SequencerNavigator(double z, int spx, int spy, int csx, int csy)
				: currentZoom(z), scrollerPos(spx, spy), clientSize(spx, spy) {}

			void createXmlNode() const override {

			}
		};

		struct TimeSelection : public virtual SmallNode
		{
			const int anchorTime;
			const int otherTime;

			TimeSelection(int a, int o)
				: anchorTime(a), otherTime(o) {}

			void createXmlNode() const override {

			}
		};

		struct ScaleInformation : public virtual SmallNode
		{
			const int rootNote;
			const std::string name;

			ScaleInformation(int r, std::string n)
				: rootNote(r), name(n) {}

			void createXmlNode() const override {

			}
		};

		struct Grid : public virtual SmallNode
		{
			const int fixedNumerator, fixedDenominator, gridIntervalPixel, nToles;
			const bool snapToGrid, fixed;

			Grid(int fn, int fd, int gip, int nt, bool s, bool f)
				: fixedNumerator(fn), fixedDenominator(fd), gridIntervalPixel(gip), nToles(nt), snapToGrid(s), fixed(f) {}

			void createXmlNode() const override {

			}
		};

	}	// namespace ableton_data_types
}	// namespace avc