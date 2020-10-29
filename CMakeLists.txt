CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
project(tp4)
include_directories(. source)
set(CMAKE_BUILD_TYPE Debug)

ADD_EXECUTABLE(TP6 main.cpp source/earth_map.cpp source/spheric.cpp
	source/scenario.cpp)
