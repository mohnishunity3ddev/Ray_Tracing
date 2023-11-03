cmake_minimum_required(VERSION 3.22.0)

project(02.simplify_raySphere_intersection)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(02.simplify_raySphere_intersection "Chapter2")

if(TARGET SharedUtils)
target_link_libraries(02.simplify_raySphere_intersection SharedUtils)
endif()
