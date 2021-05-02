// avc.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#ifndef AVC
#define AVC

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <memory>
#include <utility>
#include "../deps/tinyxml2/tinyxml2.h"

#ifdef _DEBUG
#define AVC_DEBUG 1
#else
#define AVC_DEBUG 0
#endif

#define DBG(fmt, ...) \
            do { if (AVC_DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt "\n", __FILE__, \
                __LINE__, __func__, __VA_ARGS__); } while (0)

#define LOG(fmt, ...) \
            do { fprintf(stderr, fmt "\n", __VA_ARGS__); } while (0)

#endif  // #ifndef AVC
// TODO: Find out what Locators, GroovePool, DetailClipKeyMidis are in main element list
//       Find out what LastPresetRef and SourceContext is in Track->DeviceChain->Mixer
//       Figure out differences in Mixer Nodes between MIDI, Audio, Master, Send, Pre Hear tracks AAAAAAAAA
