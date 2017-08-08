project(cpp-using-npm CXX)
cmake_minimum_required(VERSION 3.0.2)

# only need this in special for use header only libraries from npm !
include_directories(node_modules)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
add_executable(example example.cpp)
install(TARGETS example DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
