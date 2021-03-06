cmake_minimum_required(VERSION 3.15.0)

cmake_policy(SET CMP0079 NEW)
cmake_policy(SET CMP0091 NEW) # Enable CMAKE_MSVC_RUNTIME_LIBRARY

set(CMAKE_CXX_STANDARD 20)

# Set vcpkg toolchain file in global settings.json
set(VCPKG_TARGET_TRIPLET "x86-windows-static")

set(CMAKE_GENERATOR_PLATFORM "Win32")

project(gta4_chs)

add_definitions(-DNOMINMAX -D_CRT_SECURE_NO_WARNINGS)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include("VC-LTL helper for cmake.cmake")

file(GLOB common_sources "common/*.cpp")

find_package(utf8cpp CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_subdirectory(plugin)
add_subdirectory(gxt_tool)
add_subdirectory(whm_io)

target_link_libraries(GTA4.CHS PRIVATE PNG::PNG fmt::fmt-header-only utf8cpp range-v3 range-v3-meta range-v3::meta range-v3-concepts)
target_link_libraries(gxt_tool PRIVATE fmt::fmt-header-only utf8cpp range-v3 range-v3-meta range-v3::meta range-v3-concepts)
target_link_libraries(whm_io PRIVATE fmt::fmt-header-only utf8cpp range-v3 range-v3-meta range-v3::meta range-v3-concepts ZLIB::ZLIB)
