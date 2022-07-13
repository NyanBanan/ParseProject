cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#change to your way to vcpkg (need to correct work)
set(your_way_to_vcpkg D:/VCPKG)

set(CMAKE_TOOLCHAIN_FILE ${your_way_to_vcpkg}/scripts/buildsystems/vcpkg.cmake)

project(ParseProject)

find_package(Poco REQUIRED JSON NET)
set(SRC src/ParseProject.cpp)
set(HEADERS h/ParseProject.h)

add_executable(ParseProject ${SRC})

target_link_libraries(ParseProject PRIVATE Poco::JSON POCO::NET)