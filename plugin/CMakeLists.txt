cmake_minimum_required(VERSION 3.24)

project(plugin)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(utf8cpp CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include("VC-LTL helper for cmake.cmake")

add_library(plugin SHARED
    ${CMAKE_SOURCE_DIR}/common/common.cpp
    ${CMAKE_SOURCE_DIR}/common/fnv_hash.cpp
    ${CMAKE_SOURCE_DIR}/common/module_path.cpp
    batch_matching.cpp
    byte_pattern.cpp
    char_table.cpp
    dllmain.cpp
    font.cpp
    game.cpp
    gta_benchmark.cpp
    gta_font.cpp
    gta_game.cpp
    gta_html.cpp
    gta_mail_reply.cpp
    gta_menu.cpp
    gta_phone.cpp
    gta_save.cpp
    gta_string.cpp
    gta_whm.cpp
    plugin.cpp
    string_table.cpp
    whm_table.cpp
)

target_compile_definitions(plugin PRIVATE NOMINMAX _CRT_SECURE_NO_WARNINGS)

target_precompile_headers(plugin PRIVATE "${CMAKE_SOURCE_DIR}/common/stdinc.h")

target_link_libraries(plugin PRIVATE utf8::cpp utf8cpp::utf8cpp spdlog::spdlog_header_only)
target_link_libraries(plugin PRIVATE nlohmann_json::nlohmann_json)

set_target_properties(plugin PROPERTIES OUTPUT_NAME "GTA4.CHS")
set_target_properties(plugin PROPERTIES SUFFIX ".asi")
