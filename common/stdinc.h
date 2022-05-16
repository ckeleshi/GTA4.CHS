#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX

#pragma warning(disable : 4244)

#define VALIDATE_SIZE(expr, size) static_assert(sizeof(expr) == (size), "Wrong size.")

#include <Windows.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iterator>
#include <map>
#include <regex>
#include <set>
#include <span>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "BinaryFile.hpp"
#include "MemoryFile.hpp"
#include "injector/assembly.hpp"
#include "injector/calling.hpp"
#include "injector/hooking.hpp"
#include "injector/injector.hpp"

// vcpkg
#include <range/v3/all.hpp>
#include <utf8cpp/utf8.h>
#include <zlib.h>

typedef unsigned char uint8;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned short uint16;
typedef unsigned int uint;
typedef unsigned int uint32;
typedef unsigned long long ulonglong;
typedef unsigned long long uint64;
