// avc.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <memory>
#include <utility>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include "../deps/tinyxml2/tinyxml2.h"

#ifndef AVC
#define AVC
#endif

#define DBG(fmt, ...) \
            do { if (_DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt "\n", __FILE__, \
                __LINE__, __func__, __VA_ARGS__); } while (0)

#define LOG(fmt, ...) \
            do { fprintf(stderr, fmt "\n", __VA_ARGS__); } while (0)

// TODO: Locators, GroovePool, DetailClipKeyMidis
