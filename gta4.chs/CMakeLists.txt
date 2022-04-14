cmake_minimum_required(VERSION 3.15.0)

project(gta4.chs)

file(GLOB sources "*.cpp")

add_library(gta4.chs SHARED ${sources} ${common_sources})

set_target_properties(gta4.chs PROPERTIES SUFFIX ".asi")
