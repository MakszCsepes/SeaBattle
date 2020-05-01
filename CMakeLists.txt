cmake_minimum_required(VERSION 3.15)
project(SeaBattle)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

include_directories(${SDL2_INCLUDE_DIR})
# SFML
#link_directories(${PROJECT_SOURCE_DIR}/lib)

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
file(GLOB CPPS "Sources/*.cpp")
file(GLOB HS "Headers/*.h")

include_directories("${PROJECT_SOURCE_DIR}/Headers")
include_directories("${PROJECT_SOURCE_DIR}/Sources")
include_directories("${PROJECT_SOURCE_DIR}/Lua Scripts")

add_executable(${PROJECT_NAME} ${CPPS})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} "SDL2_ttf" lua dl)