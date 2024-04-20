cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 20)

project(gta4_chs)

add_definitions(-DNOMINMAX -D_CRT_SECURE_NO_WARNINGS)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include("VC-LTL helper for cmake.cmake")

file(GLOB common_sources "common/*.cpp")

find_package(utf8cpp CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

add_subdirectory(plugin)
add_subdirectory(whm_io)

target_precompile_headers(GTA4.CHS PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common/stdinc.h")
target_precompile_headers(whm_io PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common/stdinc.h")

target_link_libraries(GTA4.CHS PRIVATE utf8::cpp range-v3 range-v3-meta range-v3::meta range-v3-concepts fmt::fmt-header-only spdlog::spdlog_header_only)
target_link_libraries(whm_io PRIVATE range-v3 range-v3-meta range-v3::meta range-v3-concepts ZLIB::ZLIB fmt::fmt-header-only rapidjson)
