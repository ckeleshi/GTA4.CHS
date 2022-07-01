#pragma once

#define VALIDATE_SIZE(expr, size) static_assert(sizeof(expr) == (size), "Wrong size.")

#include <Windows.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <d3d9.h>
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
#include <fmt/printf.h>
#include <png.h>
#include <range/v3/all.hpp>
#include <squish.h>
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
