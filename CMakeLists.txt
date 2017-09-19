project(cpp-using-npm CXX)
cmake_minimum_required(VERSION 3.0)

################### NORMAL VERSION - NO CMAKI - NO STRANGERS CMAKES ##############
# # only need this in special for use header only libraries from npm !
# include_directories(node_modules)
# set(CMAKE_CXX_STANDARD 14)
# add_executable(example src/example.cpp)
# need generate some target install
# install(TARGETS example DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
##################################################################################

##################### CMAKI VERSION (can caching artifacts) ######################
include("$ENV{CMAKI_PWD}/node_modules/cmaki/cmaki.cmake")
cmaki_find_package(boost-headers)
cmaki_find_package(fast-event-system)
cmaki2_executable(example src/example.cpp PTHREADS)
##################################################################################
