cmake_minimum_required(VERSION 3.22.0)

project(01.RayTracer)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(01.RayTracer "Chapter1")

if(TARGET SharedUtils)
target_link_libraries(01.RayTracer SharedUtils)
endif()
