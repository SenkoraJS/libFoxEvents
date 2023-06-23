cmake_minimum_required(VERSION 3.22)
project(foxevents VERSION 0.2 LANGUAGES CXX)

add_subdirectory(foxWorkers/)
include_directories(foxWorkers/src)

add_library(foxevents src/foxevents.cpp)
add_executable(foxevents_test test.cpp)
target_link_libraries(foxevents_test foxevents foxworkers)