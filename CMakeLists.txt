cmake_minimum_required(VERSION 3.15)
project(SeaBattle)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

add_executable(SeaBattle main.cpp game.cpp draw.cpp artifical_inteligence.cpp)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})

target_link_libraries(${CMAKE_PROJECT_NAME} ${SDL2_LIBRARY} "SDL2_ttf")