cmake_minimum_required(VERSION 3.15.0)

project(whm_io)

add_executable(whm_io HtmlTextExport.cpp WhmIO.cpp ${common_sources})
