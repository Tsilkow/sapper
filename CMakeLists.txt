cmake_minimum_required(VERSION 3.10)

project(Sapper VERSION 1.0)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories("inc")
file(GLOB SOURCES "src/*.cpp")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_library(Modules SHARED ${SOURCES})

add_executable(Sapper main.cpp)
target_compile_options(Sapper PRIVATE -Wall)
target_link_libraries(Sapper sfml-graphics sfml-audio Modules)
