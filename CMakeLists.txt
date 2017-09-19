project(cpp-using-npm CXX)
cmake_minimum_required(VERSION 3.0)
include("$ENV{CMAKI_PWD}/node_modules/cmaki/cmaki.cmake")
cmaki_executable(example src/example.cpp)
