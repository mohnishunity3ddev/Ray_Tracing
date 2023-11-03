cmake_minimum_required(VERSION 3.22.0)

project(01.shading_with_surface_normals)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(01.shading_with_surface_normals "Chapter2")

if(TARGET SharedUtils)
target_link_libraries(01.shading_with_surface_normals SharedUtils)
endif()
