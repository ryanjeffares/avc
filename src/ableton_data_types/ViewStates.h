#pragma once
#include "../avc.h"
#include <map>

namespace avc {
	namespace ableton_data_types {

		class ViewStates
		{
		public:
			
			std::map<std::string, int> elements;

			void createXmlNode();

		};

	}	// namespace ableton_data_types
}	// namespace avc