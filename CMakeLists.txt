# CMakeList.txt : CMake project for avc, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("avc")

# Add source to this project's executable.
add_executable (avc "src/avc.cpp" "src/avc.h" "src/AbletonLiveSet.h" "src/AbletonLiveSet.cpp" "src/AlsIOHandler.h" "src/AlsIOHandler.cpp" "src/ableton_data_types/AbletonHeader.h"  "src/ableton_data_types/Track.cpp" "src/ableton_data_types/Track.h" "src/ableton_data_types/ViewStates.h" "src/ableton_data_types/SmallNodes.h")

# TODO: Add tests and install targets if needed.
