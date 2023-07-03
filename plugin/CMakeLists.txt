cmake_minimum_required(VERSION 3.24)

project(GTA4.CHS)

file(GLOB sources "*.cpp")

add_library(GTA4.CHS SHARED ${sources} ${common_sources})

set_target_properties(GTA4.CHS PROPERTIES SUFFIX ".asi")
