cmake_minimum_required(VERSION 3.22.0)

project(02.progress.indicator)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(02.progress.indicator "Chapter1")

if(TARGET SharedUtils)
target_link_libraries(02.progress.indicator SharedUtils)
endif()
