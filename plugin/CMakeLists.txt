cmake_minimum_required(VERSION 3.24)

project(plugin)

find_package(utf8cpp CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include("VC-LTL helper for cmake.cmake")

add_library(plugin SHARED
    ${CMAKE_SOURCE_DIR}/common/common.cpp
    ${CMAKE_SOURCE_DIR}/common/fnv_hash.cpp
    ${CMAKE_SOURCE_DIR}/common/module_path.cpp
    batch_matching.cpp
    batch_matching.h
    byte_pattern.cpp
    byte_pattern.h
    char_table.cpp
    char_table.h
    class_pointer.hpp
    dllmain.cpp
    font.cpp
    font.h
    game.cpp
    game.h
    gta_benchmark.cpp
    gta_benchmark.h
    gta_font.cpp
    gta_font.h
    gta_game.cpp
    gta_game.h
    gta_html.cpp
    gta_html.h
    gta_mail_reply.cpp
    gta_mail_reply.h
    gta_menu.cpp
    gta_menu.h
    gta_phone.cpp
    gta_phone.h
    gta_save.cpp
    gta_save.h
    gta_string.cpp
    gta_string.h
    gta_whm.cpp
    gta_whm.h
    plugin.cpp
    plugin.h
    whm_table.cpp
    whm_table.h
)

target_compile_definitions(plugin PRIVATE NOMINMAX _CRT_SECURE_NO_WARNINGS)

target_precompile_headers(plugin PRIVATE "${CMAKE_SOURCE_DIR}/common/stdinc.h")

target_link_libraries(plugin PRIVATE utf8::cpp utf8cpp::utf8cpp spdlog::spdlog_header_only)

set_target_properties(plugin PROPERTIES OUTPUT_NAME "GTA4.CHS")
set_target_properties(plugin PROPERTIES SUFFIX ".asi")
