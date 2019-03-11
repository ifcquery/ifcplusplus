CMAKE_MINIMUM_REQUIRED (VERSION 2.8.12)

IF(NOT WIN32)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF()
ENDIF(NOT WIN32)

FIND_PACKAGE(Boost) 
IF(Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS}) 
	ADD_DEFINITIONS( -DCARVE_SYSTEM_BOOST )
ENDIF()

SET(carve_VERSION_MAJOR 2)
SET(carve_VERSION_MINOR 0)
SET(carve_VERSION_PATCH 0a)
SET(CARVE_VERSION ${carve_VERSION_MAJOR}.${carve_VERSION_MINOR}.${carve_VERSION_PATCH})

CONFIGURE_FILE (
  "${CARVE_SOURCE_DIR}/src/include/carve/cmake-config.h.in"
  "${CARVE_BINARY_DIR}/include/carve/config.h"
)

SET(CARVE_SOURCE_FILES 
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
  src/lib/intersect_debug.cpp
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
  src/lib/triangulator.cpp
  src/common/geometry.cpp  
)
IF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	ADD_DEFINITIONS(-DCMAKE_BUILD)
    ADD_LIBRARY(carve SHARED ${CARVE_SOURCE_FILES})
ELSE(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    ADD_LIBRARY(carve STATIC ${CARVE_SOURCE_FILES})
ENDIF()

# Use this to request C++11 features. Dependent targets will transitively get
# this requirement as well.
TARGET_COMPILE_FEATURES(carve PUBLIC cxx_auto_type)

set_target_properties(carve PROPERTIES DEBUG_POSTFIX "d")

TARGET_INCLUDE_DIRECTORIES(carve
  PUBLIC
    "$<BUILD_INTERFACE:${CARVE_SOURCE_DIR}/src/include>"
    "$<BUILD_INTERFACE:${CARVE_BINARY_DIR}/include>" # For the generated Config file
    "$<INSTALL_INTERFACE:include>"
)

INSTALL(
    DIRECTORY   src/include/carve
    DESTINATION include
    PATTERN "*.in" EXCLUDE
)
INSTALL(
    FILES "${CARVE_BINARY_DIR}/include/carve/config.h"
    DESTINATION include/carve
)
INSTALL(
    TARGETS carve
    EXPORT  carve
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Export the targets so they can be consumed by other cmake projects
SET(target_output_filename carve-targets.cmake)
SET(target_output "${CMAKE_BINARY_DIR}/cmake/${target_output_filename}")

export(
  EXPORT carve
  FILE ${target_output}
  NAMESPACE IFCPP::)

install(
  EXPORT carve
  DESTINATION ${IFCPP_CONFIG_DIR}
  FILE ${target_output_filename}
  NAMESPACE IFCPP::)
