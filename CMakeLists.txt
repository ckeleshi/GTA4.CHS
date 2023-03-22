cmake_minimum_required(VERSION 3.16.0)

cmake_policy(SET CMP0079 NEW)
cmake_policy(SET CMP0091 NEW) # Enable CMAKE_MSVC_RUNTIME_LIBRARY

set(CMAKE_CXX_STANDARD 20)

set(VCPKG_TARGET_TRIPLET "x86-windows-static")

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
find_package(unofficial-libsquish CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_subdirectory(plugin)
add_subdirectory(gxt_tool)
add_subdirectory(whm_io)

target_precompile_headers(GTA4.CHS PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common/stdinc.h")
target_precompile_headers(gxt_tool PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common/stdinc.h")
target_precompile_headers(whm_io PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common/stdinc.h")

target_link_libraries(GTA4.CHS PRIVATE utf8cpp range-v3 range-v3-meta range-v3::meta range-v3-concepts ZLIB::ZLIB PNG::PNG fmt::fmt-header-only unofficial::libsquish::squish)
target_link_libraries(gxt_tool PRIVATE utf8cpp range-v3 range-v3-meta range-v3::meta range-v3-concepts ZLIB::ZLIB PNG::PNG fmt::fmt-header-only unofficial::libsquish::squish nlohmann_json::nlohmann_json)
target_link_libraries(whm_io PRIVATE utf8cpp range-v3 range-v3-meta range-v3::meta range-v3-concepts ZLIB::ZLIB PNG::PNG fmt::fmt-header-only unofficial::libsquish::squish nlohmann_json::nlohmann_json)
