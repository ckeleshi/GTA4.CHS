cmake_minimum_required(VERSION 3.16.0)

project(whm_io)

add_executable(whm_io
    HTMLDocument.h
    HtmlTextExport.h
    HtmlTextExport.cpp
    pgBase.h
    rsc_header.h
    main.cpp
    ${common_sources})
