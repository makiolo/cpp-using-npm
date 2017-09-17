project(cpp-using-npm CXX)
cmake_minimum_required(VERSION 3.0)

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
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/../cmaki")
	include("${CMAKE_CURRENT_LIST_DIR}/../cmaki/cmaki.cmake")
else()
	include("${CMAKE_CURRENT_LIST_DIR}/node_modules/cmaki/cmaki.cmake")
endif()
cmaki_setup()
cmaki_find_package(boost-headers)
cmaki_find_package(fast-event-system)
cmaki2_executable(example src/example.cpp PTHREADS)
##################################################################################

