cmake_minimum_required(VERSION 3.24)

#FOR WINDOWS AND VCPKG (if you build for MinGW, comment or delete this block)
##################################
#change to your way to vcpkg (need to correct work)
set(your_way_to_vcpkg D:/VCPKG)

set(CMAKE_TOOLCHAIN_FILE ${your_way_to_vcpkg}/scripts/buildsystems/vcpkg.cmake)
###################################
project(ParseProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall)

find_package(Poco REQUIRED JSON Net Foundation)
set(SRC ParseProject.cpp)
set(HEADERS ParseProject.h)


add_executable(ParseProject ${SRC} ${HEADERS})

target_link_libraries(ParseProject PRIVATE Poco::JSON Poco::Net Poco::Foundation) 