# CMakeList.txt : CMake project for avc, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("avc")

# Boost
set(BOOST_INCLUDEDIR "C:/local/boost_1_76_0")
set(BOOST_LIBRARYDIR "C:/local/boost_1_76_0/lib64-msvc-14.2")
find_package(Boost REQUIRED COMPONENTS iostreams)

# Add source to this project's executable.
add_executable (avc "src/Main.cpp" "src/avc.h" "src/AbletonLiveSet.h"  "src/AlsIOHandler.h" 
	"src/AlsIOHandler.cpp"    "src/ableton_data_types/Track.h"  
	"src/ableton_data_types/SmallNodes.h"   "src/ableton_data_types/Mixer.h"
	  "deps/tinyxml2/tinyxml2.h" "deps/tinyxml2/tinyxml2.cpp"  "src/ableton_data_types/DeviceChain.h")

target_include_directories(avc PUBLIC ${BOOST_INCLUDEDIR})
target_link_libraries(avc PUBLIC ${BOOST_LIBRARYDIR}/libboost_iostreams-vc142-mt-gd-x64-1_76.lib ${BOOST_LIBRARYDIR}/libboost_zlib-vc142-mt-gd-x64-1_76.lib)
