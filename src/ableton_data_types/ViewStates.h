#pragma once
#include "../avc.h"

namespace avc {
	namespace ableton_data_types {

		using namespace tinyxml2;

		class ViewStates
		{
		public:
			
			std::map<std::string, int> elements;

			void createXmlNode(XMLDocument& doc, XMLNode* parent) const {
				auto node = doc.NewElement("ViewStates");
				for (auto& val : elements) {
					auto el = node->InsertNewChildElement(val.first.c_str());
					el->SetAttribute("Value", val.second);
				}
				parent->InsertEndChild(node);
			}

		};

	}	// namespace ableton_data_types
}	// namespace avc