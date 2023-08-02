cmake_minimum_required(VERSION 3.10.0)

file(READ env/MOD_NAME PROJECT_NAME)

project(${PROJECT_NAME})

add_library(${PROJECT_NAME} SHARED src/main.cpp)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
  set(CMAKE_SIZEOF_VOID_P 4)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -target i386-pc-windows-msvc")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -target i386-pc-windows-msvc")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
  src

  lib
  lib/Mega-Hack-Extensions/include

  lib/minhook/include

  lib/json/single_include/nlohmann

  lib/Sig/include/Sig
)

target_link_libraries(${PROJECT_NAME} minhook ${CMAKE_SOURCE_DIR}/lib/Mega-Hack-Extensions/bin/hackpro.lib)

add_subdirectory(lib/minhook)