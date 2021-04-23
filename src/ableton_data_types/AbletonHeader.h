#pragma once
#include "../avc.h"

namespace avc {
	namespace ableton_data_types {

		class AbletonHeader
		{
		public:
			AbletonHeader(int majVer, std::string minVer, int schem, std::string cr, std::string rev)
				: majorVersion(majVer), minorVersion(minVer), schemaChangeCount(schem), creator(cr), revision(rev) {}			

			const int majorVersion, schemaChangeCount;
			const std::string minorVersion, creator, revision;
		};

	}	// namespace ableton_data_types
}	// namespace avc