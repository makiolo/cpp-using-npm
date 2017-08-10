project(cpp-using-npm CXX)
cmake_minimum_required(VERSION 3.2.2)

################### NORMAL VERSION - NO CMAKI - NO STRANGERS CMAKES ##############
# # only need this in special for use header only libraries from npm !
# include_directories(node_modules)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
# add_executable(example src/example.cpp)
# need generate some target install
# install(TARGETS example DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
##################################################################################

##################### CMAKI VERSION (can caching artifacts) ######################
set(CMAKI_REPOSITORY "http://artifacts.myftp.biz:8080")
include("${CMAKE_SOURCE_DIR}/node_modules/cmaki/cmaki.cmake")
cmaki_setup()
cmaki_executable(example src/example.cpp)
##################################################################################
