cmake_minimum_required(VERSION 3.22.0)

project(03.send_rays_into_the_scene)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(03.send_rays_into_the_scene "Chapter1")

if(TARGET SharedUtils)
target_link_libraries(03.send_rays_into_the_scene SharedUtils)
endif()
