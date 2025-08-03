cmake_minimum_required(VERSION 3.9)
project("mod_autocomplete_bots_quest")

add_compile_options(-std=c++20)

include_directories(${CMAKE_SOURCE_DIR}/src/server)

file(GLOB_RECURSE sources src/*.cpp)

add_library(${PROJECT_NAME} MODULE ${sources})

target_link_libraries(${PROJECT_NAME} PRIVATE game)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/modules/mod-playerbots
)
