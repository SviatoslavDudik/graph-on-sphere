CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
project (Graph-on-Sphere)
include_directories (lib)

ADD_EXECUTABLE (
	simul
	src/main.cpp
	src/earth_map.cpp
	src/spheric.cpp
	src/scenario.cpp
	src/test_graph.cpp
	src/test_spheric.cpp
)
