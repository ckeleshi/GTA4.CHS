#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX

#pragma warning(disable:4244)

#define VALIDATE_SIZE(expr, size) static_assert(sizeof(expr) == (size), "Wrong size.")

#include <Windows.h>
#include <d3d9.h>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <vector>
#include <filesystem>
#include <regex>
#include <functional>
#include <array>
#include <utility>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iterator>
#include <algorithm>
#include <chrono>
#include <span>

#include "injector/hooking.hpp"
#include "injector/calling.hpp"
#include "injector/injector.hpp"
#include "injector/assembly.hpp"
#include "BinaryFile.hpp"
#include "MemoryFile.hpp"

//vcpkg
#include <range/v3/all.hpp>
#include <utf8cpp/utf8.h>
#include <zlib.h>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ulonglong;
typedef unsigned long long uint64;
