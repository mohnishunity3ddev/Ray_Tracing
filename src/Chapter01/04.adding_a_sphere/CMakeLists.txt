cmake_minimum_required(VERSION 3.22.0)

project(04.adding_a_sphere)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(04.adding_a_sphere "Chapter1")

if(TARGET SharedUtils)
target_link_libraries(04.adding_a_sphere SharedUtils)
endif()
