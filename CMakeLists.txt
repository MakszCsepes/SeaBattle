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

include_directories("Headers")

message(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB CPPS "Sources/*.cpp")
file(GLOB HS "Headers/*.h")
file(GLOB PNGS "Images/*.png")
file(GLOB LS "Lua\ Scripts/*.lua")

file(MAKE_DIRECTORY ${directory} ${CMAKE_CURRENT_BINARY_DIR}/Resources)
file(MAKE_DIRECTORY ${directory} ${CMAKE_CURRENT_BINARY_DIR}/Resources/Images)
file(MAKE_DIRECTORY ${directory} ${CMAKE_CURRENT_BINARY_DIR}/Resources/Lua\ Scripts)

foreach(i IN LISTS PNGS)
    configure_file(${i} ${CMAKE_CURRENT_BINARY_DIR}/Resources/Images COPYONLY)
endforeach()

foreach(i IN LISTS LS)
    configure_file(${i} ${CMAKE_CURRENT_BINARY_DIR}/Resources/Lua\ Scripts COPYONLY)
endforeach()

#file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/Resources)
add_executable(${PROJECT_NAME} ${CPPS})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} "SDL2_ttf" lua dl)