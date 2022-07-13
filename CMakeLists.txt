cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(ParseProject)
set(CMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE)
set(your_way_to_vcpkg D:/VCPKG)

include(${your_way_to_vcpkg}/scripts/buildsystems/vcpkg.cmake REQUIRED)

find_package(Poco REQUIRED JSON)

set(SRC src/ParseProject.cpp)
set(HEADERS h/ParseProject.h)

add_executable(ParseProject ${SRC})

target_link_libraries(ParseProject PRIVATE Poco::JSON)