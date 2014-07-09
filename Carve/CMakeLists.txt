cmake_minimum_required (VERSION 2.8)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF(NOT CMAKE_BUILD_TYPE)


message(STATUS "Carve: directories of Carve")
message(STATUS "source: " ${CARVE_SOURCE_DIR})
message(STATUS "binary: " ${CARVE_BINARY_DIR})

INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_SOURCE_DIR}/src/include)

configure_file (
  "${CARVE_SOURCE_DIR}/src/include/carve/cmake-config.h.in"
  "${CARVE_BINARY_DIR}/include/carve/config.h"
)
add_definitions(-DCMAKE_BUILD)

INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_BINARY_DIR}/include) # For the generated Config file


find_package(Boost 1.53)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  message(STATUS "Using Boost libraries for Carve")
else(Boost_FOUND)
  message(FATAL_ERROR "Could not find Boost libraries for Carve")
endif(Boost_FOUND)
add_definitions(-DCARVE_SYSTEM_BOOST)


set(CARVE_SOURCE_FILES 
  src/lib/aabb.cpp
  src/lib/carve.cpp
  src/lib/convex_hull.cpp
  src/lib/csg.cpp
  src/lib/csg_collector.cpp
  src/lib/edge.cpp
  src/lib/face.cpp
  src/lib/geom.cpp
  src/lib/geom2d.cpp
  src/lib/geom3d.cpp
  src/lib/intersect.cpp
  src/lib/intersect_classify_edge.cpp
  src/lib/intersect_classify_group.cpp
  src/lib/intersect_common.hpp
  src/lib/intersect_debug.cpp
  src/lib/intersect_debug.hpp
  src/lib/intersect_face_division.cpp
  src/lib/intersect_group.cpp
  src/lib/intersect_half_classify_group.cpp
  src/lib/intersection.cpp
  src/lib/math.cpp
  src/lib/mesh.cpp
  src/lib/octree.cpp
  src/lib/pointset.cpp
  src/lib/polyhedron.cpp
  src/lib/polyline.cpp
  src/lib/shewchuk_predicates.cpp
  src/lib/tag.cpp
  src/lib/timing.cpp
  src/lib/triangle_intersection.cpp
  src/lib/triangulator.cpp
)

ADD_LIBRARY(carve SHARED ${CARVE_SOURCE_FILES})

add_subdirectory(src/common) 
