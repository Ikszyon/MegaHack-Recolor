cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(MegaHack-Recolor VERSION 1.0.0)

# Set up the mod binary
file(GLOB SOURCE_FILES src/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

if(NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
  lib/Sig/include/Sig
)

if(NOT COMMAND CPMAddPackage)
    include(cmake/CPM.cmake)
endif()

CPMAddPackage("gh:matcool/gd-imgui-cocos#09a9555")

target_link_libraries(${PROJECT_NAME} imgui-cocos)

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# Set up dependencies, resources, and link Geode.
setup_geode_mod(${PROJECT_NAME})
