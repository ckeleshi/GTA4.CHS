cmake_minimum_required(VERSION 3.24)

project(gta4_chs)

find_package(utf8cpp CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

add_subdirectory(plugin)
add_subdirectory(whm_io)

