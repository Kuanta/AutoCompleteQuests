cmake_minimum_required(VERSION 3.9)
project("mod_autocomplete_bots_quest")

add_definitions(-std=c++17)

include_directories(${CMAKE_SOURCE_DIR}/src/server)

file(GLOB_RECURSE sources src/*.cpp)

add_library(${PROJECT_NAME} MODULE ${sources})

target_link_libraries(${PROJECT_NAME} PRIVATE game)
