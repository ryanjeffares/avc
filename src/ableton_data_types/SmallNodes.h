#pragma once
#include "../avc.h"

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

			void createXmlNode() const {

			}
		};

	}	// namespace ableton_data_types
}	// namespace avc