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
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_SOURCE_DIR}/src/include)
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CARVE_BINARY_DIR}/include) # For the generated Config file

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
IF(CMAKE_COMPILER_IS_GNUCXX)
	ADD_DEFINITIONS(-DCMAKE_BUILD)
    ADD_LIBRARY(carve SHARED ${CARVE_SOURCE_FILES})
ELSE(CMAKE_COMPILER_IS_GNUCXX)
    ADD_LIBRARY(carve STATIC ${CARVE_SOURCE_FILES})
ENDIF()

set_target_properties(carve PROPERTIES DEBUG_POSTFIX "d")


INSTALL(
    DIRECTORY   src/include/carve
    DESTINATION include
)
INSTALL(
		TARGETS carve
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION bin
		ARCHIVE DESTINATION lib
	)
    
IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
ENDIF()
 
