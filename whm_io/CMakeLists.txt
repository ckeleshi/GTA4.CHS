cmake_minimum_required(VERSION 3.24)

project(whm_io)

find_package(fmt CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

add_executable(whm_io
    ${CMAKE_SOURCE_DIR}/common/common.cpp
    ${CMAKE_SOURCE_DIR}/common/fnv_hash.cpp
    ${CMAKE_SOURCE_DIR}/common/module_path.cpp
    HTMLDocument.h
    HtmlTextExport.h
    HtmlTextExport.cpp
    pgBase.h
    rsc_header.h
    main.cpp
)

target_compile_definitions(whm_io PRIVATE NOMINMAX)

target_precompile_headers(whm_io PRIVATE "${CMAKE_SOURCE_DIR}/common/stdinc.h")

target_link_libraries(whm_io PRIVATE ZLIB::ZLIB fmt::fmt-header-only rapidjson)
