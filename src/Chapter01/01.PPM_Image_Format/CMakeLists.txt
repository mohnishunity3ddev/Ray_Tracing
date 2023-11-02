cmake_minimum_required(VERSION 3.22.0)

project(01.PPM_Image_Format)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(01.PPM_Image_Format "Chapter1")

if(TARGET SharedUtils)
target_link_libraries(01.PPM_Image_Format SharedUtils)
endif()
