cmake_minimum_required(VERSION 3.2)

project(ParseProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall)

find_package(Poco REQUIRED JSON Net Foundation XML Util NetSSL)
find_package(OpenSSL REQUIRED)
set(SRC ParseProject.cpp Responcer.cpp)
set(HEADERS ParseProject.h Responcer.h FunctionErrorsVariables.h)

add_executable(ParseProject ${SRC} ${HEADERS})

target_link_libraries(ParseProject PUBLIC Poco::Foundation Poco::XML Poco::JSON Poco::Util Poco::Net Poco::NetSSL) 