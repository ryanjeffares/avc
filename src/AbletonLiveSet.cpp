#include "AbletonLiveSet.h"

namespace avc {

	AbletonLiveSet::AbletonLiveSet(std::string n, std::string v, std::string e, std::shared_ptr<ableton_data_types::AbletonHeader> ah) 
		: name(n), xmlVersion(v), xmlEncoding(e), abletonHeader(ah) {

	}

	AbletonLiveSet::~AbletonLiveSet() {}

}	// namespace avc